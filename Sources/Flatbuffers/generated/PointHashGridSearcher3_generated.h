// automatically generated by the FlatBuffers compiler, do not modify

#ifndef FLATBUFFERS_GENERATED_POINTHASHGRIDSEARCHER3_CUBBYFLOW_FBS_H_
#define FLATBUFFERS_GENERATED_POINTHASHGRIDSEARCHER3_CUBBYFLOW_FBS_H_

#include "flatbuffers/flatbuffers.h"

#include "BasicTypes_generated.h"

namespace CubbyFlow {
namespace fbs {

struct PointHashGridSearcherBucket3;

struct PointHashGridSearcher3;

struct PointHashGridSearcherBucket3 FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  enum {
    VT_DATA = 4
  };
  const flatbuffers::Vector<uint64_t> *Data() const { return GetPointer<const flatbuffers::Vector<uint64_t> *>(VT_DATA); }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<flatbuffers::uoffset_t>(verifier, VT_DATA) &&
           verifier.Verify(Data()) &&
           verifier.EndTable();
  }
};

struct PointHashGridSearcherBucket3Builder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_Data(flatbuffers::Offset<flatbuffers::Vector<uint64_t>> data) { fbb_.AddOffset(PointHashGridSearcherBucket3::VT_DATA, data); }
  PointHashGridSearcherBucket3Builder(flatbuffers::FlatBufferBuilder &_fbb) : fbb_(_fbb) { start_ = fbb_.StartTable(); }
  PointHashGridSearcherBucket3Builder &operator=(const PointHashGridSearcherBucket3Builder &);
  flatbuffers::Offset<PointHashGridSearcherBucket3> Finish() {
    auto o = flatbuffers::Offset<PointHashGridSearcherBucket3>(fbb_.EndTable(start_, 1));
    return o;
  }
};

inline flatbuffers::Offset<PointHashGridSearcherBucket3> CreatePointHashGridSearcherBucket3(flatbuffers::FlatBufferBuilder &_fbb,
    flatbuffers::Offset<flatbuffers::Vector<uint64_t>> data = 0) {
  PointHashGridSearcherBucket3Builder builder_(_fbb);
  builder_.add_Data(data);
  return builder_.Finish();
}

inline flatbuffers::Offset<PointHashGridSearcherBucket3> CreatePointHashGridSearcherBucket3Direct(flatbuffers::FlatBufferBuilder &_fbb,
    const std::vector<uint64_t> *data = nullptr) {
  return CreatePointHashGridSearcherBucket3(_fbb, data ? _fbb.CreateVector<uint64_t>(*data) : 0);
}

struct PointHashGridSearcher3 FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  enum {
    VT_GRIDSPACING = 4,
    VT_RESOLUTION = 6,
    VT_POINTS = 8,
    VT_BUCKETS = 10
  };
  double GridSpacing() const { return GetField<double>(VT_GRIDSPACING, 0.0); }
  const CubbyFlow::fbs::Size3 *Resolution() const { return GetStruct<const CubbyFlow::fbs::Size3 *>(VT_RESOLUTION); }
  const flatbuffers::Vector<const CubbyFlow::fbs::Vector3D *> *Points() const { return GetPointer<const flatbuffers::Vector<const CubbyFlow::fbs::Vector3D *> *>(VT_POINTS); }
  const flatbuffers::Vector<flatbuffers::Offset<PointHashGridSearcherBucket3>> *Buckets() const { return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<PointHashGridSearcherBucket3>> *>(VT_BUCKETS); }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<double>(verifier, VT_GRIDSPACING) &&
           VerifyField<CubbyFlow::fbs::Size3>(verifier, VT_RESOLUTION) &&
           VerifyField<flatbuffers::uoffset_t>(verifier, VT_POINTS) &&
           verifier.Verify(Points()) &&
           VerifyField<flatbuffers::uoffset_t>(verifier, VT_BUCKETS) &&
           verifier.Verify(Buckets()) &&
           verifier.VerifyVectorOfTables(Buckets()) &&
           verifier.EndTable();
  }
};

struct PointHashGridSearcher3Builder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_GridSpacing(double gridSpacing) { fbb_.AddElement<double>(PointHashGridSearcher3::VT_GRIDSPACING, gridSpacing, 0.0); }
  void add_Resolution(const CubbyFlow::fbs::Size3 *resolution) { fbb_.AddStruct(PointHashGridSearcher3::VT_RESOLUTION, resolution); }
  void add_Points(flatbuffers::Offset<flatbuffers::Vector<const CubbyFlow::fbs::Vector3D *>> points) { fbb_.AddOffset(PointHashGridSearcher3::VT_POINTS, points); }
  void add_Buckets(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<PointHashGridSearcherBucket3>>> buckets) { fbb_.AddOffset(PointHashGridSearcher3::VT_BUCKETS, buckets); }
  PointHashGridSearcher3Builder(flatbuffers::FlatBufferBuilder &_fbb) : fbb_(_fbb) { start_ = fbb_.StartTable(); }
  PointHashGridSearcher3Builder &operator=(const PointHashGridSearcher3Builder &);
  flatbuffers::Offset<PointHashGridSearcher3> Finish() {
    auto o = flatbuffers::Offset<PointHashGridSearcher3>(fbb_.EndTable(start_, 4));
    return o;
  }
};

inline flatbuffers::Offset<PointHashGridSearcher3> CreatePointHashGridSearcher3(flatbuffers::FlatBufferBuilder &_fbb,
    double gridSpacing = 0.0,
    const CubbyFlow::fbs::Size3 *resolution = 0,
    flatbuffers::Offset<flatbuffers::Vector<const CubbyFlow::fbs::Vector3D *>> points = 0,
    flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<PointHashGridSearcherBucket3>>> buckets = 0) {
  PointHashGridSearcher3Builder builder_(_fbb);
  builder_.add_GridSpacing(gridSpacing);
  builder_.add_Buckets(buckets);
  builder_.add_Points(points);
  builder_.add_Resolution(resolution);
  return builder_.Finish();
}

inline flatbuffers::Offset<PointHashGridSearcher3> CreatePointHashGridSearcher3Direct(flatbuffers::FlatBufferBuilder &_fbb,
    double gridSpacing = 0.0,
    const CubbyFlow::fbs::Size3 *resolution = 0,
    const std::vector<const CubbyFlow::fbs::Vector3D *> *points = nullptr,
    const std::vector<flatbuffers::Offset<PointHashGridSearcherBucket3>> *buckets = nullptr) {
  return CreatePointHashGridSearcher3(_fbb, gridSpacing, resolution, points ? _fbb.CreateVector<const CubbyFlow::fbs::Vector3D *>(*points) : 0, buckets ? _fbb.CreateVector<flatbuffers::Offset<PointHashGridSearcherBucket3>>(*buckets) : 0);
}

inline const CubbyFlow::fbs::PointHashGridSearcher3 *GetPointHashGridSearcher3(const void *buf) { return flatbuffers::GetRoot<CubbyFlow::fbs::PointHashGridSearcher3>(buf); }

inline bool VerifyPointHashGridSearcher3Buffer(flatbuffers::Verifier &verifier) { return verifier.VerifyBuffer<CubbyFlow::fbs::PointHashGridSearcher3>(nullptr); }

inline void FinishPointHashGridSearcher3Buffer(flatbuffers::FlatBufferBuilder &fbb, flatbuffers::Offset<CubbyFlow::fbs::PointHashGridSearcher3> root) { fbb.Finish(root); }

}  // namespace fbs
}  // namespace CubbyFlow

#endif  // FLATBUFFERS_GENERATED_POINTHASHGRIDSEARCHER3_CUBBYFLOW_FBS_H_
