
#ifndef VIDEOTHREAD_CPP
#define VIDEOTHREAD_CPP


#include "videoThread.h"
#include <QLabel>
#include <QTimer>
#include <QMutex>
#include <opencv2/opencv.hpp>
#include <QImage>
#include <QPixmap>
#include <QDebug>
#include <QMessageBox>

VideoThread::VideoThread(QLabel *label) {
    this->label = label;
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateFrame()));
    timer->start(20);

}

VideoThread::~VideoThread() {
    mutex.lock();
    isRecording = false;
    isPaused = false;
    isStopped = false;
    isSaved = false;
    isVideoOpened = false;
    isCameraRunning = false;
    isVideoRunning = false;
    isVideoPaused = false;
    isVideoStopped = false;
    isVideoSaved = false;
    isVideoRecording = false;
    mutex.unlock();
    wait();
    delete timer;
    delete label;
    delete timer;



}

void VideoThread::run() {
    cap = cv::VideoCapture(0);
    if (!cap.isOpened()) {
        qDebug() << "Error: Could not open camera";
        return;
    }
    frame = cv::Mat();
    while (isCameraRunning) {
        mutex.lock();
        if (!cap.read(frame)) {
            qDebug() << "Error: Could not read frame";
            break;
        }
        cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB);
        QImage img(frame.data, frame.cols, frame.rows, frame.step, QImage::Format_RGB888);
        QPixmap pix = QPixmap::fromImage(img);
        label->setPixmap(pix);
        mutex.unlock();
    }
    cap.release();
    label->clear();
    qDebug() << "Camera closed";
    isCameraRunning = false;

}

void VideoThread::startRecording() {
    if(!isCameraRunning) {
        QMessageBox::warning(nullptr, "Error", "Camera is not running");
        qDebug() << "Error: Camera is not running";
        return;
    }

    isRecording = true;

}

void VideoThread::closeVideo() {

}

void VideoThread::updateFrame() {

}

void VideoThread::stopRecording() {

}

void VideoThread::pauseRecording() {

}

void VideoThread::saveRecording() {

}

void VideoThread::openCamera() {

    isCameraRunning = true;

}

void VideoThread::openVideo() {
    cv::VideoCapture cap;
    cap = cv::VideoCapture(0);
    if (!cap.isOpened()) {
        qDebug() << "Error: Could not open camera";
        return;
    }

    isVideoOpened = true;

    frameWidth = cap.get(cv::CAP_PROP_FRAME_WIDTH);
    frameHeight = cap.get(cv::CAP_PROP_FRAME_HEIGHT);
    frameRate = cap.get(cv::CAP_PROP_FPS);
    codec = cv::VideoWriter::fourcc('M', 'J', 'P', 'G');
    writer = cv::VideoWriter(currentFile.toStdString(), codec, frameRate, cv::Size(frameWidth, frameHeight));
    if (!writer.isOpened()) {
        qDebug() << "Error: Could not open video file";
        return;
    }
    frame = cv::Mat();

    while (true) {
        mutex.lock();
        if (!cap.read(frame)) {
            qDebug() << "Error: Could not read frame";
            break;
        }
        cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB);
        QImage img(frame.data, frame.cols, frame.rows, frame.step, QImage::Format_RGB888);
        QPixmap pix = QPixmap::fromImage(img);
        label->setPixmap(pix);
        if (isRecording) {
            writer.write(frame);
        }
        mutex.unlock();
    }
    cap.release();
    writer.release();
    label->clear();
    qDebug() << "Video closed";
    isVideoRunning = false;
    isRecording = false;
    isVideoOpened = false;
    isVideoRecording = false;


}

void VideoThread::closeCamera() {

        isCameraRunning = false;

}

void VideoThread::startVideo() {


}

void VideoThread::stopVideo() {

}

void VideoThread::pauseVideo() {

}

void VideoThread::saveVideo() {

}

#endif // VIDEOTHREAD_CPP


