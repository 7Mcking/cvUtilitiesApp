#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "videoThread.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;
    void setupActions();
    void setupConnections();
    void showAbout();
    void setupShortCuts();

    //Image Processing
    void blurImage(int position);
    void cartoonifyImage(int position);



private:
    Ui::MainWindow *ui;
    QString currentFile;
    bool isImageOpened = false;
    bool isCameraRunning = false;
    VideoThread *videoThread;
    QLabel *videoLabel;
    cv::Mat blurredImage;



private slots:
    //Images
    void onOpenImagePressed();
    void saveImage();
    void saveImageAs();
    void closeImage();

    void zoomIn();
    void zoomOut();
    void fitToWindow();
    void normalSize();
    void rotateLeft();
    void rotateRight();
    void flipHorizontal();
    void flipVertical();


    //Image Processing
    void onBlurHSSliderMoved(int position);
    void onCartoonifyHSSliderMoved(int position);
    void onGaussianBlurHSSliderMoved(int position);
    void onSharpenHSSliderMoved(int position);

    //Video Processing
    void openVideo();
    void openCamera(); //Need to check
    void closeCamera(); //Need to check
    void showCameraInfo();

    //Add Face Masks
    void addGlasses();
    void addMustache();
    void makeCartoon();
    void makeOld();

    //Text Detection
    void onPerformOCRPressed();
    void onSaveTextPressed();
    void onAddMouseStateChanged(int arg1);
    void onOpenCameraPressed();
    void onTakePhotoPressed();
};
#endif // MAINWINDOW_H
