#-------------------------------------------------
#
# Project created by QtCreator 2011-08-25T19:01:35
#
#-------------------------------------------------

CONFIG += debug
QT       += core gui opengl

TARGET = SnakeTheBullet
TEMPLATE = app


SOURCES += main.cpp \
	game/game.cpp \
	game/module.cpp \
	game/signal.cpp

HEADERS  += render.h \
    camera.h \
    menubutton.h \
    gamecontroller.h \
    gamebutton.h \
    customlabel.h \
    main.h \
    game/game.h \
    game/module.h \
    game/signal.h

RESOURCES += \
	resources.qrc

OTHER_FILES += \
    graphics/startgame.xcf
