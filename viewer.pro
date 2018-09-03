#-------------------------------------------------
#
# Project created by QtCreator 2017-05-14T20:49:17
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets xml

TARGET = viewer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    zoomleveldescription.cpp \
    imagedescription.cpp \
    graphicsitemgroup.cpp \
    parameterstuner.cpp \
    nucleus.cpp \
    processimagedbinterraction.cpp \
    dbhandler.cpp \
    positionindb.cpp

HEADERS  += mainwindow.h \
    zoomleveldescription.h \
    imagedescription.h \
    graphicsitemgroup.h \
    parameterstuner.h \
    nucleus.h \
    processimagedbinterraction.h \
    dbhandler.h \
    positionindb.h

FORMS    += mainwindow.ui
