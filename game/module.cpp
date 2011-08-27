#include "module.h"

#include "player/player.h"
#include "bullet/bullet.h"
#include "controller/controller.h"

struct ModuleDefinition ModuleData[] =
{
	{ &modPlayer },
	{ &modBullet },
	{ &modController },
	{ 0 },
};
