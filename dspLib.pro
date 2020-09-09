TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        dspAPI\dspAPI.c \
        main.c \

HEADERS  += \
             dspAPI\dspAPI.h \

INCLUDEPATH += \
             dspAPI
