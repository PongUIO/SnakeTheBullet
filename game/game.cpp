#include "game.h"
#include "signal.h"

class mdGame modGame;

void mdGame::runFrame(double delta)
{
	modSignal.process(delta);
	modSignal.draw();
}
