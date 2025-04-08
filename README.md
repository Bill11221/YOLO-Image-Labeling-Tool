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


**###Future improvement directions**

Responsive and adaptive function upgrade
Image zoom and pan: At present, it is possible to consider expanding the image editing area and using QGraphicsView to implement more interactive operations, such as fine zooming, rotation, and panning, so that users can more easily view details and the entire image when annotating.

Responsive layout: Further optimize the adaptation of windows and controls, especially the display effect and operation experience under high resolution and touch devices (such as tablets).

Multi-category annotation and data management

Category management: Add a category selection drop-down menu and shortcut keys to support simultaneous annotation of different categories, and reflect category information in the annotation results in real time.

Configuration and database: Design a configuration file (such as JSON) or use SQLite database to manage configuration information such as categories, colors, shortcut keys, etc., to facilitate user customization and expansion.

Data statistics and browsing: Develop a statistical module to display and statistically analyze the number of annotations, category distribution, annotation quality, etc., to provide a reference for subsequent data cleaning and model training.

Annotation editing and undo-redo functions

Annotation editing: Add editing functions for selecting, resizing and positioning the drawn annotation box. QGraphicsItem can be used to drag, zoom and interact with the annotation box.

Undo/redo: Combine Qt's QUndoStack to implement undo and redo functions, so that users can quickly restore the previous state when they make mistakes, improving annotation efficiency.

Format export and data compatibility

On the basis of the current support for YOLO format, other popular annotation formats can be added, such as Pascal VOC (XML) or COCO (JSON), which can meet the needs of different deep learning platforms.

At the same time, batch processing and automatic conversion functions are supported, which can organize a batch of annotation files into a data set according to the specified format for subsequent use.

Interface and animation effect upgrade

Further use of QSS and tools such as QPropertyAnimation and QStateMachine to enhance interface animation effects, such as button hover animation, operation feedback animation, etc., to make the user experience smoother and more modern.

If project requirements allow, you can also consider introducing some Qt Quick/QML components to achieve more complex and dynamic interface effects and interaction logic.

Continuous integration and automated testing

Combined with CI tools (such as GitHub Actions), automatically perform compilation, unit testing, and packaging to ensure that each submission is automatically checked to reduce problems in the released version.

Develop unit tests and UI tests, and use the Qt Test framework to test the main functions to ensure that the improved version is more stable and efficient.

