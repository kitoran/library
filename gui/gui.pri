#if(guiPers) {
include($$PWD/../settingsS/settings/settings.pri)
DEFINES += GUI_RESOURCE_PATH=\\\"$$PWD/gui/icons\\\"
SOURCES +=  \
    $$PWD/gui/layoutStack.c \
    $$PWD/gui/linelayout.c \
    $$PWD/gui/loadImage.c \
    $$PWD/gui/persistent.c \
    $$PWD/gui/draw.c \
    $$PWD/gui/toolbuttongroup.c \
    $$PWD/gui/windowlayout.c
SDL {
    SOURCES += \
    $$PWD/gui/backends/sdlbackend.c
    DEFINES += SDL
}
X {
SOURCES += \
    $$PWD/gui/backends/xbackend.c
    INCLUDEPATH += /usr/include/freetype2/
}
HEADERS +=  \
    $$PWD/gui/backends/backend.h \
    $$PWD/gui/combobox.h \
    $$PWD/gui/layoutStack.h \
    $$PWD/gui/linelayout.h \
    $$PWD/gui/loadImage.h \
    $$PWD/gui/persistent.h \
    $$PWD/gui/guiglobals.h \
    $$PWD/gui/draw.h \
    $$PWD/gui/toolbuttongroup.h \
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
    $$PWD/gui/color.h

INCLUDEPATH += $$PWD/gui/
INCLUDEPATH += $$PWD/gui/backends/

X {
#LIBS += -lSDL2 #-lSDL_TTF
CONFIG += link_pkgconfig
PKGCONFIG += x11 xrender xft
}
include($$PWD/../stb/stb_image.pri)
include($$PWD/../misc/misc.pri)
include($$PWD/../stb/stb_image_write.pri)
