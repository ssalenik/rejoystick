#ifndef js_axis_h
#define js_axis_h

#include <SDL/SDL.h>

typedef struct js_axis js_axis;

/* 
 * type: SDL_JOYAXISMOTION 
 * keyboard_keycode: the key that should be generated
 * limit: (absolute) threshold value until the "axis key" is pressed 
 * event: describes which axis and so on. If event->value > 0 then it is the positive axis 
 * */

struct js_axis {
	Uint8			type;
	int			keyboard_keycode;
	unsigned int		limit;		/* When is the "button" pressed/released, which value? */
	SDL_JoyAxisEvent	event;		/* Axis movement */
};


#endif
