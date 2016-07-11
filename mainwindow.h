#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <stdio.h>
#include <opencv2/core/core.hpp>
#include<opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

#include "qdebug.h"
#include <QImage>
#include <QtCore>
#include<iostream>
#include<vector>
#include<algorithm>
#include "vidcap.h"

#include "opencv2/opencv.hpp"

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

private:
    Ui::MainWindow *ui;
    QImage MainWindow::convertOpenCVMatToQtQImage(cv::Mat mat);
    QTimer *timer;
      cv::VideoCapture m_cap;
    //cv::Ptr<cv::BackgroundSubtractorMOG2> pMOG;

};

#endif // MAINWINDOW_H
