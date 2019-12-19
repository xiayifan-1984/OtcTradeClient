
 
HEADERS += \
    $$PWD/sdiwindow.h \
    $$PWD/mainwindow.h

SOURCES += \
    $$PWD/sdiwindow.cpp \
    $$PWD/mainwindow.cpp

#[sec02]
INCLUDEPATH         += $$PWD
INCLUDEPATH         += $$PWD/control
INCLUDEPATH         += $$PWD/dialog
INCLUDEPATH         += $$PWD/form
INCLUDEPATH         += $$PWD/sdi

#[sec03]
include             ($$PWD/control/control.pri)
include             ($$PWD/dialog/dialog.pri)
include             ($$PWD/form/form.pri)
include             ($$PWD/sdi/sdi.pri)
