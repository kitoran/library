TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += link_pkgconfig
LIBS += -lSDL2 -lexplain
PKGCONFIG += x11
INCLUDEPATH += ../gui
#LIBS += -L../../bui
SOURCES += main.c

unix:!macx: LIBS += -L$$OUT_PWD/../gui/ -lgui

INCLUDEPATH += $$PWD/../gui
DEPENDPATH += $$PWD/../gui

unix:!macx: PRE_TARGETDEPS += $$OUT_PWD/../gui/libgui.a

