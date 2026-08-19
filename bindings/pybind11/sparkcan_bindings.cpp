// bindings/pybind11/sparkcan_bindings.cpp
#include <pybind11/pybind11.h>
#include <array>
#include <string>
#include <cstdint>

#include "SparkBase.hpp"
#include "SparkFlex.hpp"
#include "SparkMax.hpp"

namespace py = pybind11;
using guard = py::call_guard<py::gil_scoped_release>;

PYBIND11_MODULE(_sparkcan, m) {
    m.doc() = "Python bindings for SparkCAN motor control and configuration";

    py::enum_<CtrlType>(m, "CtrlType")
        .value("kDutyCycle", CtrlType::kDutyCycle)
        .value("kVelocity", CtrlType::kVelocity)
        .value("kVoltage", CtrlType::kVoltage)
        .value("kPosition", CtrlType::kPosition)
        .value("kSmartMotion", CtrlType::kSmartMotion)
        .value("kSmartVelocity", CtrlType::kSmartVelocity)
        .value("kMaxMotionPosition", CtrlType::kMaxMotionPosition)
        .value("kMaxMotionVelocity", CtrlType::kMaxMotionVelocity)
        .export_values();

    py::enum_<IdleMode>(m, "IdleMode")
        .value("kCoast", IdleMode::kCoast)
        .value("kBrake", IdleMode::kBrake)
        .export_values();

    // Bind just the requested SparkBase APIs so SparkFlex/Max inherit them.
    py::class_<SparkBase>(m, "SparkBase")
        .def("SetPosition", &SparkBase::SetPosition, py::arg("position"), guard())
        .def("SetVelocity", &SparkBase::SetVelocity, py::arg("velocity"), guard())
        .def("GetAbsoluteEncoderPosition", &SparkBase::GetAbsoluteEncoderPosition, guard())
        .def("GetPosition", &SparkBase::GetPosition, guard())
        .def("GetVelocity", &SparkBase::GetVelocity, guard())
        .def("SetIdleMode", &SparkBase::SetIdleMode, py::arg("mode"), guard())
        .def("SetCtrlType", &SparkBase::SetCtrlType, py::arg("type"), guard())
        .def("SetPositionConversionFactor", &SparkBase::SetPositionConversionFactor, py::arg("factor"), guard())
        .def("SetVelocityConversionFactor", &SparkBase::SetVelocityConversionFactor, py::arg("factor"), guard())
        .def("SetClosedLoopRampRate", &SparkBase::SetClosedLoopRampRate, py::arg("ramp_rate"), guard())
        .def("SetP", &SparkBase::SetP, py::arg("slot"), py::arg("p"), guard())
        .def("SetI", &SparkBase::SetI, py::arg("slot"), py::arg("i"), guard())
        .def("SetD", &SparkBase::SetD, py::arg("slot"), py::arg("d"), guard())
        .def("SetF", &SparkBase::SetF, py::arg("slot"), py::arg("f"), guard())
        .def("SetVelocityFF", &SparkBase::SetF, py::arg("slot"), py::arg("ff"), guard())
        .def("SetIZone", &SparkBase::SetIZone, py::arg("slot"), py::arg("i_zone"), guard())
        .def("SetDFilter", &SparkBase::SetDFilter, py::arg("slot"), py::arg("d_filter"), guard())
        .def("SetOutputMin", &SparkBase::SetOutputMin, py::arg("slot"), py::arg("minimum"), guard())
        .def("SetOutputMax", &SparkBase::SetOutputMax, py::arg("slot"), py::arg("maximum"), guard())
        .def("GetIdleModeRaw", &SparkBase::GetIdleModeRaw, guard())
        .def("GetCtrlType", &SparkBase::GetCtrlType, guard())
        .def("GetPositionConversionFactor", &SparkBase::GetPositionConversionFactor, guard())
        .def("GetVelocityConversionFactor", &SparkBase::GetVelocityConversionFactor, guard())
        .def("GetClosedLoopRampRate", &SparkBase::GetClosedLoopRampRate, guard())
        .def("GetP", &SparkBase::GetP, py::arg("slot"), guard())
        .def("GetI", &SparkBase::GetI, py::arg("slot"), guard())
        .def("GetD", &SparkBase::GetD, py::arg("slot"), guard())
        .def("GetF", &SparkBase::GetF, py::arg("slot"), guard())
        .def("GetVelocityFF", &SparkBase::GetF, py::arg("slot"), guard())
        .def("GetIZone", &SparkBase::GetIZone, py::arg("slot"), guard())
        .def("GetDFilter", &SparkBase::GetDFilter, py::arg("slot"), guard())
        .def("GetOutputMin", &SparkBase::GetOutputMin, py::arg("slot"), guard())
        .def("GetOutputMax", &SparkBase::GetOutputMax, py::arg("slot"), guard())
        .def("ReadPIDSlot", [](SparkBase &motor, uint8_t slot) {
            std::array<float, 8> values{};
            {
                py::gil_scoped_release release;
                values = {
                    motor.GetP(slot), motor.GetI(slot), motor.GetD(slot), motor.GetF(slot),
                    motor.GetIZone(slot), motor.GetDFilter(slot),
                    motor.GetOutputMin(slot), motor.GetOutputMax(slot)};
            }
            py::dict result;
            result["p"] = values[0];
            result["i"] = values[1];
            result["d"] = values[2];
            result["velocity_ff"] = values[3];
            result["i_zone"] = values[4];
            result["d_filter"] = values[5];
            result["output_min"] = values[6];
            result["output_max"] = values[7];
            return result;
        }, py::arg("slot"))
        .def("SetPeriodicStatus2Period", &SparkBase::SetPeriodicStatus2Period, py::arg("period"), guard())
        .def("Heartbeat", &SparkBase::Heartbeat, guard())
        .def("heartbeat", &SparkBase::Heartbeat, guard());  // alias

    // Concrete devices (inherit SparkBase methods)
    py::class_<SparkFlex, SparkBase>(m, "SparkFlex")
        .def(py::init<const std::string&, uint8_t>(),
             py::arg("interface"), py::arg("device_id"));

    py::class_<SparkMax, SparkBase>(m, "SparkMax")
        .def(py::init<const std::string&, uint8_t>(),
             py::arg("interface"), py::arg("device_id"));
}
