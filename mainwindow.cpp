#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFileDialog>
#include <QPixmap>
#include <QMessageBox>
#include <QImage>
#include <opencv2/opencv.hpp>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include "videoThread.h"
#include <QCamera>


int DEFAULT_TIMEOUT = 2000;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->statusbar->showMessage("Ready", DEFAULT_TIMEOUT);
    setupActions();
    setupConnections();
    setupShortCuts();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupActions()
{
    // Image Actiions
    connect(ui->actionOpen_Image, &QAction::triggered, this, &MainWindow::onOpenImagePressed);
    connect(ui->actionSave_Image, &QAction::triggered, this, &MainWindow::saveImage);
    connect(ui->actionSave_Image_As, &QAction::triggered, this, &MainWindow::saveImageAs);
    connect(ui->actionClose_Image, &QAction::triggered, this, &MainWindow::closeImage);

    // Image Edit Actions
    connect(ui->actionZoom_In, &QAction::triggered, this, &MainWindow::zoomIn);
    connect(ui->actionZoom_Out, &QAction::triggered, this, &MainWindow::zoomOut);
    connect(ui->actionFit_to_Window, &QAction::triggered, this, &MainWindow::fitToWindow);
    connect(ui->actionNormal_Size, &QAction::triggered, this, &MainWindow::normalSize);
    connect(ui->actionRotate_Left, &QAction::triggered, this, &MainWindow::rotateLeft);
    connect(ui->actionRotate_Right, &QAction::triggered, this, &MainWindow::rotateRight);
    connect(ui->actionFlip_Horizontal, &QAction::triggered, this, &MainWindow::flipHorizontal);
    connect(ui->actionFlip_Vertical, &QAction::triggered, this, &MainWindow::flipVertical);

    // Image Processing Actions
    connect(ui->actionBlur, &QAction::triggered, this, &MainWindow::blurImage);
    connect(ui->actionCartoonify, &QAction::triggered, this, &MainWindow::cartoonifyImage);

    // Video Actions
    connect(ui->actionOpen_Camera, &QAction::triggered, this, &MainWindow::openCamera);
    connect(ui->actionClose_Camera, &QAction::triggered, this, &MainWindow::closeCamera);
    connect(ui->actionExit, SIGNAL(triggered()), QApplication::instance(), SLOT(quit()));
    connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::showAbout);
    connect(ui->actionShow_Cameras, &QAction::triggered, this, &MainWindow::showCameraInfo);
}

void MainWindow::setupConnections()
{
    connect(ui->openImage, &QPushButton::pressed, this, &MainWindow::onOpenImagePressed);
    connect(ui->blurHS, &QSlider::valueChanged, this, &MainWindow::onBlurHSSliderMoved);
    connect(ui->cartoonifyHS, &QSlider::valueChanged, this, &MainWindow::onCartoonifyHSSliderMoved);
    connect(ui->gaussianBlurHS, &QSlider::valueChanged, this, &MainWindow::onGaussianBlurHSSliderMoved);
    connect(ui->sharpenHS, &QSlider::valueChanged, this, &MainWindow::onSharpenHSSliderMoved);
    connect(ui->openCamera, &QPushButton::pressed, this, &MainWindow::onOpenCameraPressed);
    connect(ui->takePhoto, &QPushButton::pressed, this, &MainWindow::onTakePhotoPressed);
}



void MainWindow::showAbout()
{
    QMessageBox::about(this, tr("About Image Processing App"),
                       tr("This is a simple image processing app that allows you to open images, apply filters, and save the processed images. "
                          "You can also open a camera and take photos. "
                          "This app is created using Qt and OpenCV."));
}

void MainWindow::setupShortCuts()
{
    // Setup Shortcuts
    ui->actionOpen_Image->setShortcut(QKeySequence::Open);
    ui->actionSave_Image->setShortcut(QKeySequence::Save);
    ui->actionSave_Image_As->setShortcut(QKeySequence::SaveAs);
    ui->actionClose_Image->setShortcut(QKeySequence::Close);
    ui->actionExit->setShortcut(QKeySequence::Quit);
    ui->actionZoom_In->setShortcut(QKeySequence::ZoomIn);
    ui->actionZoom_Out->setShortcut(QKeySequence::ZoomOut);



}

void MainWindow::blurImage(int position)
{

}

void MainWindow::cartoonifyImage(int position)
{

}

void MainWindow::onOpenImagePressed()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Image"), "", tr("Image Files (*.png *.jpg *.jpeg *.bmp)"));
    if(fileName.isEmpty())
    {
        return;
    }

    currentFile = fileName;
    QImage image(fileName);
    if(image.isNull())
    {
        QMessageBox::information(this, tr("Image Processing App"), tr("Cannot load %1.").arg(fileName));
        return;
    }

    // Display image

    ui->imageGraphicsView->setScene(new QGraphicsScene(this));
    ui->imageGraphicsView->scene()->addPixmap(QPixmap::fromImage(image));
    ui->imageGraphicsView->fitInView(ui->imageGraphicsView->scene()->itemsBoundingRect(), Qt::KeepAspectRatio);
    ui->imageGraphicsView->show();

    isImageOpened = true;

    ui->openCamera->setEnabled(false);
    ui->takePhoto->setEnabled(false);

    // Show status message
    ui->statusbar->showMessage("Image loaded", DEFAULT_TIMEOUT);

}

void MainWindow::saveImage()
{
    if(!isImageOpened)
    {
        return;
    }

    if(currentFile.isEmpty())
    {
        saveImageAs();
        return;
    }
    // Save the image in the same location

    QPixmap pixmap = ui->imageGraphicsView->grab();
    QImage image = pixmap.toImage();
    image.save(currentFile);

    if(!image.save(currentFile))
    {
        QMessageBox::information(this, tr("Image Processing App"), tr("Cannot save %1.").arg(currentFile));
    }
    else{
        ui->statusbar->showMessage("Image saved", DEFAULT_TIMEOUT);

    }

    //Reload the image
    /*QImage newImage(currentFile);
    ui->imageGraphicsView->scene()->clear();
    ui->imageGraphicsView->scene()->addPixmap(QPixmap::fromImage(newImage));
    ui->imageGraphicsView->fitInView(ui->imageGraphicsView->scene()->itemsBoundingRect(), Qt::KeepAspectRatio);
    ui->imageGraphicsView->show();
     */


}

void MainWindow::saveImageAs()
{
// Check if an image is opened
    if(!isImageOpened)
    {
        return;
    }

    // Get the current image and save the image in the location given by the user
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setNameFilter(tr("Images (*.png *.xpm *.jpg)"));
    dialog.setDefaultSuffix("png");

    QString fileName = dialog.getSaveFileName(this, tr("Save Image"), "", tr("Images (*.png *.xpm *.jpg)"));
    if(fileName.isEmpty())
    {
        return;
    }

    QPixmap pixmap = ui->imageGraphicsView->grab();
    QImage image = pixmap.toImage();
    image.save(fileName);
    ui->statusbar->showMessage("Image saved", DEFAULT_TIMEOUT);
}

void MainWindow::closeImage()
{
    if(!isImageOpened)
    {
        return;
    }

    ui->imageGraphicsView->scene()->clear();
    isImageOpened = false;
    ui->openCamera->setEnabled(true);
    ui->takePhoto->setEnabled(true);
    ui->statusbar->showMessage("Image closed", DEFAULT_TIMEOUT);

}

void MainWindow::zoomIn()
{
    //Check if the image is loaded
    if(!isImageOpened)
    {
        return;
    }

    // Zoom in the image, save and show the current state of the image


    ui->imageGraphicsView->scale(1.1, 1.1);
    ui->statusbar->showMessage("Zoomed in", DEFAULT_TIMEOUT);

}

void MainWindow::zoomOut()
{
    //Check if the image is loaded
    if(!isImageOpened)
    {
        return;
    }

    // Zoom out the image

    ui->imageGraphicsView->scale(0.9, 0.9);
    ui->statusbar->showMessage("Zoomed out", DEFAULT_TIMEOUT);

}

void MainWindow::fitToWindow()
{
    if(!isImageOpened)
    {
        return;
    }

    // Fit the image to the window
    ui->imageGraphicsView->resetTransform();
    ui->imageGraphicsView->fitInView(ui->imageGraphicsView->scene()->itemsBoundingRect(),Qt::KeepAspectRatio);
    ui->statusbar->showMessage("Fitted to windows size", DEFAULT_TIMEOUT);

}

void MainWindow::normalSize()
{
    if(!isImageOpened)
    {
        return;
    }

    // Reset the image to its original size
    auto currentImage = ui->imageGraphicsView->scene()->items().first();
    ui->imageGraphicsView->resetTransform();
    ui->imageGraphicsView->centerOn(currentImage);
    ui->imageGraphicsView->show();

    ui->statusbar->showMessage("Original size", DEFAULT_TIMEOUT);

}

void MainWindow::rotateLeft()
{
    if(!isImageOpened)
    {
        return;
    }

    // Rotate the image to the left

    ui->imageGraphicsView->rotate(-90);
    ui->statusbar->showMessage("Rotated left", DEFAULT_TIMEOUT);

}

void MainWindow::rotateRight()
{
    if(!isImageOpened)
    {
        return;
    }

    // Rotate the image to the right
    ui->imageGraphicsView->rotate(90);
    ui->statusbar->showMessage("Rotated right", DEFAULT_TIMEOUT);



}

void MainWindow::flipHorizontal()
{
    if(!isImageOpened)
    {
        return;
    }
    // Flip the image horizontally
    ui->imageGraphicsView->scale(-1, 1);
    ui->statusbar->showMessage("Flipped horizontally", DEFAULT_TIMEOUT);
}

void MainWindow::flipVertical()
{
    if(!isImageOpened)
    {
        return;
    }

    // Flip the image vertically
    ui->imageGraphicsView->scale(1, -1);
    ui->statusbar->showMessage("Flipped vertically", DEFAULT_TIMEOUT);


}


void MainWindow::onBlurHSSliderMoved(int position)
{
    if(!isImageOpened)
    {
        return;
    }

    if(position == 0)
    {
        position = 1;  // smallest possible valid kernel size
    }
    else if(position % 2 == 0)
    {
        position += 1;  // ensure that the position is odd
    }

    // Apply blur filter to the image
    cv::Mat image = cv::imread(currentFile.toStdString(), cv::COLOR_BGR2RGB);
    cv::GaussianBlur(image, blurredImage, cv::Size(position, position), 0, 0,
                     cv::BORDER_REFLECT);
    cv::cvtColor(blurredImage, blurredImage, cv::COLOR_BGR2RGB);
    QImage qImage = QImage((uchar*) blurredImage.data, blurredImage.cols, blurredImage.rows, blurredImage.step, QImage::Format_RGB888);
    ui->imageGraphicsView->scene()->clear();
    ui->imageGraphicsView->scene()->addPixmap(QPixmap::fromImage(qImage));
    ui->imageGraphicsView->fitInView(ui->imageGraphicsView->scene()->itemsBoundingRect(), Qt::KeepAspectRatio);
    ui->imageGraphicsView->show();

    ui->statusbar->showMessage("Blurred", DEFAULT_TIMEOUT);

}


void MainWindow::onCartoonifyHSSliderMoved(int position)
{
    if(!isImageOpened)
    {
        return;
    }

    if(position == 0)
    {
        position = 1;  // smallest possible valid kernel size
    }
    else if(position % 2 == 0)
    {
        position += 1;  // ensure that the position is odd
    }

    // Apply cartoonify filter to the image
    //Get currrent state of the image
    cv::Mat image = cv::imread(currentFile.toStdString(), cv::COLOR_BGR2RGB);
    cv::Mat grayImage;
    cv::cvtColor(image, grayImage, cv::COLOR_BGR2GRAY);
    cv::Mat blurredImage;
    cv::GaussianBlur(grayImage, blurredImage, cv::Size(position, position), 0, 0, cv::BORDER_DEFAULT);
    cv::Mat edges;
    cv::adaptiveThreshold(blurredImage, edges, 255, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY, position, 9);
    cv::Mat color;
    cv::bilateralFilter(image, color, 9, position*2, position/2);
    cv::Mat cartoonImage;
    cv::bitwise_and(color, color, cartoonImage, edges);
    cv::cvtColor(cartoonImage, cartoonImage, cv::COLOR_BGR2RGB);
    QImage qImage = QImage((uchar*) cartoonImage.data, cartoonImage.cols, cartoonImage.rows, cartoonImage.step, QImage::Format_RGB888);
    ui->imageGraphicsView->scene()->clear();
    ui->imageGraphicsView->scene()->addPixmap(QPixmap::fromImage(qImage));
    ui->imageGraphicsView->fitInView(ui->imageGraphicsView->scene()->itemsBoundingRect(), Qt::KeepAspectRatio);
    ui->imageGraphicsView->show();

    ui->statusbar->showMessage("Cartoonified", DEFAULT_TIMEOUT);




}


void MainWindow::onGaussianBlurHSSliderMoved(int position)
{
    if(!isImageOpened)
    {
        return;
    }

    if(position == 0)
    {
        position = 1;  // smallest possible valid kernel size
    }
    else if(position % 2 == 0)
    {
        position += 1;  // ensure that the position is odd
    }

    // Apply Gaussian blur filter to the image

    cv::Mat image = cv::imread(currentFile.toStdString(), cv::COLOR_BGR2RGB);
    cv::GaussianBlur(image, blurredImage, cv::Size(position, position), 0, 0,
                     cv::BORDER_REFLECT);
    cv::cvtColor(blurredImage, blurredImage, cv::COLOR_BGR2RGB);
    QImage qImage = QImage((uchar*) blurredImage.data, blurredImage.cols, blurredImage.rows, blurredImage.step, QImage::Format_RGB888);
    ui->imageGraphicsView->scene()->clear();
    ui->imageGraphicsView->scene()->addPixmap(QPixmap::fromImage(qImage));
    ui->imageGraphicsView->fitInView(ui->imageGraphicsView->scene()->itemsBoundingRect(), Qt::KeepAspectRatio);
    ui->imageGraphicsView->show();

    ui->statusbar->showMessage("Gaussian Blurred", DEFAULT_TIMEOUT);


}


void MainWindow::onSharpenHSSliderMoved(int position)
{
    if(!isImageOpened)
    {
        return;
    }

    if(position == 0)
    {
        position = 1;  // smallest possible valid kernel size
    }
    else if(position % 2 == 0)
    {
        position += 1;  // ensure that the position is odd
    }

    // Apply sharpen filter to the image
    cv::Mat image = cv::imread(currentFile.toStdString(), cv::COLOR_BGR2RGB);
    cv::Mat kernel = (cv::Mat_<char>(3,3) << 0, -1, 0, -1, 5, -1, 0, -1, 0);
    cv::filter2D(image, blurredImage, image.depth(), kernel);
    cv::cvtColor(blurredImage, blurredImage, cv::COLOR_BGR2RGB);
    QImage qImage = QImage((uchar*) blurredImage.data, blurredImage.cols, blurredImage.rows, blurredImage.step, QImage::Format_RGB888);
    ui->imageGraphicsView->scene()->clear();
    ui->imageGraphicsView->scene()->addPixmap(QPixmap::fromImage(qImage));
    ui->imageGraphicsView->fitInView(ui->imageGraphicsView->scene()->itemsBoundingRect(), Qt::KeepAspectRatio);
    ui->imageGraphicsView->show();


}

void MainWindow::openVideo()
{

}

void MainWindow::openCamera()
{
    // Start the video thread
    videoThread = new VideoThread(videoLabel);
    videoThread->start();
    ui->statusbar->showMessage("Camera opened", DEFAULT_TIMEOUT);

    isCameraRunning = true;
    ui->statusbar->showMessage("Camera opened", DEFAULT_TIMEOUT);






}

void MainWindow::closeCamera()
{

}

void MainWindow::showCameraInfo()
{


}

void MainWindow::addGlasses()
{


}

void MainWindow::addMustache()
{

}

void MainWindow::makeCartoon()
{

}

void MainWindow::makeOld()
{

}

void MainWindow::onPerformOCRPressed()
{

}

void MainWindow::onSaveTextPressed()
{

}



void MainWindow::onAddMouseStateChanged(int arg1)
{

}


void MainWindow::onOpenCameraPressed()
{

}


void MainWindow::onTakePhotoPressed()
{

}

