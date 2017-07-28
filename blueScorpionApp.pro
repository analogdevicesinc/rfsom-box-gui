#-------------------------------------------------
#
# Project created by QtCreator 2017-06-21T17:07:45
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG+= C++11

TARGET = blueScorpionApp
TEMPLATE = app

stylesheets.path = /home/analog/blueScorpionApp/stylesheets
stylesheets.files = ./stylesheets/*.qss

icons.path = /home/analog/blueScorpionApp/icons
icons.files = ./icons/*.png ./icons/*.ico ./icons/*.svg

config.path = /home/analog/blueScorpionApp/bin
config.files = ./*.json

target.path = /home/analog/blueScorpionApp/bin

INSTALLS += target config icons stylesheets

SOURCES += main.cpp\
        mainwindow.cpp \
    landingpage.cpp \
    menupage.cpp \
    mainmenu.cpp \
    sysfsvalue.cpp \
    apppage.cpp \
    launcherpage.cpp \
    appgenericlist.cpp \
    app.cpp \
    buttonelementui.cpp \
    readonlyelementui.cpp \
    scriptresult.cpp \
    checkboxelementui.cpp \
    editboxelementui.cpp \
    numericelementui.cpp \
    appvideoplayer.cpp \
    ipaddresselementui.cpp

HEADERS  += mainwindow.h \
    app.h \
    appgenericlist.h \
    apppage.h \
    buttonelementui.h \
    checkboxelementui.h \
    editboxelementui.h \
    hardware.h \
    landingpage.h \
    launcherpage.h \
    mainmenu.h \
    menupage.h \
    readonlyelementui.h \
    scriptresult.h \
    stringvalue.h \
    sysfsvalue.h \
    numericelementui.h \
    appvideoplayer.h \
    ipaddresselementui.h \
    uielement.h

FORMS    += mainwindow.ui \
    readonlyvalue.ui \
    buttonElement.ui \
    checkboxElement.ui \
    numericedit.ui \
    editboxElement.ui

RESOURCES += \
    res.qrc


