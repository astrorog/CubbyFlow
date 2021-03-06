/*************************************************************************
> File Name: main.cpp
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: Smoke Simulator
> Created Time: 2017/09/07
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <Core/Array/Array2.h>
#include <Core/Collider/RigidBodyCollider3.h>
#include <Core/Emitter/VolumeGridEmitter3.h>
#include <Core/Geometry/Box3.h>
#include <Core/Geometry/ImplicitTriangleMesh3.h>
#include <Core/Geometry/Sphere3.h>
#include <Core/Geometry/TriangleMesh3.h>
#include <Core/Grid/ScalarGrid3.h>
#include <Core/Math/MathUtils.h>
#include <Core/SemiLagrangian/CubicSemiLagrangian3.h>
#include <Core/SemiLagrangian/SemiLagrangian3.h>
#include <Core/Solver/Smoke/GridSmokeSolver3.h>
#include <Core/Surface/Implicit/CustomImplicitSurface3.h>
#include <Core/Utils/Logging.h>

#include <pystring/pystring.h>

#ifdef CUBBYFLOW_WINDOWS
#include <direct.h>
#else
#include <sys/stat.h>
#endif

#include <getopt.h>

#include <fstream>
#include <string>
#include <vector>

#define APP_NAME "SmokeSim"

using namespace CubbyFlow;

const size_t EDGE_BLUR = 3;
const float EDGE_BLUR_F = 3.f;
const double TGA_SCALE = 10.0;

inline float SmoothStep(float edge0, float edge1, float x)
{
	const float t = Clamp((x - edge0) / (edge1 - edge0), 0.f, 1.f);
	return t * t * (3.f - 2.f * t);
}

// Export density field to Mitsuba volume file.
void SaveVolumeAsVol(const ScalarGrid3Ptr& density, const std::string& rootDir, int frameCnt)
{
	char baseName[256];
	snprintf(baseName, sizeof(baseName), "frame_%06d.vol", frameCnt);
	std::string fileName = pystring::os::path::join(rootDir, baseName);
	std::ofstream file(fileName.c_str(), std::ofstream::binary);	
	if (file)
	{
		printf("Writing %s...\n", fileName.c_str());

		// Mitsuba 0.5.0 grid-volume format
		char header[48];
		memset(header, 0, sizeof(header));

		header[0] = 'V';
		header[1] = 'O';
		header[2] = 'L';
		header[3] = 3;

		int32_t* encoding = reinterpret_cast<int32_t*>(header + 4);
		encoding[0] = 1;  // 32-bit float
		encoding[1] = static_cast<int32_t>(density->GetDataSize().x);
		encoding[2] = static_cast<int32_t>(density->GetDataSize().y);
		encoding[3] = static_cast<int32_t>(density->GetDataSize().z);
		encoding[4] = 1;  // number of channels

		const BoundingBox3D domain = density->BoundingBox();
		float* bbox = reinterpret_cast<float*>(encoding + 5);
		bbox[0] = static_cast<float>(domain.lowerCorner.x);
		bbox[1] = static_cast<float>(domain.lowerCorner.y);
		bbox[2] = static_cast<float>(domain.lowerCorner.z);
		bbox[3] = static_cast<float>(domain.upperCorner.x);
		bbox[4] = static_cast<float>(domain.upperCorner.y);
		bbox[5] = static_cast<float>(domain.upperCorner.z);

		file.write(header, sizeof(header));

		Array3<float> data(density->GetDataSize());
		data.ParallelForEachIndex([&](size_t i, size_t j, size_t k)
		{
			float d = static_cast<float>((*density)(i, j, k));

			// Blur the edge for less-noisy rendering
			if (i < EDGE_BLUR)
			{
				d *= SmoothStep(0.f, EDGE_BLUR_F, static_cast<float>(i));
			}
			if (i > data.size().x - 1 - EDGE_BLUR)
			{
				d *= SmoothStep(0.f, EDGE_BLUR_F, static_cast<float>((data.size().x - 1) - i));
			}
			if (j < EDGE_BLUR)
			{
				d *= SmoothStep(0.f, EDGE_BLUR_F, static_cast<float>(j));
			}
			if (j > data.size().y - 1 - EDGE_BLUR)
			{
				d *= SmoothStep(0.f, EDGE_BLUR_F, static_cast<float>((data.size().y - 1) - j));
			}
			if (k < EDGE_BLUR)
			{
				d *= SmoothStep(0.f, EDGE_BLUR_F, static_cast<float>(k));
			}
			if (k > data.size().z - 1 - EDGE_BLUR)
			{
				d *= SmoothStep(0.f, EDGE_BLUR_F, static_cast<float>((data.size().z - 1) - k));
			}

			data(i, j, k) = d;
		});

		file.write(
			reinterpret_cast<const char*>(data.data()),
			sizeof(float) * data.size().x * data.size().y * data.size().z);

		file.close();
	}
}

void SaveVolumeAsTga(const ScalarGrid3Ptr& density, const std::string& rootDir, int frameCnt)
{
	char baseName[256];
	snprintf(baseName, sizeof(baseName), "frame_%06d.tga", frameCnt);
	std::string fileName = pystring::os::path::join(rootDir, baseName);
	std::ofstream file(fileName.c_str(), std::ofstream::binary);
	if (file)
	{
		printf("Writing %s...\n", fileName.c_str());

		Size3 dataSize = density->GetDataSize();

		std::array<char, 18> header;
		header.fill(0);

		const int imgWidth = static_cast<int>(dataSize.x);
		const int imgHeight = static_cast<int>(dataSize.y);

		header[2] = 2;
		header[12] = static_cast<char>(imgWidth & 0xff);
		header[13] = static_cast<char>((imgWidth & 0xff00) >> 8);
		header[14] = static_cast<char>(imgHeight & 0xff);
		header[15] = static_cast<char>((imgHeight & 0xff00) >> 8);
		header[16] = 24;

		file.write(header.data(), header.size());

		Array2<double> hdrImg(dataSize.x, dataSize.y);
		hdrImg.ParallelForEachIndex([&](size_t i, size_t j)
		{
			double sum = 0.0;
			for (size_t k = 0; k < dataSize.z; ++k)
			{
				sum += (*density)(i, j, k);
			}
			hdrImg(i, j) = TGA_SCALE * sum / static_cast<double>(dataSize.z);
		});

		std::vector<char> img(3 * dataSize.x * dataSize.y);
		for (size_t i = 0; i < dataSize.x * dataSize.y; ++i)
		{
			const char val = static_cast<char>(Clamp(hdrImg[i], 0.0, 1.0) * 255.0);
			img[3 * i + 0] = val;
			img[3 * i + 1] = val;
			img[3 * i + 2] = val;
		}

		file.write(img.data(), img.size());

		file.close();
	}
}

void PrintUsage() 
{
	printf(
		"Usage: " APP_NAME " [options]\n"
		"   -r, --resx: grid resolution in x-axis (default is 50)\n"
		"   -f, --frames: total number of frames (default is 100)\n"
		"   -p, --fps: frames per second (default is 60.0)\n"
		"   -l, --log: log file name (default is " APP_NAME ".log)\n"
		"   -o, --output: output directory name (default is " APP_NAME "_output)\n"
		"   -e, --example: example number (between 1 and 5, default is 1)\n"
		"   -m, --format: particle output format (tga or vol. default is tga)\n"
		"   -h, --help: print this message\n");
}

void PrintInfo(const GridSmokeSolver3Ptr& solver)
{
	const auto grids = solver->GetGridSystemData();
	const Size3 resolution = grids->GetResolution();
	const BoundingBox3D domain = grids->GetBoundingBox();
	const Vector3D gridSpacing = grids->GetGridSpacing();

	printf(
		"Resolution: %zu x %zu x %zu\n",
		resolution.x, resolution.y, resolution.z);
	printf(
		"Domain: [%f, %f, %f] x [%f, %f, %f]\n",
		domain.lowerCorner.x, domain.lowerCorner.y, domain.lowerCorner.z,
		domain.upperCorner.x, domain.upperCorner.y, domain.upperCorner.z);
	printf(
		"Grid spacing: [%f, %f, %f]\n",
		gridSpacing.x, gridSpacing.y, gridSpacing.z);
}

void RunSimulation(const std::string& rootDir, const GridSmokeSolver3Ptr& solver, int numberOfFrames, const std::string& format, double fps)
{
	const auto density = solver->GetSmokeDensity();

	for (Frame frame(0, 1.0 / fps); frame.index < numberOfFrames; ++frame)
	{
		solver->Update(frame);

		if (format == "vol")
		{
			SaveVolumeAsVol(density, rootDir, frame.index);
		}
		else if (format == "tga")
		{
			SaveVolumeAsTga(density, rootDir, frame.index);
		}
	}
}

void RunExample1(const std::string& rootDir, size_t resolutionX, int numberOfFrames, const std::string& format, double fps)
{
	// Build solver
	auto solver = GridSmokeSolver3::Builder()
		.WithResolution({ resolutionX, 2 * resolutionX, resolutionX })
		.WithDomainSizeX(1.0)
		.MakeShared();

	solver->SetAdvectionSolver(std::make_shared<CubicSemiLagrangian3>());

	const auto grids = solver->GetGridSystemData();
	BoundingBox3D domain = grids->GetBoundingBox();

	// Build emitter
	const auto box = Box3::Builder()
		.WithLowerCorner({ 0.45, -1, 0.45 })
		.WithUpperCorner({ 0.55, 0.05, 0.55 })
		.MakeShared();

	auto emitter = VolumeGridEmitter3::Builder()
		.WithSourceRegion(box)
		.WithIsOneShot(false)
		.MakeShared();

	solver->SetEmitter(emitter);
	emitter->AddStepFunctionTarget(solver->GetSmokeDensity(), 0, 1);
	emitter->AddStepFunctionTarget(solver->GetTemperature(), 0, 1);

	// Build collider
	const auto sphere = Sphere3::Builder()
		.WithCenter({ 0.5, 0.3, 0.5 })
		.WithRadius(0.075 * domain.Width())
		.MakeShared();

	const auto collider = RigidBodyCollider3::Builder()
		.WithSurface(sphere)
		.MakeShared();

	solver->SetCollider(collider);

	// Print simulation info
	printf("Running example 1 (rising smoke with cubic-spline advection)\n");
	PrintInfo(solver);

	// Run simulation
	RunSimulation(rootDir, solver, numberOfFrames, format, fps);
}

void RunExample2(const std::string& rootDir, size_t resolutionX, int numberOfFrames, const std::string& format, double fps)
{
	// Build solver
	auto solver = GridSmokeSolver3::Builder()
		.WithResolution({ resolutionX, 2 * resolutionX, resolutionX })
		.WithDomainSizeX(1.0)
		.MakeShared();

	solver->SetAdvectionSolver(std::make_shared<SemiLagrangian3>());

	const auto grids = solver->GetGridSystemData();
	BoundingBox3D domain = grids->GetBoundingBox();

	// Build emitter
	const auto box = Box3::Builder()
		.WithLowerCorner({ 0.45, -1, 0.45 })
		.WithUpperCorner({ 0.55, 0.05, 0.55 })
		.MakeShared();

	auto emitter = VolumeGridEmitter3::Builder()
		.WithSourceRegion(box)
		.WithIsOneShot(false)
		.MakeShared();

	solver->SetEmitter(emitter);
	emitter->AddStepFunctionTarget(solver->GetSmokeDensity(), 0, 1);
	emitter->AddStepFunctionTarget(solver->GetTemperature(), 0, 1);

	// Build collider
	const auto sphere = Sphere3::Builder()
		.WithCenter({ 0.5, 0.3, 0.5 })
		.WithRadius(0.075 * domain.Width())
		.MakeShared();

	const auto collider = RigidBodyCollider3::Builder()
		.WithSurface(sphere)
		.MakeShared();

	solver->SetCollider(collider);

	// Print simulation info
	printf("Running example 2 (rising smoke with linear advection)\n");
	PrintInfo(solver);

	// Run simulation
	RunSimulation(rootDir, solver, numberOfFrames, format, fps);
}

void RunExample3(const std::string& rootDir, size_t resolutionX, int numberOfFrames, const std::string& format, double fps)
{
	// Build solver
	auto solver = GridSmokeSolver3::Builder()
		.WithResolution({ resolutionX, resolutionX / 4 * 5, resolutionX / 2 })
		.WithDomainSizeX(2.0)
		.WithOrigin({ -1, -0.15, -0.5 })
		.MakeShared();

	// Build emitter
	auto dragonMesh = TriangleMesh3::Builder().MakeShared();
	std::ifstream objFile("Resources/dragon.obj");
	if (objFile)
	{
		dragonMesh->ReadObj(&objFile);
	}
	else
	{
		fprintf(stderr, "Cannot open Resources/dragon.obj\n");
		exit(EXIT_FAILURE);
	}

	const auto dragon = ImplicitTriangleMesh3::Builder()
		.WithTriangleMesh(dragonMesh)
		.WithResolutionX(resolutionX)
		.MakeShared();

	auto emitter = VolumeGridEmitter3::Builder()
		.WithSourceRegion(dragon)
		.WithIsOneShot(false)
		.MakeShared();

	solver->SetEmitter(emitter);
	emitter->AddStepFunctionTarget(solver->GetSmokeDensity(), 0, 1);
	emitter->AddStepFunctionTarget(solver->GetTemperature(), 0, 1);

	// Print simulation info
	printf("Running example 3 (rising dragon)\n");
	PrintInfo(solver);

	// Run simulation
	RunSimulation(rootDir, solver, numberOfFrames, format, fps);
}

void RunExample4(const std::string& rootDir, size_t resolutionX, int numberOfFrames, const std::string& format, double fps)
{
	// Build solver
	auto solver = GridSmokeSolver3::Builder()
		.WithResolution({ resolutionX, 6 * resolutionX / 5, resolutionX / 2 })
		.WithDomainSizeX(1.0)
		.MakeShared();

	solver->SetBuoyancyTemperatureFactor(2.0);

	// Build emitter
	const auto box = Box3::Builder()
		.WithLowerCorner({ 0.05, 0.1, 0.225 })
		.WithUpperCorner({ 0.1, 0.15, 0.275 })
		.MakeShared();

	auto emitter = VolumeGridEmitter3::Builder()
		.WithSourceRegion(box)
		.WithIsOneShot(false)
		.MakeShared();

	solver->SetEmitter(emitter);
	emitter->AddStepFunctionTarget(solver->GetSmokeDensity(), 0, 1);
	emitter->AddStepFunctionTarget(solver->GetTemperature(), 0, 1);
	emitter->AddTarget(solver->GetVelocity(),
		[](double sdf, const Vector3D&, const Vector3D& oldVal)
	{
		if (sdf < 0.05)
		{
			return Vector3D(0.5, oldVal.y, oldVal.z);
		}
		else
		{
			return Vector3D(oldVal);
		}
	});

	// Print simulation info
	printf("Running example 4 (rising smoke with cubic-spline advection)\n");
	PrintInfo(solver);

	// Run simulation
	RunSimulation(rootDir, solver, numberOfFrames, format, fps);
}

void RunExample5(const std::string& rootDir, size_t resolutionX, int numberOfFrames, const std::string& format, double fps)
{
	// Build solver
	auto solver = GridSmokeSolver3::Builder()
		.WithResolution({ resolutionX, 6 * resolutionX / 5, resolutionX / 2 })
		.WithDomainSizeX(1.0)
		.MakeShared();

	solver->SetBuoyancyTemperatureFactor(2.0);
	solver->SetAdvectionSolver(std::make_shared<SemiLagrangian3>());

	// Build emitter
	const auto box = Box3::Builder()
		.WithLowerCorner({ 0.05, 0.1, 0.225 })
		.WithUpperCorner({ 0.1, 0.15, 0.275 })
		.MakeShared();

	auto emitter = VolumeGridEmitter3::Builder()
		.WithSourceRegion(box)
		.WithIsOneShot(false)
		.MakeShared();

	solver->SetEmitter(emitter);
	emitter->AddStepFunctionTarget(solver->GetSmokeDensity(), 0, 1);
	emitter->AddStepFunctionTarget(solver->GetTemperature(), 0, 1);
	emitter->AddTarget(solver->GetVelocity(),
		[](double sdf, const Vector3D& pt, const Vector3D& oldVal)
	{
		UNUSED_VARIABLE(pt);

		if (sdf < 0.05)
		{
			return Vector3D(0.5, oldVal.y, oldVal.z);
		}
		else
		{
			return Vector3D(oldVal);
		}
	});

	// Print simulation info
	printf("Running example 5 (rising smoke with linear advection)\n");
	PrintInfo(solver);

	// Run simulation
	RunSimulation(rootDir, solver, numberOfFrames, format, fps);
}

int main(int argc, char* argv[])
{
	size_t resolutionX = 50;
	int numberOfFrames = 100;
	double fps = 60.0;
	int exampleNum = 1;
	std::string logFileName = APP_NAME ".log";
	std::string outputDir = APP_NAME "_output";
	std::string format = "tga";

	// Parse options
	static struct option longOptions[] =
	{
		{ "resx",      optional_argument, nullptr, 'r' },
		{ "frames",    optional_argument, nullptr, 'f' },
		{ "fps",       optional_argument, nullptr, 'p' },
		{ "example",   optional_argument, nullptr, 'e' },
		{ "log",       optional_argument, nullptr, 'l' },
		{ "outputDir", optional_argument, nullptr, 'o' },
		{ "format",    optional_argument, nullptr, 'm' },
		{ "help",      optional_argument, nullptr, 'h' },
		{ nullptr,     0,                 nullptr,  0 }
	};

	int opt;
	int long_index = 0;
	while ((opt = getopt_long(argc, argv, "r:f:p:e:l:o:m:h", longOptions, &long_index)) != -1)
	{
		switch (opt)
		{
		case 'r':
			resolutionX = static_cast<size_t>(atoi(optarg));
			break;
		case 'f':
			numberOfFrames = atoi(optarg);
			break;
		case 'p':
			fps = atof(optarg);
			break;
		case 'e':
			exampleNum = atoi(optarg);
			break;
		case 'l':
			logFileName = optarg;
			break;
		case 'o':
			outputDir = optarg;
			break;
		case 'm':
			format = optarg;
			if (format != "vol" && format != "tga")
			{
				PrintUsage();
				exit(EXIT_FAILURE);
			}
			break;
		case 'h':
			PrintUsage();
			exit(EXIT_SUCCESS);
		default:
			PrintUsage();
			exit(EXIT_FAILURE);
		}
	}

#ifdef CUBBYFLOW_WINDOWS
	_mkdir(outputDir.c_str());
#else
	mkdir(outputDir.c_str(), S_IRWXU | S_IRWXG | S_IRWXO);
#endif

	std::ofstream logFile(logFileName.c_str());
	if (logFile)
	{
		Logging::SetAllStream(&logFile);
	}

	switch (exampleNum)
	{
	case 1:
		RunExample1(outputDir, resolutionX, numberOfFrames, format, fps);
		break;
	case 2:
		RunExample2(outputDir, resolutionX, numberOfFrames, format, fps);
		break;
	case 3:
		RunExample3(outputDir, resolutionX, numberOfFrames, format, fps);
		break;
	case 4:
		RunExample4(outputDir, resolutionX, numberOfFrames, format, fps);
		break;
	case 5:
		RunExample5(outputDir, resolutionX, numberOfFrames, format, fps);
		break;
	default:
		PrintUsage();
		exit(EXIT_FAILURE);
	}

	return EXIT_SUCCESS;
}