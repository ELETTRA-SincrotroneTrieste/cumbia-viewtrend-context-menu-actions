#-------------------------------------------------
#
# Project created by QtCreator 2019-09-03T15:28:56
#
#-------------------------------------------------


isEmpty(INSTALL_ROOT) {
    INSTALL_ROOT = /usr/local/cumbia-libs
}

isEmpty(prefix) {
    prefix = $${INSTALL_ROOT}
}

include($${INSTALL_ROOT}/include/cumbia-qtcontrols/cumbia-qtcontrols.pri)

QUMBIA_PLUGINS_LIBDIR=$${INSTALL_ROOT}/lib/qumbia-plugins


QT       += core gui datavisualization

TARGET = cumbia-viewtrend-context-menu-actions
TEMPLATE = lib
CONFIG += plugin

isEmpty(buildtype) {
        buildtype = release
} else {
    equals(buildtype, debug) {
        message("")
        message("debug build")
        message("")
    }
}

CONFIG += $${buildtype}

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    cumbia-viewtrend-ctx-menu-action.cpp \
    qutrendwidget.cpp

HEADERS += \
    cumbia-viewtrend-ctx-menu-action.h \
    qutrendwidget.h
DISTFILES += cumbia-viewtrend-context-menu-action.json  \
    README.md

unix {
    target.path = $${QUMBIA_PLUGINS_LIBDIR}
    INSTALLS += target
}
