#if(guiPers) {
include($$PWD/../settingsS/settings/settings.pri)
include($$PWD/../loadImage/loadImage.pri)
DEFINES += GUI_RESOURCE_PATH=\\\"$$PWD/gui/icons\\\"
SOURCES +=  \
    $$PWD/gui/layoutStack.c \
    $$PWD/gui/linelayout.c \
    $$PWD/gui/persistent.c \
    $$PWD/gui/draw.c \
    $$PWD/gui/toolbuttongroup.c \
    $$PWD/gui/windowlayout.c
SDL {
    SOURCES += $$PWD/gui/backends/x/sdlbackend.c
    DEFINES += SDL
}
X {
SOURCES += \
    $$PWD/gui/backends/x/xbackend.c
    INCLUDEPATH += /usr/include/freetype2/
}
HEADERS +=  \
    $$PWD/gui/layoutStack.h \
    $$PWD/gui/linelayout.h \
    $$PWD/gui/persistent.h \
    $$PWD/gui/guiglobals.h \
    $$PWD/gui/draw.h \
    $$PWD/gui/toolbuttongroup.h \
    $$PWD/gui/backends/x/backend.h \
    $$PWD/gui/windowlayout.h
#}
SOURCES += \
    $$PWD/gui/gui.c \
#    $$PWD/gui/stb.c \
    $$PWD/gui/gridlayout.c \
    $$PWD/gui/combobox.c \
    $$PWD/gui/guiglobals.c \


HEADERS += \
    $$PWD/gui/gui.h \
#    $$PWD/gui/stb_ds.h \
    $$PWD/gui/gridlayout.h \
    $$PWD/gui/color.h \
    $$PWD/gui/shittyintrospection.h 

INCLUDEPATH += $$PWD/gui/
INCLUDEPATH += $$PWD/gui/backends/x

X {
#LIBS += -lSDL2 #-lSDL_TTF
CONFIG += link_pkgconfig
PKGCONFIG += x11 xrender xft
}
