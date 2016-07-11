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

private:
    Ui::MainWindow *ui;
    QImage MainWindow::convertOpenCVMatToQtQImage(cv::Mat mat);
    QTimer *timer;
      cv::VideoCapture m_cap;
     int thresholdSliderValue;
     int blurSize;
     std::vector<std::vector<cv::Point> > contours;
     std::vector<cv::Vec4i> hierarchy;

    //cv::Ptr<cv::BackgroundSubtractorMOG2> pMOG;

};

#endif // MAINWINDOW_H
