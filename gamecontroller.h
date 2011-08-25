#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H
#include<QtGui>
#include<stdlib.h>
#include<typeinfo>

#include "render.h"
#include "customlabel.h"
#include "gamebutton.h"

#define RATE 20

void something() {
	qDebug() << "works.";
}

class GameController {
public:
	QWidget *main;
	QTimer *glTimer, *camTimer, *timer;
        QHBoxLayout *lower, *upper;
        QVBoxLayout *iconHolder;
        QSpacerItem *space;
        QPixmap profiles[1];
	GameButton *label;
        CustomLabel *label2, *robot;
	Camera *cam;
#ifdef WIN32
	QPalette *p;
#endif
	GameController(QWidget *parent = 0) {

		main = new QWidget(parent);
		main->showFullScreen();
                main->resize(parent->geometry().width(), parent->geometry().height());
		cam = new Camera(0, 0, main->width(), main->height());
                MyGLDrawer *drawer = new MyGLDrawer(cam, main);

		glTimer = new QTimer(main);
		drawer->connect(glTimer, SIGNAL(timeout()), drawer, SLOT(redraw()));
		glTimer->start(0);

		camTimer = new QTimer(main);
                drawer->connect(camTimer, SIGNAL(timeout()), drawer, SLOT(moveMouseCheck()));
		camTimer->start(1./RATE);

                iconHolder  = new QVBoxLayout(main);
                lower = new QHBoxLayout();
                upper = new QHBoxLayout();
                iconHolder->insertLayout(0, lower);
                iconHolder->insertSpacing(1, -1);
                iconHolder->insertLayout(0, upper);
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

                label = new GameButton(0, main);
		label2 = new CustomLabel(&something, main);
		label->setPixmap(QPixmap(":/graphics/temp/temp.png"));
		label->resize(60,60);
		label2->setPixmap(QPixmap(":/graphics/temp/temp2.png"));
		label2->resize(60,60);
                profiles[0] = QPixmap(":/graphics/profiles/test.xcf");
		space = new QSpacerItem(main->width(), 0, QSizePolicy::Expanding);
		label->show();
		label2->show();
		lower->insertWidget(0, label);
		lower->insertWidget(1, label2);
		lower->insertSpacerItem(2, space);
                lower->setAlignment(Qt::AlignBottom);
                upper->setAlignment(Qt::AlignTop);
	}
	void showAll() {
		main->show();
		label->show();
		label2->show();
	}
	void hideAll() {
		main->hide();
		label->hide();
		label2->hide();
	}
};

#endif // GAMEWIDGET_H
