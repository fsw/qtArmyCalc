# -------------------------------------------------
# Project created by QtCreator 2011-07-31T17:59:20
# -------------------------------------------------
QT += network \
    webkit \
    xml
TARGET = qtArmyCalc
TEMPLATE = app
INCLUDEPATH += libs/libmaia
LIBS += libs/libmaia/libmaia.a
SOURCES += src/main.cpp \
    src/mainwindow.cpp
HEADERS += src/mainwindow.h
FORMS += src/mainwindow.ui
RESOURCES += icons/resources.qrc
