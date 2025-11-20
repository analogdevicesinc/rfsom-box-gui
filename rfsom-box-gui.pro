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
scripts.files += ./bin/get_gmap.sh
scripts.files += ./bin/camera_view.sh
scripts.files += ./bin/start_terminal.sh
scripts.files += ./bin/read_pss
scripts.files += ./bin/start_stream.sh
scripts.files += ./bin/recv_stream.sh
scripts.files += ./bin/start_plot.sh
scripts.files += ./bin/enc-onoff.sh
scripts.files += ./bin/batt_man.sh
scripts.files += ./bin/send_tone.sh
scripts.files += ./tun_tap/en_macsec.sh
scripts.files += ./bin/start_buffer_stream.sh
scripts.files += ./bin/configure_peer.sh
scripts.files += ./bin/testmode.sh

share.path = $$INSTALL_LOCATION/share
share.files = ./share/*

# Generate systemd service file from template
systemd_template.input = ./bin/rfsom-box-gui.service.in
systemd_template.output = ./bin/rfsom-box-gui.service
QMAKE_SUBSTITUTES += systemd_template

systemd.path = /$$INSTALL_LOCATION/lib/systemd/system
systemd.files = ./bin/rfsom-box-gui.service

target.path = $$INSTALL_LOCATION/bin
permission.path = $$INSTALL_LOCATION/bin
unix:permission.extra = chmod 777 $$INSTALL_LOCATION/bin/rfsom-box-gui-start.sh;
unix:permission.extra += chmod 777 $$INSTALL_LOCATION/bin/gps_get_coords.sh;
unix:permission.extra += chmod 777 $$INSTALL_LOCATION/bin/read_pss;
unix:permission.extra += chmod 777 $$INSTALL_LOCATION/bin/start_stream.sh;
unix:permission.extra += chmod 777 $$INSTALL_LOCATION/bin/en_macsec.sh;
unix:permission.extra += chmod 777 $$INSTALL_LOCATION/bin/get_gmap.sh;
unix:permission.extra += chmod 777 $$INSTALL_LOCATION/bin/enc-onoff.sh;
unix:permission.extra += chmod 777 $$INSTALL_LOCATION/bin/batt_man.sh;
unix:permission.extra += chmod 777 $$INSTALL_LOCATION/bin/send_tone.sh;
unix:permission.extra += chmod 777 $$INSTALL_LOCATION/bin/start_buffer_stream.sh;
unix:permission.extra += chmod 777 $$INSTALL_LOCATION/bin/configure_peer.sh;
unix:permission.extra += chmod 777 $$INSTALL_LOCATION/bin/testmode.sh;

INSTALLS += target scripts share permission systemd

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
    $$SRC_DIR/common.h \
    src/toastnotification.h

FORMS    += $$UI_DIR/mainwindow.ui \
    $$UI_DIR/readonlyvalue.ui \
    $$UI_DIR/buttonElement.ui \
    $$UI_DIR/checkboxElement.ui \
    $$UI_DIR/numericedit.ui \
    $$UI_DIR/editboxElement.ui

RESOURCES += \
   $$RES_DIR/res.qrc

