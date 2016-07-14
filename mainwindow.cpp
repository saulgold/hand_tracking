#include "mainwindow.h"
#include "ui_mainwindow.h"

#include<QFileDialog>
#include<QtCore>


cv::Mat m_frame,m_frame_col;
int m_number_of_fingers;
cv::vector< cv::vector< cv::Point> >hull;
cv::vector<cv::Point> fingers;
cv::vector< cv::Vec4i> defects;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    mean_button = false;


    if(!m_cap.open(0)){
        qDebug()<<"webcam no open";
    }

    ui->setupUi(this);
    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(updateGUI()));
    timer->start();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateGUI(){



    //capture frame from default webcam
    m_cap>>m_frame_col;

    //give white border to frame to help with contouring
    cv::copyMakeBorder(m_frame_col,m_frame_col,10,10,10,10,cv::BORDER_CONSTANT ,cv::Scalar(255,255,255));
    //flip frame to mirror user
    cv::flip(m_frame_col,m_frame_col,1);
    //change colour space to YCC to improve skin detection
    cv::cvtColor(m_frame_col,frame_hsv,CV_BGR2YCrCb);

    //if the ROI is calculated, apply threshold based on the mean picel value and standard deviation
    if(roiMean1!= cv::Scalar(0,0,0)){
        cv::inRange(frame_hsv,roiMean1-roiSD,roiMean1+roiSD,frame_hsv_threshold);
    }

    //if the thresholded frame is not empty do all the morphology and contour
    if(!frame_hsv_threshold.empty()){
        //erosion dilation cycles to remove noise
         cv::Mat dilate_mask = cv::getStructuringElement(cv::MORPH_ELLIPSE,cv::Size(8,8));
         cv::Mat erode_mask = cv::getStructuringElement(cv::MORPH_ELLIPSE,cv::Size(2,2));
         cv::dilate(frame_hsv_threshold,frame_hsv_threshold,dilate_mask);
         cv::erode(frame_hsv_threshold,frame_hsv_threshold,erode_mask);

         //find contours of frame
         frame_hsv_contours = frame_hsv_threshold;
         cv::findContours(frame_hsv_contours,contours, hierarchy,CV_RETR_TREE,CV_CHAIN_APPROX_NONE,cv::Point(0,0));
         cv::findContours(frame_hsv_contours,hull, hierarchy,CV_RETR_TREE,CV_CHAIN_APPROX_NONE,cv::Point(0,0));


         if(!contours.empty()){
             double largest_area = 0;
             int largest_contour_index= 0;

             //get largest contour which is assumed to be the hand
             for(int i=0; i < contours.size();i++){
                 double area = cv::contourArea(contours[i],false);
                 if (area>largest_area){
                     largest_area=area;
                     largest_contour_index =i;
                 }

             }
             //find the convex hull
             cv::convexHull( cv::Mat(contours[largest_contour_index]), hull[largest_contour_index], false,false );

             //find moments of contours
             std::vector<cv::Moments> mu(contours.size() );
             for( int i = 0; i < contours.size(); i++ )
             { mu[i] = moments( contours[i], false ); }

             //find centre of moment
             std::vector<cv::Point2f> mc( contours.size() );
             for( int i = 0; i < contours.size(); i++ )
             { mc[i] = cv::Point2f( mu[i].m10/mu[i].m00 , mu[i].m01/mu[i].m00 ); }

             //draw the contour, the hull and the centre
             cv::drawContours(m_frame_col,contours,largest_contour_index,cv::Scalar(255,0,50),2,8,hierarchy, 0, cv::Point());
             cv::drawContours(m_frame_col,hull,largest_contour_index,cv::Scalar(255,0,50),2,8,hierarchy, 0, cv::Point());
             circle( m_frame_col, mc[largest_contour_index], 4, cv::Scalar(30,55,190), -1, 8, 0 );

             max_contour = contours[largest_contour_index];
             max_convex = hull[largest_contour_index];
             std::vector<cv::Point> tips;
             cv::Point centre_point = mc[largest_contour_index];
             int k = 0;

             //find finger tips
             for( int i=0; i<max_contour.size();i++){
                 for(int j=0; j<max_convex.size();j++){
                     if (cv::norm(cv::Mat(max_contour[i]),cv::Mat(max_convex[j]))==0){

                         tips.push_back(max_contour[i]);
                         k++;

                     }
                 }
             }

             double distance = 0 ;
             double centre_distance=0;
             m_number_of_fingers=0;

             //refine fingertip selection
             for(int i = 1 ; i<tips.size();i++){

                 distance = cv::norm(cv::Mat(tips[i-1]),cv::Mat(tips[i]));
                 centre_distance = cv::norm(cv::Mat(tips[i]),cv::Mat(centre_point));
                 if(distance>20 && tips[i].y <  mc[largest_contour_index].y &&centre_distance >100 ){
                     m_number_of_fingers++;
                     cv::circle( m_frame_col, tips[i], 4, cv::Scalar(255,0,255), -1, 8, 0 );
                 }

             }

             if(m_number_of_fingers ==2){
                 ui->gesture_label->setText(QString("peace"));


             }
             else if(m_number_of_fingers== 5){
                 ui->gesture_label->setText(QString("open hand"));
             }
             else(ui->gesture_label->setText(QString("gesture")));
             ui->lcdNumber->display(int(m_number_of_fingers));

         }
     }







    cv::rectangle(frame_hsv,cv::Rect(100,100,100,100),cv::Scalar(255,0,100));
    cv::rectangle(m_frame_col,cv::Rect(100,100,100,100),cv::Scalar(255,0,100));




    qframe = convertOpenCVMatToQtQImage(m_frame_col);
    QPixmap pix = QPixmap::fromImage(qframe);
    ui->videoScreen->setPixmap(pix);
}




//function converts the cv::Mat() type to the QImage() type
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


//button for getting the mean pixel values from the box
void MainWindow::on_getThreshold_clicked()
{

    roi1 = frame_hsv(cv::Rect(100,100,100,100));


    cv::meanStdDev(roi1,roiMean1,roiSD);
}

