

#ifndef INPUT_H
#define INPUT_H

#include "types.h"

//Handles: input buffer per frame
void input_handle(GameState *gs);


//Handles: word submission per spacebar pressed. 
void input_try_submit(GameState *gs);

#endif 
