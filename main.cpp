#include <QApplication>
#include <QMainWindow>
#include <QFileDialog>
#include <QLabel>
#include <QMouseEvent>
#include <QPainter>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDir>
#include <QMessageBox>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QUrl>
#include <QFile>
#include <QFileInfo>
#include <QGraphicsDropShadowEffect>
#include <QStyle>
#include <QMimeData>
#include <QStringList>
#include <vector>

// ===================== ImageLabel 类 =======================
class ImageLabel : public QLabel {
    Q_OBJECT
public:
    explicit ImageLabel(QWidget *parent = nullptr)
        : QLabel(parent), drawing(false) {
        setMouseTracking(true);
        setScaledContents(true);
    }

    void setImage(const QPixmap &pix) {
        originalPixmap = pix;
        setPixmap(originalPixmap);
        boxes.clear();
        update();
    }

    void clearBoxes() {
        boxes.clear();
        update();
    }

    std::vector<QRect> getBoxes() const {
        return boxes;
    }

signals:
    void boxDrawn();

protected:
    void mousePressEvent(QMouseEvent *event) override {
        if (event->button() == Qt::LeftButton) {
            drawing = true;
            startPoint = event->pos();
            currentRect = QRect(startPoint, QSize());
        }
        QLabel::mousePressEvent(event);
    }

    void mouseMoveEvent(QMouseEvent *event) override {
        if (drawing) {
            currentRect = QRect(startPoint, event->pos()).normalized();
            update();
        }
        QLabel::mouseMoveEvent(event);
    }

    void mouseReleaseEvent(QMouseEvent *event) override {
        if (drawing && event->button() == Qt::LeftButton) {
            drawing = false;
            currentRect = QRect(startPoint, event->pos()).normalized();
            boxes.push_back(currentRect);
            emit boxDrawn();
            update();
        }
        QLabel::mouseReleaseEvent(event);
    }

    void paintEvent(QPaintEvent *ev) override {
        QLabel::paintEvent(ev);
        QPainter painter(this);
        painter.setPen(QPen(Qt::green, 2));
        for (const QRect &rect : boxes) {
            painter.drawRect(rect);
        }
        if (drawing) {
            painter.setPen(QPen(Qt::red, 2));
            painter.drawRect(currentRect);
        }
    }

private:
    QPixmap originalPixmap;
    bool drawing;
    QPoint startPoint;
    QRect currentRect;
    std::vector<QRect> boxes;
};

// ===================== MainWindow 类 =======================
class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr)
        : QMainWindow(parent), currentIndex(0) {
        setWindowTitle("YOLO Annotator");
        setupStyle();
        setupUI();
        setAcceptDrops(true);
    }

protected:
    void dragEnterEvent(QDragEnterEvent *event) override {
        if (event->mimeData()->hasUrls())
            event->acceptProposedAction();
    }
    void dropEvent(QDropEvent *event) override {
        QList<QUrl> urls = event->mimeData()->urls();
        if (urls.isEmpty())
            return;
        imageFiles.clear();
        for (const QUrl &url : urls) {
            QString filePath = url.toLocalFile();
            if (isImageFile(filePath))
                imageFiles.append(filePath);
        }
        if (!imageFiles.isEmpty()) {
            currentIndex = 0;
            imageFolder = QFileInfo(imageFiles.first()).absolutePath();
            QDir dir(imageFolder);
            if (!dir.exists("label"))
                dir.mkdir("label");
            loadImage();
        }
    }

private slots:
    void selectFolder() {
        QString folder = QFileDialog::getExistingDirectory(this, "Select Image Folder");
        if (folder.isEmpty())
            return;
        imageFolder = folder;
        QDir dir(folder);
        QStringList filters;
        filters << "*.jpg" << "*.jpeg" << "*.png" << "*.bmp";
        imageFiles = dir.entryList(filters, QDir::Files);
        for (QString &f : imageFiles) {
            f = QDir(folder).filePath(f);
        }
        if (imageFiles.isEmpty()) {
            QMessageBox::warning(this, "Error", "No images found in the selected folder!");
            return;
        }
        currentIndex = 0;
        QDir d(folder);
        if (!d.exists("label"))
            d.mkdir("label");
        loadImage();
    }
    void resetAnnotation() {
        if (!imageFiles.isEmpty())
            imageLabel->clearBoxes();
    }
    void nextImage() {
        if (imageFiles.isEmpty())
            return;
        saveAnnotation();
        currentIndex++;
        if (currentIndex >= imageFiles.size()) {
            QMessageBox::information(this, "Info", "All images have been annotated!");
            return;
        }
        loadImage();
    }

private:
    void setupStyle() {
        QString style = R"(
            QMainWindow { background-color: #eaeaea; }
            QPushButton {
                background-color: #4CAF50;
                color: white;
                border: none;
                border-radius: 10px;
                padding: 8px 16px;
                font-size: 14px;
            }
            QPushButton:hover { background-color: #45a049; }
            QPushButton:pressed { background-color: #3d8b3d; }
            QLabel { font-size: 14px; }
        )";
        setStyleSheet(style);
    }
    void setupUI() {
        QWidget *centralWidget = new QWidget(this);
        QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
        mainLayout->setContentsMargins(10, 10, 10, 10);
        mainLayout->setSpacing(10);

        QHBoxLayout *buttonLayout = new QHBoxLayout();
        buttonLayout->setSpacing(10);
        QPushButton *btnSelectFolder = new QPushButton("Select Folder", this);
        QPushButton *btnReset = new QPushButton("Reset Annotation", this);
        QPushButton *btnNext = new QPushButton("Next Image", this);
        QPushButton *btnQuit = new QPushButton("Quit", this);
        buttonLayout->addWidget(btnSelectFolder);
        buttonLayout->addWidget(btnReset);
        buttonLayout->addWidget(btnNext);
        buttonLayout->addWidget(btnQuit);
        mainLayout->addLayout(buttonLayout);

        imageLabel = new ImageLabel(this);
        imageLabel->setAlignment(Qt::AlignCenter);
        imageLabel->setMinimumSize(400, 300);
        QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
        shadow->setBlurRadius(20);
        shadow->setOffset(5, 5);
        imageLabel->setGraphicsEffect(shadow);
        mainLayout->addWidget(imageLabel, 1);

        setCentralWidget(centralWidget);

        connect(btnSelectFolder, &QPushButton::clicked, this, &MainWindow::selectFolder);
        connect(btnReset, &QPushButton::clicked, this, &MainWindow::resetAnnotation);
        connect(btnNext, &QPushButton::clicked, this, &MainWindow::nextImage);
        connect(btnQuit, &QPushButton::clicked, this, &MainWindow::close);
    }
    bool isImageFile(const QString &fileName) {
        QString lower = fileName.toLower();
        return lower.endsWith(".jpg") || lower.endsWith(".jpeg")
            || lower.endsWith(".png") || lower.endsWith(".bmp");
    }
    void loadImage() {
        if (currentIndex < 0 || currentIndex >= imageFiles.size())
            return;
        QPixmap pix(imageFiles[currentIndex]);
        if (pix.isNull()) {
            QMessageBox::warning(this, "Error", "Failed to load image: " + imageFiles[currentIndex]);
            return;
        }
        imageLabel->setImage(pix);
        setWindowTitle(QString("YOLO Annotator - %1 (%2/%3)")
                           .arg(QFileInfo(imageFiles[currentIndex]).fileName())
                           .arg(currentIndex + 1)
                           .arg(imageFiles.size()));
    }
    void saveAnnotation() {
        if (imageFiles.isEmpty())
            return;
        QPixmap pix(imageFiles[currentIndex]);
        int imgWidth = pix.width();
        int imgHeight = pix.height();
        std::vector<QRect> boxes = imageLabel->getBoxes();
        QString annotation;
        for (const QRect &r : boxes) {
            float x_center = (r.x() + r.width() / 2.0f) / imgWidth;
            float y_center = (r.y() + r.height() / 2.0f) / imgHeight;
            float width = r.width() / static_cast<float>(imgWidth);
            float height = r.height() / static_cast<float>(imgHeight);
            annotation += QString("0 %1 %2 %3 %4\n")
                              .arg(x_center, 0, 'f', 6)
                              .arg(y_center, 0, 'f', 6)
                              .arg(width, 0, 'f', 6)
                              .arg(height, 0, 'f', 6);
        }
        QString imageFileName = QFileInfo(imageFiles[currentIndex]).fileName();
        QString annotationFile = QFileInfo(imageFiles[currentIndex]).absolutePath() +
                                 "/label/" +
                                 QFileInfo(imageFileName).completeBaseName() + ".txt";
        QFile file(annotationFile);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            file.write(annotation.toUtf8());
            file.close();
        } else {
            QMessageBox::warning(this, "Error", "Failed to save annotation to " + annotationFile);
        }
    }
    ImageLabel *imageLabel;
    QString imageFolder;
    QStringList imageFiles;
    int currentIndex;
};

#include "main.moc"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MainWindow w;
    w.resize(800, 600);
    w.show();
    return app.exec();
}
