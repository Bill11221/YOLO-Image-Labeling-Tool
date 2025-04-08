# YoloAnnotator

YoloAnnotator is a Qt-based image annotation tool, mainly used to generate annotation data in YOLO format.
The project has the following functions:
- **Image display and annotation**: Supports drawing rectangular annotation boxes on images and automatically saving annotation data (normalized to YOLO format).
- **Drag and drop and folder selection**: Supports loading images by dragging or selecting folders, and automatically generates a `label` folder in the directory where the image is located to save the annotation file.
- **Beautify the interface**: Use QSS style sheets and shadow effects to provide a simple and modern interface.
- **Cross-platform**: Developed based on Qt Widgets, it can be built into a Windows GUI application (also supports other platforms).

## Features

- Responsive layout to ensure beautiful display at different resolutions
- Support dragging files to the interface for annotation
- Automatically generate annotation files in YOLO format (`<class_id> <x_center> <y_center> <width> <height>`)
- Use QSS to beautify the interface and provide a smooth user experience

## Build and deploy

### Compile with Qt Creator

1. Open Qt Creator and import this project.
2. Make sure the configuration in the `.pro` file is correct:
```pro
QT += widgets
CONFIG += c++17 windows release
SOURCES += main.cpp
```
3. Click the "Build" button to compile. The generated executable file is usually in the release folder of the build directory (such as `release/YoloAnnotator.exe`).

### Build with command line

1. Open a command prompt or PowerShell in the project root directory.
2. Run the following command to generate Makefile:
```bash
qmake YoloAnnotator.pro
```
3. Run according to the compilation environment:
- Under MinGW environment:
```bash
mingw32-make
```
- Under MSVC environment:
```bash
nmake
```
4. After successful compilation, the generated exe file is usually located in the release folder.

### Deployment and operation

To ensure normal runtime, all Qt dependent libraries need to be deployed together. It is recommended to use the `windeployqt` tool that comes with Qt:
```bash
windeployqt YoloAnnotator.exe
