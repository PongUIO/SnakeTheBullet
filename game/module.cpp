#include "module.h"

#include "player/player.h"
#include "bullet/bullet.h"
#include "controller/controller.h"
#include "item/item.h"

struct ModuleDefinition ModuleData[] =
{
#ifdef USE_PLAYERITEM
	{ &modPlayer },
#endif
	{ &modBullet },
	{ &modController },
#ifdef USE_PLAYERITEM
	{ &modItem},
#endif
	{ 0 },
};
