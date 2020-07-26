#-------------------------------------------------
#
# Project created by QtCreator 2011-05-17T09:51:07
#
#-------------------------------------------------
QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Haji_Gesture_Demo
TEMPLATE = app
target.path=/usr/local/bin
INSTALLS=target



SOURCES += main.cpp\
        mainwindow.cpp \
    gestureAlgorithm.cpp \
    gestureRender.cpp \


HEADERS  += mainwindow.h \
    gestureAlgorithm.h \
    gesture_DS.h \
    gesture_Math.h \
    gestureRender.h \


FORMS    += mainwindow.ui \


RESOURCES += \
    resources.qrc
