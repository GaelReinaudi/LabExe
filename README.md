# LabExe: Advanced Laboratory Control Framework

## Table of Contents
- [Overview](#overview)
- [Demonstrations](#demonstrations)
- [Core Architecture](#core-architecture)
- [Key Features](#key-features)
- [Project Structure](#project-structure)
- [Plugin System](#plugin-system)
  - [Available Plugins](#available-plugins)
  - [Creating Plugins](#creating-plugins)
- [Technologies & Dependencies](#technologies--dependencies)
- [Building & Installation](#building--installation)
- [Technical Achievement](#technical-achievement)
- [Project Leadership](#project-leadership)
- [License](#license)

## Overview

LabExe is a comprehensive, production-ready software framework designed for controlling complex scientific apparatus in quantum physics laboratories. Developed at Columbia University, this modular C++ solution provides researchers with powerful tools for experiment automation, data acquisition, hardware control, and real-time optimization.

**Key Philosophy**: Breaking the traditional 2-year rewrite cycle in academic laboratories through robust architecture, extensibility, and user-driven plugin development.

## Demonstrations

[![LabExe demo](https://j.gifs.com/BLYJ8n.gif)](http://www.youtube.com/watch?v=hch3cZZywzE "LabExe demo")
[![LabExe demo](https://j.gifs.com/kZ0kgN.gif)](http://www.youtube.com/watch?v=hch3cZZywzE "LabExe demo")
[![LabExe demo](https://j.gifs.com/Q08lm0.gif)](http://www.youtube.com/watch?v=hch3cZZywzE "LabExe demo")
[![Evolutionary Optimization demo](https://j.gifs.com/l59lkg.gif)](https://vimeo.com/31039111 "Evolutionary Optimization demo")
[![Quantum physics experiment](https://j.gifs.com/MQVl7B.gif)](https://www.youtube.com/watch?v=8w8fkU-Klpk "Quantum physics experiment")

## Core Architecture

LabExe is built around three fundamental concepts:

### 1. **Control Panel** (`GLabControlPanel`)
The main application window that serves as the central hub for:
- Managing multiple workbenches
- Providing device shelves (hardware and software)
- Loading and managing plugins
- Coordinating settings persistence
- Auto-update checking (via WinSparkle)

### 2. **Workbenches** (`GWorkBench`)
Customizable virtual workspaces where users compose experiments by:
- Dragging and dropping devices from shelves
- Connecting device parameters via drag-and-drop
- Creating dockable widget arrangements
- Supporting specialized workbench types:
  - **`GImagingBench`**: Image acquisition and processing workspace
  - **`GSequencingBench`**: Experimental timing sequence editor
  - **`GOptimizingBench`**: Parameter optimization workspace

### 3. **Devices** (`GDevice`)
Abstract representation of hardware and software components:
- **Hard Devices** (`GHardDevice`): Physical hardware interfaces
  - Cameras (`GCamera`)
  - Data acquisition systems
  - Signal generators
  - Measurement instruments
- **Prog Devices** (`GProgDevice`): Software processing units
  - Data filters and transformers
  - Plotters and visualizers
  - Mathematical operators
  - Loggers and recorders

Each device exposes **Parameters** (`GParam`) that can be:
- Serialized/deserialized automatically
- Connected to other devices via drag-and-drop
- Monitored and logged
- Optimized using built-in algorithms

## Key Features

### Experimental Control
- **Precise Timing Sequencer**: Multi-channel experimental sequence editor with graphical timeline
- **Real-time Imaging**: Advanced camera control with live image processing pipelines
- **Hardware Abstraction**: Unified interface for diverse laboratory equipment
- **Event Management**: Synchronization events for coordinating complex experimental protocols

### Data Processing & Analysis
- **Image Processing**: Real-time image acquisition, AOI selection, line profiling, 2D Gaussian fitting
- **Mathematical Expression Engine**: Runtime evaluation of user-defined formulas using ExprTK parser
- **Statistical Analysis**: Rolling statistics, data filtering, and signal processing
- **Drag-and-Drop Data Flow**: Visual connection of parameters between devices

### Optimization Capabilities
- **Evolutionary Algorithms**: Genetic optimization using EO (Evolving Objects) library
- **Gradient-based Methods**: NLopt integration for local optimization
- **Multi-parameter Optimization**: Simultaneous tuning of experimental parameters
- **Real-time Feedback**: Live visualization of optimization progress

### Persistence & Logging
- **Automatic Settings Management**: Device states and parameters saved/restored seamlessly
- **Experiment Logging**: Comprehensive recording of experimental parameters and results
- **Undo/Redo Framework**: Qt-based command pattern for operation history
- **Screenshot Export**: Automatic workspace documentation

### Extensibility
- **Plugin Architecture**: Hot-loadable plugins without core modification
- **Plugin Wizard**: Automated plugin project scaffolding tool
- **Factory Pattern**: Dynamic device creation and registration
- **Clean API**: Well-documented interfaces for third-party development

## Project Structure

```
LabExe/
├── src/                          # Core framework source
│   ├── Device/                   # Device base classes and plugin system
│   ├── Param/                    # Parameter system (Int, Double, Bool, String)
│   ├── TheLab/                   # Control panel and workbench infrastructure
│   ├── ToolBox/                  # Utilities (serialization, factories, objects)
│   ├── ModelView/                # Qt Model-View architecture implementations
│   ├── Sequencer/                # Legacy sequencer components
│   ├── Optimizer/                # Optimization algorithms (NOMAD)
│   ├── LabExeSequencer/          # Modern sequencer module (channels, events)
│   ├── LabExeImaging/            # Camera and image processing module
│   ├── LabExeOptimizing/         # Optimization module (EO, NLopt)
│   ├── Curve/                    # Curve data structures
│   ├── Instruction/              # Instruction system for sequencer
│   ├── Script/                   # Scripting support
│   ├── Logger/                   # Logging UI components
│   └── labexe.pro                # Qt project file (builds as library)
│
├── Deployement/                  # Deployment resources
│   ├── WinLabexe/               # Windows executable wrapper
│   ├── UserPluginWizard/        # Plugin creation wizard application
│   ├── Installers/              # Windows installer projects (MSI)
│   └── doc/                     # Doxygen documentation
│
├── include/                      # Third-party library headers
│   ├── eo/                      # Evolving Objects (evolutionary algorithms)
│   ├── loki/                    # Loki library (design patterns)
│   ├── nlopt/                   # NLopt optimization library
│   ├── qtcolorpicker.h          # Qt Solutions color picker
│   └── WinSparkle/              # Auto-update framework (Windows)
│
├── lib/                          # Pre-compiled third-party libraries
│   ├── eo/                      # EO static libraries
│   ├── nlopt/                   # NLopt static libraries
│   └── GSmtp.lib, *.dll         # SSL and SMTP libraries
│
├── Manual/                       # End-user documentation
├── examples-resources/           # Tutorial resources
├── common.pri                    # Shared qmake configuration
└── README.md
```

### Build System
- **Qt Project Files** (`.pro`): qmake-based build system
- **Visual Studio Projects** (`.vcxproj`): Windows IDE integration
- **Modular Compilation**: Core library + separate modules (Imaging, Sequencing, Optimizing)
- **Unified Build Directory**: `build/debug/` and `build/release/` structure

## Plugin System

LabExe's plugin architecture enables seamless extension without modifying core code.

### Plugin Types
1. **Hard Device Plugins**: Interface with physical laboratory equipment
2. **Prog Device Plugins**: Software processing and analysis modules

### Plugin Registration Macros
```cpp
// For hardware devices
G_REGISTER_PLUGIN_HARD_DEVICE(ClassName, UniqueID)

// For software devices (single)
G_REGISTER_PLUGIN_PROG_DEVICE(ClassName)

// For multiple devices in one plugin
G_REGISTER_2_PLUGIN_PROG_DEVICE(Class1, Class2)
G_REGISTER_3_PLUGIN_PROG_DEVICE(Class1, Class2, Class3)
```

### Available Plugins

The plugin repository is maintained at [GitHub: LabExe-UserPlugins](https://github.com/GaelReinaudi/LabExe-UserPlugins).

#### Hardware Plugins
- **Camera Integration**
  - UEye Camera Plugin (IDS Imaging)
- **Signal Generation**
  - Novatech 409B Plugin (RF signal generator)
  - Novatech 425A Plugin (RF signal generator)
  - Menlo DDS120 Plugin (Direct Digital Synthesizer)
- **Measurement Devices**
  - Thorlabs TSP01 Plugin (Temperature sensor)
  - HighFinesse Wavelength Meter Plugin (WS6/WS7 series)
- **Data Acquisition**
  - PXI System Plugin (National Instruments)
- **Control Systems**
  - Phidgets Plugin (USB sensors and actuators)

#### Software Plugins
- **Data Processing**
  - Linear Combination Plugin
  - Numerical Expression Plugin (math formula evaluator)
  - Random Number Generator Plugin
  - Low Pass Filter Plugin
  - Rolling Stats Plugin
- **Image Analysis**
  - Image Stacker Plugin (averaging/summing)
  - Image Absorption Plugin (atomic absorption imaging)
  - Better Absorption Image Plugin (improved algorithm)
  - Image Saver Plugin (automated capture)
  - Fitter 2D Gaussian Plugin (beam profiling)
  - Image Pixel Adder Plugin
- **Visualization**
  - Plot XY Plugin (scatter plots)
  - Plot 1D Plugin (line plots)
  - Plot Y Plugin (time series)
  - Param Plotter Plugin (parameter monitoring)
- **Experiment Control**
  - Timer Plugin (experiment timing)
  - Stepper Plugin (parameter scanning)
  - Memory Switcher Plugin (state switching)
  - Locking Slapper Plugin (feedback control)
  - SRS Controller Plugin (Stanford Research Systems)
- **Utilities**
  - Plain Text Logger Plugin (CSV export)
  - Lab Notes Logger Plugin (experiment journal)
  - Param Supplier Plugin (value injection)
  - Text Loader Plugin (configuration import)
  - Serial Com Port Plugin (RS-232 communication)
  - Email Messager Plugin (experiment notifications)
  - Google Chat Plugin (team alerts)
  - Internet Browser Plugin (web integration)

### Creating Plugins

#### Using the Plugin Wizard
1. Launch `UserPluginWizard` (included in deployment)
2. Select a template plugin to base your work on
3. Enter your device name (e.g., "MyAwesomeCamera")
4. Wizard generates complete project with:
   - Base class implementation
   - Qt project files
   - Visual Studio integration
   - Example parameters and GUI

#### Manual Plugin Development
Inherit from `GDevice` or `GProgDevice` and implement:
```cpp
class MyDevice : public GProgDevice {
    Q_OBJECT
public:
    MyDevice(QObject *parent, QString uniqueID = "");
    
protected:
    // Define your device's GUI
    void PopulateDeviceWidget(GDeviceWidget* widget) override;
    
    // Optional: custom settings persistence
    void PopulateSettings(QSettings& settings) override;
    void InterpretSettings(QSettings& settings) override;
    
private:
    // Your parameters (auto-serialized)
    GParamDouble m_Frequency;
    GParamInt m_SampleCount;
    GParamBool m_EnableFeature;
};
```

## Technologies & Dependencies

### Core Technologies
- **C++17**: Modern C++ with STL
- **Qt 5/6**: Cross-platform application framework
  - QtCore, QtGui, QtWidgets
  - QtConcurrent (parallel processing)
  - Qt5Compat (for Qt6 migration)
  - QML (for modern UI components)

### Design Patterns & Libraries
- **Loki Library**: Modern C++ Design patterns implementation
  - Factory pattern for device/workbench creation
  - Singleton pattern for managers
  - Type-safe functional programming utilities
- **EO (Evolving Objects)**: Evolutionary computation framework
  - Genetic algorithms
  - Evolution strategies
  - Multi-objective optimization
- **NLopt**: Nonlinear optimization library
  - Local and global optimizers
  - Derivative-free methods
  - Constrained optimization

### Additional Libraries
- **ExprTK**: Mathematical expression parsing and evaluation
- **AlgLib**: Numerical analysis and data processing
- **spdlog**: Fast C++ logging library
- **QtSolutions ColorPicker**: Enhanced color selection widgets
- **OpenSSL**: Secure communication (email notifications)
- **WinSparkle**: Application auto-update framework (Windows)

### Build Tools
- **qmake**: Qt's build system generator
- **Visual Studio 2008+**: Windows development (legacy support)
- **Modern compilers**: GCC/Clang (Linux), MSVC (Windows)

## Building & Installation

### Prerequisites
- Qt 5.15+ or Qt 6.x with QtWidgets module
- C++17 compatible compiler
- Dependencies (specified in `common.pri`):
  - spdlog (logging)
  - ExprTK (expression parsing)
  - AlgLib (numerical algorithms)

### Build Instructions

#### Using Qt Creator
1. Open `src/labexe.pro`
2. Configure build directory structure (see `common.pri`)
3. Build the core library
4. Build modules: LabExeImaging, LabExeSequencer, LabExeOptimizing
5. Build deployment: `Deployement/WinLabexe/WinLabexe.pro`

#### Using qmake (Command Line)
```bash
cd src
qmake labexe.pro
make

# Build modules
cd LabExeImaging && qmake && make
cd ../LabExeSequencer && qmake && make
cd ../LabExeOptimizing && qmake && make
```

### Directory Structure After Build
```
build/
├── debug/    or    release/
    ├── bin/                    # Executables
    │   ├── WinLabexe.exe      # Main application
    │   └── plugins/           # Plugin DLLs/SOs
    ├── lib/                    # Shared libraries
    └── generated/              # MOC, UI, RCC files
```

### Installing Plugins
1. Compile plugin projects (from LabExe-UserPlugins repository)
2. Copy resulting DLL/SO to `build/release/bin/plugins/<PluginName>/`
3. Restart LabExe (plugins auto-discovered on startup)

## Technical Achievement

This framework represents a **significant advancement in laboratory software engineering**, addressing critical challenges in academic research environments:

### Breaking the Rewrite Cycle
Traditional lab software typically:
- Gets rewritten every 2 years as students graduate
- Lacks extensibility for new equipment
- Becomes unmaintainable after original author departs
- Relies on fragile, undocumented code

LabExe solves this through:
- **Stable Core**: Well-architected framework with clear separation of concerns
- **Plugin Ecosystem**: Hardware changes don't require core modifications
- **Comprehensive Documentation**: Doxygen-generated API docs and tutorials
- **Industrial Patterns**: Factory, Singleton, MVC patterns ensure maintainability
- **Active Community**: Multiple research groups contribute and maintain plugins

### Real-World Impact
- **Multi-generational deployment**: Continuous operation at Columbia University across multiple PhD cycles
- **Cross-lab adoption**: Used in multiple quantum physics research groups
- **Production stability**: Handles mission-critical experimental data acquisition
- **Extensibility proven**: 30+ community-developed plugins demonstrate platform viability

### Software Engineering Excellence
- **Clean abstraction layers**: Hardware specifics isolated from core logic
- **Automatic persistence**: Settings and state management handled by framework
- **Type-safe parameter system**: Drag-and-drop with compile-time safety
- **Undo/redo support**: Professional-grade user experience
- **Real-time performance**: Optimized for high-speed data acquisition and imaging

## Project Leadership

This project was **conceived, architected, and implemented by Gael Reinaudi**, demonstrating expertise in:

- **Software Architecture**: Multi-year sustainable design for research environments
- **Hardware Integration**: Seamless abstraction of diverse laboratory equipment
- **Real-time Systems**: High-performance data acquisition and control
- **API Design**: Clean, extensible interfaces for third-party development
- **Technical Leadership**: Enabling cross-lab collaboration through plugin ecosystem
- **Research Software Engineering**: Bridging academic research and industrial software practices

## License

Copyright (c) Gael Reinaudi

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
