# CarND-P09-PidController

CarND-P09-PidController implements a PID controller that controls
the vehicle in the simulator using the provided Cross Tracking Error (CTE).

## File Structure
### C++ Source Files - /src
- **[main.cpp](src/main.cpp)** - runs WebSocket server to interact with 
    the Term 2 simulator.  It receives CTE and speed inputs from the
    simulator, and it returns the computed steering and throttle values.
- **[PID.cpp](src/PID.cpp)** - implements PID controller.  It tracks
    and updates each of P, I, and D factors and associated coefficients. 
- **[ParamOptimizer.cpp](src/ParamOptimizer.cpp)** - implements
    parameter optimization using _Twiddle_ algorithm.
### Project Report
- **[writeup_report.md](writeup_report.md)** - a report discussing the
    controller and optimizer implementation, and the project reflections.
- **[pid_control_sample_video.mp4](results/pid_control_sample_video.mp4)** -
    a sample video recording of the vehicle traveling under the implemented
    PID controller.
### Other Support Files
- **[CMakeLists.txt](CMakeLists.txt)** - CMake file

## Getting Started
### [Download ZIP](https://github.com/gabeoh/CarND-P09-PidController/archive/master.zip) or Git Clone
```
git clone https://github.com/gabeoh/CarND-P09-PidController.git
```

### Install uWebSockets
Run the installation script from the project repository. 
- [uWebSockets](https://github.com/uNetworking/uWebSockets)
#### Linux Installation
```
./install-ubuntu.sh
```
#### Mac Installation
```
./install-mac.sh
```

### Download Simulator
- [Udacity Self-Driving Car - Term 2 Simulator](https://github.com/udacity/self-driving-car-sim/releases/)

### Dependencies
* cmake >= 3.5
  * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1 (Linux, Mac), 3.81 (Windows)
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* gcc/g++ >= 5.4
  * Linux: gcc / g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools](https://developer.apple.com/xcode/features/)
  * Windows: recommend using [MinGW](http://www.mingw.org/)

### Build and Run
1. Make a build directory: `mkdir build && cd build`
1. Generate Makefile: `cmake ..`
1. Compile: `make` 
1. Run it: `./pid`

## License
Licensed under [MIT](LICENSE) License.
