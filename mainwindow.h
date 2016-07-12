#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <stdio.h>
#include <opencv2/core/core.hpp>
#include<opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/video.hpp>
#include "qdebug.h"
#include <QImage>
#include <QtCore>
#include<iostream>
#include<vector>
#include<algorithm>
#include "vidcap.h"
#include "opencv2/opencv.hpp"
#include <vector>
#include <stdio.h>
#include<stdlib.h>

#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>

#include <opencv2/imgproc/imgproc_c.h>
#include <opencv2/features2d/features2d.hpp>
#include "ml.h"
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <opencv2/video/background_segm.hpp>
#include <QTime>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT


public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void updateGUI();

    void on_thresholdSlider_valueChanged(int value);

    void on_blurSlider_valueChanged(int value);

    void on_minSlider1_valueChanged(int value);

    void on_minSlider2_valueChanged(int value);

    void on_minSlider3_valueChanged(int value);

    void on_maxSlider1_valueChanged(int value);

    void on_maxSlider2_valueChanged(int value);

    void on_maxSlider3_valueChanged(int value);

    void on_backgroudCalibration_clicked();


    void on_getThreshold_clicked();

    void on_backgroudCalibration_released();

private:
    Ui::MainWindow *ui;
    QImage MainWindow::convertOpenCVMatToQtQImage(cv::Mat mat);
    QTimer *timer;
      cv::VideoCapture m_cap;
     int thresholdSliderValue,thresholdSliderValue1;
     int blurSize;
     std::vector<std::vector<cv::Point> > contours;
     std::vector<cv::Vec4i> hierarchy;
    cv::Mat m_bg;
    cv::Mat background_frame;
    //cv::Ptr<cv::BackgroundSubtractorMOG2> pMOG;
     cv::SimpleBlobDetector::Params params;
};

#endif // MAINWINDOW_H
