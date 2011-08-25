#ifndef GAMEBUTTON_H
#define GAMEBUTTON_H
#include<QtGui>
class GameButton : public QLabel {
	Q_OBJECT        // must include this if you use Qt signals/slots
public:
        int func;
        GameButton(int func, QWidget *parent = 0)
		: QLabel(parent) {
		this->func = func;
        }
protected:
	// overridden
        void mousePressEvent(QMouseEvent * event) {
                if (func == 0) {
                }
	}
};
#endif
