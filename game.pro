#-------------------------------------------------
#
# Project created by QtCreator 2020-11-03T09:23:04
#
#-------------------------------------------------

QT       += core gui
QT += multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = game
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp \
    bird.cpp\
    person.cpp

HEADERS  += mainwindow.h \
    bird.h\
    person.h

FORMS    += mainwindow.ui

LIBS += -I/usr/include/opencv

LIBS += /usr/lib/arm-linux-gnueabihf/libopencv_highgui.so \
       /usr/lib/arm-linux-gnueabihf/libopencv_core.so    \
       /usr/lib/arm-linux-gnueabihf/libopencv_imgproc.so \
       /usr/lib/arm-linux-gnueabihf/libopencv_imgcodecs.so \
       /usr/lib/arm-linux-gnueabihf/libopencv_videoio.so.3.2
