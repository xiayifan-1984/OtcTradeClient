#-------------------------------------------------
#
# Project created by QtCreator 2019-03-07T13:29:33
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET =		OtcTradeClientRaw
TEMPLATE =		app


# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
QMAKE_CXXFLAGS +=  -Wno-unused-parameter

CONFIG += c++14

LIBS += -lDbgHelp
LIBS += D:\workplace\OtcTradeClient\bin\kafkaMsgHandler.dll



SOURCES += main.cpp


RESOURCES           += other/qss.qrc
RESOURCES           += other/main.qrc


RC_ICONS = other/image/main.ico


#=============================================================
#[section01]
MOC_DIR             = temp/moc
RCC_DIR             = temp/rcc
UI_DIR              = temp/ui
OBJECTS_DIR         = temp/obj
DESTDIR             = $$PWD/../bin



#[section02]
INCLUDEPATH         += $$PWD
INCLUDEPATH         += $$PWD/../ShareInclude
INCLUDEPATH	    += $$PWD/guiRaw
INCLUDEPATH         += $$PWD/module
INCLUDEPATH         += $$PWD/util
INCLUDEPATH	    += $$PWD/greek
INCLUDEPATH         += $$PWD/optpricingapplication


include             ($$PWD/guiRaw/guiRaw.pri)
include             ($$PWD/module/module.pri)
include             ($$PWD/util/util.pri)
include		    ($$PWD/greek/greek.pri)
include             ($$PWD/optpricingapplication/optpricingapplication.pri)

#=============================================================


#QMAKE_LFLAGS_RELEASE +=/MAP /DEBUG  /opt:ref /INCREMENTAL:NO

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

