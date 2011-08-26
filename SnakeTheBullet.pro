#-------------------------------------------------
#
# Project created by QtCreator 2011-08-25T19:01:35
#
#-------------------------------------------------

CONFIG += debug
QT       += core gui opengl

TARGET = SnakeTheBullet
TEMPLATE = app

LIBS += -lOIS


SOURCES += main.cpp \
	input.cpp \
	game/game.cpp \
	game/module.cpp \
	game/signal.cpp \
	game/player/player.cpp

HEADERS  += render.h \
    gamecontroller.h \
    customlabel.h \
    main.h \
    input.h \
    timer.h \
    game/game.h \
    game/module.h \
    game/signal.h \
    game/player/player.h

RESOURCES += \
	resources.qrc

OTHER_FILES += \
    graphics/startgame.xcf
