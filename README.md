To install python bindings in conda env:
```bash
git clone https://github.com/vedAnts256/sparkcan_pybindings.git
cd sparkcan_pybindings
conda activate your_env
pip install -U scikit-build-core cmake ninja pybind11
pip install .
```

This works with firmware 25.0.X

### Firmware 25 PID parameters

The Python bindings support acknowledged, non-persistent reads and writes for
all four closed-loop PID slots on firmware 25.0.x:

```python
from sparkcan_py import SparkMax

motor = SparkMax("can0", 1)

# Read all eight values from slot 0. Read failures raise an exception.
print(motor.ReadPIDSlot(0))

# Each write waits for and validates the controller acknowledgement.
motor.SetP(0, 0.2)
motor.SetI(0, 0.0)
motor.SetD(0, 0.1)
motor.SetVelocityFF(0, 0.0)  # SetF is retained as a compatibility alias.
motor.SetIZone(0, 0.0)
motor.SetDFilter(0, 0.0)
motor.SetOutputMin(0, -1.0)
motor.SetOutputMax(0, 1.0)
```

These setters update RAM only. They do not burn parameters to flash.

Original sparkcan library readme below. This is a modified fork of https://github.com/grayson-arendt/sparkcan.git

#### Disclaimer
This project is not affiliated with, endorsed by, or in any way connected to REV Robotics. All product names, logos, and brands are property of their respective owners. Any references to REV Robotics products, such as the SPARK MAX or SPARK Flex, are purely for identification purposes and do not imply any endorsement or sponsorship.

#### Setup
A USB to CAN adapter such as a [CANable](https://canable.io/) is required. This package has been tested with the [Spark MAX](https://www.revrobotics.com/rev-11-2158/) and [SPARK Flex](https://www.revrobotics.com/rev-11-2159/).

Make sure to use [REV Hardware Client](https://docs.revrobotics.com/rev-hardware-client/gs/install) to set the `device ID` for your controllers and modify the values to match them in the examples. Ensure the correct motor type is set to either `kBrushed` or `kBrushless` depending on your motor, as it may damage the motor if set to the wrong type. This fork targets **firmware 25.0.x**; PID parameter transport has been hardware-tested with firmware **25.0.4**.


#### Installation
```bash
sudo apt update
sudo apt upgrade
sudo apt install git cmake

sudo add-apt-repository ppa:graysonarendt/sparkcan
sudo apt update
sudo apt install sparkcan

git clone https://github.com/grayson-arendt/sparkcan-examples.git
cd sparkcan-examples/

chmod +x canable_start.sh
./canable_start.sh

mkdir build
cd build
cmake ..
make
```

#### Running an Example

```bash
./example_control # OR ./example_status OR ./example_pid
```
