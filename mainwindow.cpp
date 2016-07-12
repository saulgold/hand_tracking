#include "mainwindow.h"
#include "ui_mainwindow.h"

#include<QFileDialog>
#include<QtCore>


cv::Mat MOGmask;
int min_1,min_2,min_3,max_1,max_2,max_3;
cv::Mat prev_frame,frame_gray;
cv::Mat m_frame,m_frame_col;
int adaptive_mask;
int dilate_size =1;
int erode_size =1;
int threshold_val = 200;
bool mean_button = false;
cv::BackgroundSubtractorMOG2 bg;
cv::Mat fgMaskMOG; //fg mask generated by MOG method
cv::Mat fgMaskMOG2; //fg mask fg mask generated by MOG2 method
cv::Mat fgMaskGMG; //fg mask fg mask generated by MOG2 method
cv::Ptr<cv::BackgroundSubtractor> pMOG = cv::Algorithm::create<cv::BackgroundSubtractorMOG>("BackgroundSubtractor.MOG"); //MOG Background subtractor
cv::Ptr<cv::BackgroundSubtractor> pMOG2 = cv::Algorithm::create<cv::BackgroundSubtractorMOG2>("BackgroundSubtractor.MOG2"); //MOG2 Background subtractor
cv::Ptr<cv::BackgroundSubtractor> pGMG = cv::Algorithm::create<cv::BackgroundSubtractorGMG>("BackgroundSubtractor.GMG"); //MOG2 Background subtractor
 bool background_frame_button = false;
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
    thresholdSliderValue = 1;
//    pMOG = new cv::BackgroundSubtractorMOG(); //MOG approach
//    pMOG2 = new cv::BackgroundSubtractorMOG2(); //MOG2 approach
    pMOG2->set("detectShadows",0);
    pMOG2->set("nmixtures",3);

    // pMOG = cv::createBackgroundSubtractorMOG2();
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::updateGUI(){

  cv::Mat frame2,contourMat,fore,back,skin_frame,min_frame, max_frame,canny_frame,adaptive_frame;

  min_frame.setTo(cv::Scalar(0,133,77));
  max_frame.setTo(cv::Scalar(255,173,127));


  QImage qframe;
  cv::Size blurMask = cv::Size(6,6);

   m_cap>>m_frame_col;
   cv::flip(m_frame_col,m_frame_col,1);
   cv::cvtColor(m_frame_col,m_frame,CV_BGR2GRAY);
   cv::cvtColor(m_frame_col,frame_gray,CV_BGR2GRAY);
//   cv::inRange(m_frame,cv::Scalar(min_1,min_2,min_3),cv::Scalar(max_1,max_2,max_3),skin_frame);
//   cv::blur(m_frame,m_frame,blurMask);
   cv::GaussianBlur(m_frame,m_frame,cv::Size(7,7),3,3);
//   adaptive_mask = (2*thresholdSliderValue) +1;
//  cv::adaptiveThreshold(frame_gray,frame_gray,255,CV_ADAPTIVE_THRESH_MEAN_C,CV_THRESH_BINARY,13,1);
  //cv::threshold(m_frame,m_frame,threshold_val,255,1);
//  cv::Mat dilate_mask = cv::getStructuringElement(cv::MORPH_RECT,cv::Size(dilate_size,dilate_size));
//  cv::dilate(m_frame,m_frame,dilate_mask);
//    cv::Mat erode_mask = cv::getStructuringElement(cv::MORPH_ELLIPSE,cv::Size(erode_size,erode_size));
//    cv::erode(m_frame,m_frame,erode_mask);
  // cv::fastNlMeansDenoising(m_frame,m_frame,3,7,21);
  //cv::Canny(m_frame,canny_frame,thresholdSliderValue,thresholdSliderValue);
    pMOG->operator()(frame_gray, fgMaskMOG,0.5);
    pMOG2->operator()(frame_gray, fgMaskMOG2,0.5);
    pGMG->operator()(frame_gray, fgMaskGMG,0.1);

    if(background_frame_button){
        m_cap>>background_frame;
        cv::flip(background_frame,background_frame,1);
        cv::cvtColor(background_frame,background_frame,CV_BGR2GRAY);
        cv::GaussianBlur(background_frame,background_frame,cv::Size(7,7),3,3);
        background_frame_button=false;
    }//else(background_frame = cv::Scalar::all(0));

    if(m_frame.cols==background_frame.cols && m_frame.rows==background_frame.rows ){
        m_frame = m_frame - background_frame;
    //cv::bitwise_not(m_frame,m_frame);

        cv::findContours(m_frame,contours,hierarchy,CV_RETR_TREE,CV_CHAIN_APPROX_NONE,cv::Point(0,0));
        cv::imshow("bg im",background_frame);


        double largest_area = 0;
        int largest_contour_index= 0;

        for(int i=0; i < contours.size();i++){
            double area = cv::contourArea(contours[i],false);

            if (area>largest_area){
                largest_area=area;
                largest_contour_index =i;
            }
        }

        cv::drawContours(m_frame_col,contours,largest_contour_index,cv::Scalar(255,0,50),2,8,hierarchy, 0, cv::Point());

//        if(m_frame.channels()==1){
//       uchar pixel;
//       uchar white =255;
//        for(int i=0; i<m_frame.rows;i++){
//            for(int j=0; j<m_frame.cols;j++){
//                pixel = m_frame.at<uchar>(i,j)- background_frame.at<uchar>(i,j);
//                if(pixel<0 ){
//                    m_frame.at<uchar>(i,j)=white;
//                }else(m_frame.at<uchar>(i,j)=pixel);
//               // qDebug()<<i<<", "<<j;
//            }
//        }
//    }else(qDebug()<<"subtraction only works on binary images");
    }

 //

// // cv::threshold(frame,frame,thresholdSliderValue,255,CV_THRESH_BINARY_INV);

    //cv::rectangle(m_frame,cv::Point (100,100),cv::Point(300,300),cv::Scalar(255,0,0),3,8,0);

    if(mean_button){
        int sum=0;
        for(int i =100; i<200;i++){
           for(int j =100; j<200;j++){
               sum += frame_gray.at<uchar>(i,j);
           }
        }
        mean_button=false;
        threshold_val = sum/(10000);
    }

  ui->meanIntensityLabel->setNum(threshold_val);
  qframe = convertOpenCVMatToQtQImage(m_frame);
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

}

void MainWindow::on_blurSlider_valueChanged(int value)
{
    dilate_size = 2*value+1;
}

void MainWindow::on_minSlider1_valueChanged(int value)
{
    min_1 = value;
}

void MainWindow::on_minSlider2_valueChanged(int value)
{
    min_2 = value;
}

void MainWindow::on_minSlider3_valueChanged(int value)
{
    min_3 = value;
}

void MainWindow::on_maxSlider1_valueChanged(int value)
{
    max_1 = value;
}

void MainWindow::on_maxSlider2_valueChanged(int value)
{
    max_2 = value;
}

void MainWindow::on_maxSlider3_valueChanged(int value)
{
    max_3 = value;
}

void MainWindow::on_backgroudCalibration_clicked()
{


}



void MainWindow::on_getThreshold_clicked()
{
    mean_button = true;
}

void MainWindow::on_backgroudCalibration_released()
{

      background_frame_button = true;
}
