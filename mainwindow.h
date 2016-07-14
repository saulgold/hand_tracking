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
   void on_getThreshold_clicked();



private:
    Ui::MainWindow *ui;
    QImage MainWindow::convertOpenCVMatToQtQImage(cv::Mat mat);
    QTimer *timer;
      cv::VideoCapture m_cap;
     int thresholdSliderValue,thresholdSliderValue1;
     int blurSize;
     cv::Scalar roiMean1,roiMean2,roiMean3,roiMean4,roiSD ;
    cv::vector<cv::vector<cv::Point> > contours;
     cv::vector<cv::Vec4i> hierarchy;
    cv::vector<cv::Point> max_contour, max_convex;
    cv::vector<cv::Point> points;
    bool mean_button;
    cv::Mat roi1;
    cv::Mat frame_hsv;
    cv::Mat frame_hsv_threshold,frame_hsv_contours;
    QImage qframe;
};

#endif // MAINWINDOW_H
