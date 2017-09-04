#-------------------------------------------------
#
# Project created by QtCreator 2017-06-21T17:07:45
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

message($$INSTALL_LOCATION)
isEmpty(INSTALL_LOCATION) {
        INSTALL_LOCATION=/usr/local
}
message(Target will be installed in $$INSTALL_LOCATION)

CONFIG+= C++11

TARGET = rfsom-box-gui
TEMPLATE = app

scripts.path = $$INSTALL_LOCATION/bin
scripts.files = ./bin/rfsom-box-gui-start.sh
scripts.files += ./bin/gps_get_coords.sh
scripts.files += ./bin/read_pss

scripts.files += ./tun_tap/cfg_modem.sh
scripts.files += ./tun_tap/en_dds.sh
scripts.files += ./tun_tap/en_dma.sh
scripts.files += ./tun_tap/en_radio.sh
scripts.files += ./tun_tap/restart_modem.sh
scripts.files += ./tun_tap/modemd

share.path = $$INSTALL_LOCATION/share
share.files = ./share/*

target.path = $$INSTALL_LOCATION/bin
permission.path = $$INSTALL_LOCATION/bin
unix:permission.extra = chmod 777 $$INSTALL_LOCATION/bin/rfsom-box-gui-start.sh;
unix:permission.extra += chmod 777 $$INSTALL_LOCATION/bin/gps_get_coords.sh;
unix:permission.extra += chmod 777 $$INSTALL_LOCATION/bin/read_pss;

unix:permission.extra += chmod 777 $$INSTALL_LOCATION/bin/cfg_modem.sh;
unix:permission.extra += chmod 777 $$INSTALL_LOCATION/bin/en_dds.sh;
unix:permission.extra += chmod 777 $$INSTALL_LOCATION/bin/en_dma.sh;
unix:permission.extra += chmod 777 $$INSTALL_LOCATION/bin/en_radio.sh;
unix:permission.extra += chmod 777 $$INSTALL_LOCATION/bin/restart_modem.sh;
unix:permission.extra += chmod 777 $$INSTALL_LOCATION/bin/modemd

INSTALLS += target scripts share permission

SRC_DIR = ./src
UI_DIR=./ui
RES_DIR=./resources

SOURCES += $$SRC_DIR/main.cpp\
        $$SRC_DIR/mainwindow.cpp \
    $$SRC_DIR/landingpage.cpp \
    $$SRC_DIR/menupage.cpp \
    $$SRC_DIR/mainmenu.cpp \
    $$SRC_DIR/sysfsvalue.cpp \
    $$SRC_DIR/apppage.cpp \
    $$SRC_DIR/launcherpage.cpp \
    $$SRC_DIR/appgenericlist.cpp \
    $$SRC_DIR/app.cpp \
    $$SRC_DIR/buttonelementui.cpp \
    $$SRC_DIR/readonlyelementui.cpp \
    $$SRC_DIR/scriptresult.cpp \
    $$SRC_DIR/checkboxelementui.cpp \
    $$SRC_DIR/editboxelementui.cpp \
    $$SRC_DIR/numericelementui.cpp \
    $$SRC_DIR/appvideoplayer.cpp \
    $$SRC_DIR/ipaddresselementui.cpp \
    $$SRC_DIR/enumelementui.cpp \
    src/common.cpp

HEADERS  += $$SRC_DIR/mainwindow.h \
    $$SRC_DIR/app.h \
    $$SRC_DIR/appgenericlist.h \
    $$SRC_DIR/apppage.h \
    $$SRC_DIR/buttonelementui.h \
    $$SRC_DIR/checkboxelementui.h \
    $$SRC_DIR/editboxelementui.h \
    $$SRC_DIR/landingpage.h \
    $$SRC_DIR/launcherpage.h \
    $$SRC_DIR/mainmenu.h \
    $$SRC_DIR/menupage.h \
    $$SRC_DIR/readonlyelementui.h \
    $$SRC_DIR/scriptresult.h \
    $$SRC_DIR/stringvalue.h \
    $$SRC_DIR/sysfsvalue.h \
    $$SRC_DIR/numericelementui.h \
    $$SRC_DIR/appvideoplayer.h \
    $$SRC_DIR/ipaddresselementui.h \
    $$SRC_DIR/uielement.h \
    $$SRC_DIR/enumelementui.h \
    $$SRC_DIR/common.h

FORMS    += $$UI_DIR/mainwindow.ui \
    $$UI_DIR/readonlyvalue.ui \
    $$UI_DIR/buttonElement.ui \
    $$UI_DIR/checkboxElement.ui \
    $$UI_DIR/numericedit.ui \
    $$UI_DIR/editboxElement.ui

RESOURCES += \
   $$RES_DIR/res.qrc

