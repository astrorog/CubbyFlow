/*************************************************************************
> File Name: PhysicsAnimation.cpp
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: Physics animation functions for CubbyFlow Python API.
> Created Time: 2018/01/20
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <API/Python/Animation/PhysicsAnimation.h>
#include <Core/Animation/Animation.h>
#include <Core/Animation/PhysicsAnimation.h>

#include <pybind11/pybind11.h>

using namespace CubbyFlow;

class PyPhysicsAnimation : public PhysicsAnimation
{
public:
    using PhysicsAnimation::PhysicsAnimation;

    void OnAdvanceTimeStep(double timeIntervalInSeconds) override
    {
        PYBIND11_OVERLOAD_PURE(void, PhysicsAnimation, OnAdvanceTimeStep, timeIntervalInSeconds);
    }

    unsigned int NumberOfSubTimeSteps(double timeIntervalInSeconds) const override
    {
        PYBIND11_OVERLOAD(unsigned int, PhysicsAnimation, NumberOfSubTimeSteps, timeIntervalInSeconds);
    }

    void OnInitialize() override
    {
        PYBIND11_OVERLOAD(void, PhysicsAnimation, OnInitialize, );
    }
};

void AddPhysicsAnimation(pybind11::module& m)
{
    pybind11::class_<PhysicsAnimation, PyPhysicsAnimation, PhysicsAnimationPtr, Animation>(static_cast<pybind11::handle>(m), "PhysicsAnimation")
    .def(pybind11::init<>())
    .def_property("isUsingFixedSubTimeSteps",
        &PhysicsAnimation::IsUsingFixedSubTimeSteps,
        &PhysicsAnimation::SetIsUsingFixedSubTimeSteps)
    .def_property("numberOfFixedSubTimeSteps",
        &PhysicsAnimation::NumberOfFixedSubTimeSteps,
        &PhysicsAnimation::SetNumberOfFixedSubTimeSteps)
    .def("advanceSingleFrame",
        &PhysicsAnimation::AdvanceSingleFrame)
    .def_property("currentFrame",
        &PhysicsAnimation::CurrentFrame,
        &PhysicsAnimation::SetCurrentFrame)
    .def_property_readonly("currentTimeInSeconds",
        &PhysicsAnimation::CurrentTimeInSeconds);
}