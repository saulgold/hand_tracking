#include "mainwindow.h"
#include "ui_mainwindow.h"

#include<QFileDialog>
#include<QtCore>



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    if(!m_cap.open(0)){
        qDebug()<<"webcam no open";
    }

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
  cv::Mat frame;
  QImage qframe;


  m_cap>>frame;


  qframe = convertOpenCVMatToQtQImage(frame);
  QPixmap pix = QPixmap::fromImage(qframe);
  ui->videoScreen->setPixmap(pix.scaled(500, 500, Qt::IgnoreAspectRatio, Qt::FastTransformation));
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
