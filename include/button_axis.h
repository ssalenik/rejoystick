#ifndef button_axis_h
#define button_axis_h

#include "js_button.h"
#include "js_axis.h"

/*
 * type: either SDL_JOYBUTTONDOWN or SDL_JOYAXISMOTION
 * button: corresponds to SDL_JOYBUTTONDOWN
 * axis: corresponds to SDL_JOYAXISMOTION */

typedef union {
	Uint8		type;
	int		keyboard_keycode;
	js_button	button;
	js_axis		axis;
} button_axis;

#endif

