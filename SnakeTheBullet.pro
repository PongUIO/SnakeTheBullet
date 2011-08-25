#-------------------------------------------------
#
# Project created by QtCreator 2011-08-25T19:01:35
#
#-------------------------------------------------

CONFIG += debug
QT       += core gui opengl object

TARGET = SnakeTheBullet
TEMPLATE = app


SOURCES += main.cpp \
	game/game.cpp \
	game/module.cpp \
	game/signal.cpp \
	game/player/player.cpp

HEADERS  += render.h \
    camera.h \
    menubutton.h \
    gamecontroller.h \
    gamebutton.h \
    customlabel.h \
    main.h \
    timer.h \
    game/game.h \
    game/module.h \
    game/signal.h \
    game/player/player.h

RESOURCES += \
	resources.qrc

OTHER_FILES += \
    graphics/startgame.xcf
