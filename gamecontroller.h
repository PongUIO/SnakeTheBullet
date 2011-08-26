#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H
#include<QtGui>
#include <QObject>
#include <QMainWindow>
#include<stdlib.h>
#include<typeinfo>

#include "render.h"
#include "timer.h"
#include "input.h"

#define LOGIC_RATE 100
#define RENDER_RATE 60

class GameController {
public:
	QMainWindow *main;
	QTimer *drawTimer, *logicTimer;
	Timer *timerObj;
#ifdef WIN32
	QPalette *p;
#endif
	
	GameController() {
		main = new QMainWindow();
		main->resize(700,700);
		main->show();
		
		MyGLDrawer *drawer = new MyGLDrawer(main);
		timerObj = new Timer(main);
		
		mIn.init(drawer);
		
		drawTimer = new QTimer(main);
		drawer->connect(drawTimer, SIGNAL(timeout()), drawer, SLOT(redraw()));
		drawTimer->start(1000/RENDER_RATE);
		
		logicTimer = new QTimer(main);
		timerObj->connect(logicTimer, SIGNAL(timeout()), timerObj, SLOT(process()));
		logicTimer->start(1000/LOGIC_RATE);
		
		main->setWindowTitle(QApplication::translate("childwidget", "Child widget"));
		drawer->setMouseTracking(true);

#ifdef _WIN32
		drawer->resize(main->width(), main->height()-1);
		p = new QPalette( main->palette() );
		p->setColor( QPalette::Window, Qt::black );
		main->setPalette( *p );
#else
		drawer->resize(main->width(), main->height());
#endif
		drawer->show();
	}
	
	~GameController() {
		mIn.shutdown();
	}
	
	void showAll() {
		main->show();
	}
	void hideAll() {
		main->hide();
	}
};

#endif // GAMEWIDGET_H
