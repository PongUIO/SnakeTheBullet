#include "module.h"

#include "player/player.h"
#include "bullet/bullet.h"

struct ModuleDefinition ModuleData[] =
{
	{ &modPlayer },
	{ &modBullet },
	{ 0 },
};
