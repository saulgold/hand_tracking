#include "mainwindow.h"
#include "ui_mainwindow.h"

#include<QFileDialog>
#include<QtCore>

cv::Ptr < cv::BackgroundSubtractorMOG2> pMOG2;
cv::Mat MOGmask;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{


    if(!m_cap.open(0)){
        qDebug()<<"webcam no open";
    }

    blurSize=1;
    ui->setupUi(this);
    timer = new QTimer(this);

    connect(timer,SIGNAL(timeout()),this,SLOT(updateGUI()));

    timer->start();

   // pMOG = cv::createBackgroundSubtractorMOG2();
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::updateGUI(){
  cv::Mat frame,contourMat,back,skin_frame,min_frame, max_frame;
  min_frame.setTo(cv::Scalar(0,133,77));
  max_frame.setTo(cv::Scalar(255,173,127));


  QImage qframe;
  cv::Size blurMask = cv::Size(blurSize,blurSize);

  m_cap>>frame;
  cv::flip(frame,frame,1);
  cv::cvtColor(frame,frame,CV_BGR2YCrCb);
  cv::inRange(frame,cv::Scalar(0,133,77),cv::Scalar(255,173,127),skin_frame);
//  cv::blur(frame,frame,blurMask);
// // cv::threshold(frame,frame,thresholdSliderValue,255,CV_THRESH_BINARY_INV);
//  cv::Canny(frame,frame,thresholdSliderValue,thresholdSliderValue);
//  cv::findContours(frame,contours,hierarchy,CV_RETR_TREE,CV_CHAIN_APPROX_SIMPLE,cv::Point(0,0));
//  contourMat = cv::Mat::zeros(frame.size(),CV_8UC3);

//  double largest_area = 0;
//  int largest_contour_index= 0;

//  for(int i=0; i<contours.size();i++){
//      double area = cv::contourArea(contours[i],false);
//      if (area>largest_area){
//          largest_area=area;
//          largest_contour_index =i;
//      }
//  }


// cv::drawContours(frame,contours,largest_contour_index,cv::Scalar(255,0,50),2,8,hierarchy, 0, cv::Point());


  qframe = convertOpenCVMatToQtQImage(skin_frame);
  QPixmap pix = QPixmap::fromImage(qframe);
  ui->videoScreen->setPixmap(pix);
}


QImage MainWindow::convertOpenCVMatToQtQImage(cv::Mat mat) {
    if(mat.channels() == 1) {                   // if grayscale image
        return QImage((uchar*)mat.data, mat.cols, mat.rows, mat.step, QImage::Format_Indexed8);     // declare and return a QImage
    } else if(mat.channels() == 3) {            // if 3 channel color image
        cv::cvtColor(mat, mat, CV_BGR2RGB);     // invert BGR to RGB
        return QImage((uchar*)mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);       // declare and return a QImage
         qDebug() << "in convertOpenCVMatToQtQImage, image was not 1 channel or 3 channel, should never get here";
    }
    return QImage();        // return a blank QImage if the above did not work
}

void MainWindow::on_thresholdSlider_valueChanged(int value)
{
    thresholdSliderValue = value;
}

void MainWindow::on_blurSlider_valueChanged(int value)
{
    blurSize = value;
}
