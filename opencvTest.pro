#-------------------------------------------------
#
# Project created by QtCreator 2016-06-27T23:09:03
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = opencvTest
TEMPLATE = app

QMAKE_LFLAGS +=/STACK:32000000

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

INCLUDEPATH += C:\\opencv2413\\build\\include \
               C:\\opencv2413\\build\\include\\opencv \
               C:\\opencv2413\\build\\include\\opencv2


LIBS += -LC:\opencv2413\build\x86\vc12\lib \
    -lopencv_imgproc2413d \
    -lopencv_objdetect2413d \
    -lopencv_photo2413d \
    -lopencv_video2413d \
    -lopencv_calib3d2413d \
    -lopencv_core2413d \
    -lopencv_features2d2413d \
    -lopencv_flann2413d \
    -lopencv_highgui2413d


