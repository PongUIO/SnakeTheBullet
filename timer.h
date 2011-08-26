#ifndef TIMER_H
#define TIMER_H

#include <QObject>
#include "game/signal.h"

#include "input.h"

class Timer : public QObject {
	Q_OBJECT
	
	public slots:
		void process() {
			mIn.frame_reset();
			mIn.update();
			modSignal.input();
			modSignal.process(0.01);
		}
	
	public:
		Timer(QWidget *parent = 0) : QObject(0) {
		}
		
		
	private:
		
};

#endif
