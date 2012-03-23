#-------------------------------------------------
#
# Project created by QtCreator 2012-03-23T16:47:05
#
#-------------------------------------------------

QT       += core gui

TARGET = haisin20f
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    function.cpp \
    extractnumber.cpp \
    config.cpp

HEADERS  += mainwindow.h \
    function.h \
    extractnumber.h \
    config.h

FORMS    += mainwindow.ui
# opencv
INCLUDEPATH += D:/software/Library/opencv2.3.1/cmake/install/include
DEPENDPATH += D:/software/Library/opencv2.3.1/cmake/install/include
LIBS += -LD:/software/Library/opencv2.3.1/build/x86/mingw/lib \
    -llibopencv_core231 \
    -llibopencv_highgui231 \
    -llibopencv_imgproc231

OTHER_FILES += \
    README \
    LICENCE
