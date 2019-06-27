
 
HEADERS += \
    $$PWD/mainwindow.h 

SOURCES += \
    $$PWD/mainwindow.cpp 

#[sec02]
INCLUDEPATH         += $$PWD
INCLUDEPATH         += $$PWD/control
INCLUDEPATH         += $$PWD/dialog
INCLUDEPATH         += $$PWD/form

#[sec03]
include             ($$PWD/control/control.pri)
include             ($$PWD/dialog/dialog.pri)
include             ($$PWD/form/form.pri)