#ifndef __TIME_H__
#define __TIME_H__

#include "gfc_types.h"




void init_time();

void update_time(); //call at end of game loop to update for delta time functions

Uint32 get_current_time(); //returned in ms since game started

Uint32 get_delta_time(); //returned in ms since last frame

#endif


