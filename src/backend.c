#include <gtk/gtk.h>
#include <SDL/SDL.h>
#include <X11/extensions/XTest.h>

#include <button_axis.h>
#include <assign_button.h>
#include <backend.h>
#include <list.h>

extern list_t*	button_list;
extern list_t*	axis_list;
extern Display*	dpy;

static gboolean button_equals(SDL_JoyButtonEvent* e1, SDL_JoyButtonEvent* e2)
{
	return (e1->which == e2->which && e1->button == e2->button);
}

static gboolean axis_equals(SDL_JoyAxisEvent* e1, SDL_JoyAxisEvent* e2)
{
	return (e1->which == e2->which && e1->axis == e2->axis && e1->value*e2->value >= 0);
}


static button_axis* find_axis_event(SDL_JoyAxisEvent* event, gboolean return_first)
{
	list_t* 	it;
	assign_button*	abutton;
	assign_button*	tmp;

	it = axis_list;

	if (it->data != it) {
		it = it->next;
		while (it != NULL) {
			abutton = it->data;
			if (axis_equals(event, &abutton->phys_button.axis.event)) {
				if (return_first) {
					return &abutton->phys_button;
				} else {
					tmp = abutton;
					return_first = TRUE; /* Så blri det den andra nästa gång*/
				}
			} 
			it = it->next;
		}
	}
	return &tmp->phys_button;
}

static button_axis* find_button_event(SDL_JoyButtonEvent* event)
{
	list_t* 	it;
	assign_button*	abutton;

	it = button_list;

	if (it->data != it) {
		it = it->next;
		while (it != NULL) {
			abutton = it->data;
			if (button_equals(event, &abutton->phys_button.button.event)) {
				return &abutton->phys_button;
			} else {
				it = it->next;
			}
		}
	}

}

void loop_and_wait()
{
	SDL_Event	key;
	button_axis* 	ba;
	button_axis*	ba2;
	
	/* Read away initial output crap */
	while(SDL_PollEvent(&key));
	
	while (1) {
		if (SDL_WaitEvent(&key) == 0) {
			error("Failed to read from device");
		} else {
			if (key.type == SDL_JOYBUTTONDOWN || key.type == SDL_JOYBUTTONUP) {
				ba = find_button_event(&key.jbutton);

				if (ba->keyboard_keycode != 0) {
					XTestFakeKeyEvent(dpy, ba->keyboard_keycode, (key.type == SDL_JOYBUTTONDOWN), 0);
					XFlush(dpy);
				}
							}
			
			if (key.type == SDL_JOYAXISMOTION) {
				ba = find_axis_event(&key.jaxis, TRUE);
				
				if (ba->keyboard_keycode != 0) {
					if (abs(key.jaxis.value) > ba->axis.limit) {
						/* Press "zone" */
						if (abs(ba->axis.event.value) == 1) {
							ba->axis.event.value = ba->axis.event.value*2;
							XTestFakeKeyEvent(dpy, ba->keyboard_keycode, 1, 0);
						}
					} else {
						/* Detach "zone" */
						if (abs(ba->axis.event.value) == 2) {
							ba->axis.event.value = ba->axis.event.value/2;
							XTestFakeKeyEvent(dpy, ba->keyboard_keycode, 0, 0);
						} else {
							/* If value == 0... can be 2 different possibillities
							   therefore we will not take the first occurance */
							ba = find_axis_event(&key.jaxis, FALSE);
							if (abs(ba->axis.event.value) == 2) {
								ba->axis.event.value = ba->axis.event.value/2;
								XTestFakeKeyEvent(dpy, ba->keyboard_keycode, 0, 0);
							}
						}
					}
				}
				XFlush(dpy);
			}
		}
	}
}
