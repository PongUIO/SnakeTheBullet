#ifndef TIMER_H
#define TIMER_H

#include <QObject>
#include "game/signal.h"

class Timer : public QObject {
	Q_OBJECT
	
	public slots:
		void draw() {
			modSignal.draw();
		}
		
		void process() {
			modSignal.process(0.01);
		}
	
	public:
		Timer(QWidget *parent = 0) : QObject(0) {
		}
		
		
	private:
		
};

#endif
