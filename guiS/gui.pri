#if(guiPers) {
include($$PWD/../settingsS/settings/settings.pri)
include($$PWD/../loadImage/loadImage.pri)
SOURCES +=  \
    $$PWD/gui/persistent.c
HEADERS +=  \
    $$PWD/gui/persistent.h
#}
SOURCES += \
    $$PWD/gui/gui.c \
    $$PWD/gui/stb.c \
    $$PWD/gui/gridlayout.c

HEADERS += \
    $$PWD/gui/gui.h \
#    $$PWD/gui/stb_ds.h \
    $$PWD/gui/gridlayout.h \
    $$PWD/gui/color.h

INCLUDEPATH += $$PWD/gui/


CONFIG += link_pkgconfig
PKGCONFIG += x11 xrender xft
