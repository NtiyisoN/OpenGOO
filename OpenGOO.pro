QT       += core gui opengl

TARGET = OpenGOOv2
TEMPLATE = app

LIBS=-lBox2D

OTHER_FILES += \
    l01.level \
    README

HEADERS += \
    tools.h \
    thorn.h \
    target.h \
    mainwidget.h \
    level.h \
    joint.h \
    ground.h \
    goo.h \
    fixedgoo.h \
    dynamicgoo.h \
    collisionlistener.h

SOURCES += \
    tools.cpp \
    thorn.cpp \
    target.cpp \
    mainwidget.cpp \
    main.cpp \
    level.cpp \
    joint.cpp \
    ground.cpp \
    goo.cpp \
    fixedgoo.cpp \
    dynamicgoo.cpp \
    collisionlistener.cpp



