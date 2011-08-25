#include <QDebug>

#include "player.h"

class mdPlayer modPlayer;

void mdPlayer::process(double delta)
{
	qDebug() << "process\n";
}

void mdPlayer::draw()
{
	qDebug() << "draw\n";
}
