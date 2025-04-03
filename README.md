# LabExe: Advanced Laboratory Control Framework

## Table of Contents
- [Overview](#overview)
- [Demonstrations](#demonstrations)
- [Key Features](#key-features)
- [Technical Achievement](#technical-achievement)
- [Plugin System](#plugin-system)
  - [Available Plugins](#available-plugins)
- [Technologies](#technologies)
- [Project Leadership](#project-leadership)
- [License](#license)

## Overview
LabExe is a comprehensive software framework designed for controlling complex scientific apparatus in quantum physics laboratories. Developed at Columbia University, this modular C++ solution provides researchers with powerful tools for experiment automation, data acquisition, and hardware control.

## Demonstrations
[![LabExe demo](https://j.gifs.com/BLYJ8n.gif)](http://www.youtube.com/watch?v=hch3cZZywzE "LabExe demo")
[![LabExe demo](https://j.gifs.com/kZ0kgN.gif)](http://www.youtube.com/watch?v=hch3cZZywzE "LabExe demo")
[![LabExe demo](https://j.gifs.com/Q08lm0.gif)](http://www.youtube.com/watch?v=hch3cZZywzE "LabExe demo")
[![Evolutionary Optimization demo](https://j.gifs.com/l59lkg.gif)](https://vimeo.com/31039111 "Evolutionary Optimization demo")
[![Quantum physics experiment](https://j.gifs.com/MQVl7B.gif)](https://www.youtube.com/watch?v=8w8fkU-Klpk "Quantum physics experiment")

## Key Features
- **Modular Architecture**: Extensible design with plugin support for hardware integration
- **Real-time Imaging**: Advanced camera control and image processing capabilities
- **Experimental Sequencing**: Precise timing control for multi-step experimental protocols
- **Optimization Algorithms**: Evolutionary and gradient-based parameter optimization
- **Persistent Data Storage**: Comprehensive logging and experimental parameter tracking
- **Cross-platform Support**: Developed using Qt framework for multi-OS compatibility
- **Plugin Ecosystem**: Extensible plugin architecture allowing researchers to develop custom solutions

## Technical Achievement
This framework represents a significant advancement in laboratory software engineering, breaking the traditional 2-year rewrite cycle common in academic research environments. Through thoughtful architecture and software design principles, LabExe has maintained continuous operation at Columbia University's quantum physics laboratories for multiple research generations.

## Plugin System
LabExe features a robust plugin architecture that enables researchers to extend functionality without modifying the core codebase. This empowers laboratory teams to:
- Develop custom hardware interfaces
- Implement specialized analysis algorithms
- Create experiment-specific control modules
- Share solutions across research groups

The plugin repository is maintained at [GitHub: LabExe-UserPlugins](https://github.com/GaelReinaudi/LabExe-UserPlugins), containing a growing collection of community-developed extensions that demonstrate the platform's versatility.

### Available Plugins

#### Hardware Plugins
- **Camera Integration**
  - UEye Camera Plugin
- **Signal Generation**
  - Novatech 409B Plugin
  - Novatech 425A Plugin
  - Menlo DDS120 Plugin
- **Measurement Devices**
  - Thorlabs TSP01 Plugin
  - HighFinesse Wavelength Meter Plugin
- **Data Acquisition**
  - PXI System Plugin
- **Control Systems**
  - Phidgets Plugin

#### Software Plugins
- **Data Processing**
  - Linear Combination Plugin
  - Numerical Expression Plugin
  - Random Number Generator Plugin
  - Low Pass Filter Plugin
  - Rolling Stats Plugin
- **Image Analysis**
  - Image Stacker Plugin
  - Image Absorption Plugin
  - Better Absorption Image Plugin
  - Image Saver Plugin
  - Fitter 2D Gaussian Plugin
  - Image Pixel Adder Plugin
- **Visualization**
  - Plot XY Plugin
  - Plot 1D Plugin
  - Plot Y Plugin
  - Param Plotter Plugin
- **Experiment Control**
  - Timer Plugin
  - Stepper Plugin
  - Memory Switcher Plugin
  - Locking Slapper Plugin
  - SRS Controller Plugin
- **Utilities**
  - Plain Text Logger Plugin
  - Lab Notes Logger Plugin
  - Param Supplier Plugin
  - Text Loader Plugin
  - Serial Com Port Plugin
  - Email Messager Plugin
  - Google Chat Plugin
  - Internet Browser Plugin

## Technologies
- **C++**: Core application development
- **Qt Framework**: UI development and cross-platform support
- **Object-Oriented Design**: Inheritance hierarchies for device abstraction
- **Design Patterns**: Factory, Singleton, and Strategy patterns for extensibility
- **Hardware Integration**: National Instruments, cameras, laser systems, and custom hardware

## Project Leadership
This project was conceived, architected, and implemented by Gael Reinaudi, demonstrating expertise in:
- Software architecture for complex systems
- Hardware-software integration
- Real-time control systems
- Technical team leadership
- Long-term maintainable code design

## License
See the [license.md](license.md) file for details.


