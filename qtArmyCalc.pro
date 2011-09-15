# -------------------------------------------------
# Project created by QtCreator 2011-07-31T17:59:20
# -------------------------------------------------
QT += network \
    webkit \
    xml
TARGET = qtArmyCalc
TEMPLATE = app
INCLUDEPATH += libs/libmaia \
    libs/quazip/quazip
LIBS += libs/libmaia/libmaia.a \
    libs/quazip/quazip/libquazip.a
SOURCES += src/main.cpp \
    src/mainwindow.cpp \
    src/dialogversioncheck.cpp
HEADERS += src/mainwindow.h \
    src/dialogversioncheck.h
FORMS += src/mainwindow.ui \
    src/dialogversioncheck.ui
RESOURCES += icons/resources.qrc \
    dist.qrc
