#include "g_time.h"


Uint32 lastTime = 0;
Uint32 deltaTime = 0;

void init_time()
{
	lastTime = SDL_GetTicks();
}

void update_time()
{
	deltaTime = SDL_GetTicks() - lastTime;
	lastTime = SDL_GetTicks();
}

Uint32 get_current_time()
{
	return SDL_GetTicks();
}

Uint32 get_delta_time()
{
	return deltaTime;
}
