#include <QtGui>
#include <QWidget>
#include <QAbstractButton>
#include <QIcon>
#include <QDebug>
#include <QTimer>

#include "main.h"
#include "gamecontroller.h"
#include "game/signal.h"

void startup() {
	modSignal.startup();
}

void shutdown() {
	modSignal.shutdown();
}

void run(int argc, char *argv[]) {
	QApplication app(argc, argv);
	GameController *test = new GameController();
	app.exec();
	delete test;
}

int main(int argc, char *argv[]) {
	startup();
		run(argc, argv);
	shutdown();
	
	return 0;
}
