
SOURCES *= \
    $$PWD/getline.c \
    $$PWD/settings.c

HEADERS += \
        $$PWD/getline.h \
        $$PWD/settings.h


include($$PWD/../../stb/stb_ds.pri)
INCLUDEPATH += $$PWD
