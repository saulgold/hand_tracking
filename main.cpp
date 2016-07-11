#include "mainwindow.h"
#include <QApplication>


int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    //cv::Mat pic = cv::imread("C:\\Users\\saul\\Pictures\\lena_color.gif",CV_LOAD_IMAGE_COLOR);
  //  cv::imshow("pic",pic);

    return a.exec();
}
