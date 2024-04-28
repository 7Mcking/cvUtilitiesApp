#pragma once
#ifdef videoThread_h
#define videoThread_h
#endif // videoThread_h


#include <QThread>
#include <QImage>
#include <QPixmap>
#include <QLabel>
#include <QTimer>
#include <QMutex>

#include <opencv2/opencv.hpp>

class VideoThread : public QThread {
Q_OBJECT
public:
    VideoThread(QLabel *label);
    ~VideoThread() override;

protected:
    void run() override;

private slots:
    void updateFrame();
    void startRecording();
    void stopRecording();
    void pauseRecording();
    void saveRecording();

private:
    QLabel *label;
    cv::VideoCapture cap;
    cv::Mat frame;
    QTimer *timer;
    QMutex mutex;
    bool isRecording = false;
    cv::VideoWriter writer;
    QString currentFile;
    bool isPaused = false;
    bool isStopped = false;
    bool isSaved = false;
    bool isVideoOpened = false;
    bool isCameraRunning = false;
    bool isVideoRunning = false;
    bool isVideoPaused = false;
    bool isVideoStopped = false;
    bool isVideoSaved = false;
    bool isVideoRecording = false;

    void openCamera();
    void openVideo();
    void closeVideo();
    void closeCamera();
    void startVideo();
    void stopVideo();
    void pauseVideo();
    void saveVideo();

//Variables
    int frameWidth;
    int frameHeight;
    int frameRate;
    int codec;

};



