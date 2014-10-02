#ifndef js_button_h
#define js_button_h

#include <SDL/SDL.h>

typedef struct js_button js_button;

/*
 * type: SDL_JOYBUTTONDOWN (have to choose something, DOWN would work to)
 * keyboard_keycode: the key that should be generated
 * event: what josytick button is it?
 * */

struct js_button {
	Uint8			type;
	int			keyboard_keycode;
	SDL_JoyButtonEvent	event;
};


#endif
