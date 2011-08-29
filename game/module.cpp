#include "module.h"

#include "player/player.h"
#include "bullet/bullet.h"
#include "controller/controller.h"
#include "item/item.h"

struct ModuleDefinition ModuleData[] =
{
	{ &modPlayer },
	{ &modBullet },
	{ &modController },
	{ &modItem},
	{ 0 },
};
