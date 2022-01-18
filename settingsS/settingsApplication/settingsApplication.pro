TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
LIBS += -lexplain -lconfig
SOURCES += main.c \
    stb_ds.c

HEADERS += \
    stb_ds.h
