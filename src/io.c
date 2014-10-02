#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <X11/Xlib.h>
#include "assign_button.h"
#include "io.h"
#include "list.h"


extern list_t*		button_list;
extern list_t*		axis_list;
extern Display*		dpy;
extern SDL_Joystick*	js;
extern int		device_index;
extern char*		filename;
extern char**		global_argv;

static void print_axis(assign_button* abutton)
{
	g_print("type: %i, ", abutton->phys_button.axis.event.type);
	g_print("which: %i, ", abutton->phys_button.axis.event.which);
	g_print("axis: %i, ", abutton->phys_button.axis.event.axis);
	g_print("value: %i, ", abutton->phys_button.axis.event.value);
	g_print("key: %s\n", XKeysymToString(XKeycodeToKeysym(dpy, abutton->phys_button.keyboard_keycode,0)));
}

static void print_button(assign_button* abutton)
{
	g_print("type: %i, ", abutton->phys_button.button.event.type);
	g_print("which: %i, ", abutton->phys_button.button.event.which);
	g_print("button: %i, ", abutton->phys_button.button.event.button);
	g_print("state: %i, ", abutton->phys_button.button.event.state);
	g_print("key: %s\n", XKeysymToString(XKeycodeToKeysym(dpy, abutton->phys_button.keyboard_keycode,0)));
}

static void fprint_axis(FILE* fp, assign_button* abutton)
{
	fprintf(fp, "jaxis ");
	fprintf(fp, "type: %i ", abutton->phys_button.axis.event.type);
	fprintf(fp, "which: %i ", abutton->phys_button.axis.event.which);
	fprintf(fp, "axis: %i ", abutton->phys_button.axis.event.axis);
	fprintf(fp, "value: %i ", abutton->phys_button.axis.event.value);
	fprintf(fp, "limit: %i ", abutton->phys_button.axis.limit);
	/*fprintf(fp, "key: %s\n", XKeysymToString(XKeycodeToKeysym(dpy, abutton->phys_button.keyboard_keycode,0)));*/
	fprintf(fp, "key: %i\n", abutton->phys_button.keyboard_keycode);
}

static void fprint_button(FILE* fp, assign_button* abutton)
{
	fprintf(fp, "jbutton ");
	fprintf(fp, "type: %i ", abutton->phys_button.button.event.type);
	fprintf(fp, "which: %i ", abutton->phys_button.button.event.which);
	fprintf(fp, "button: %i ", abutton->phys_button.button.event.button);
	fprintf(fp, "state: %i ", abutton->phys_button.button.event.state);
	/*fprintf(fp, "key: %s\n", XKeysymToString(XKeycodeToKeysym(dpy, abutton->phys_button.keyboard_keycode,0)));*/
	fprintf(fp, "key: %i\n", abutton->phys_button.keyboard_keycode);
}



/* void save_keys(char* filename) */
gboolean save_keys(GtkWidget* widget, gpointer data)
{
	FILE* fp = fopen(filename, "w");
	if (fp < 0) {
		error("Failed to to open file");
	}


	list_t* it;
	assign_button* abutton;

	it = button_list;

	if (it->data != it) {
		it = it->next;
		while (it != NULL) {
			abutton = it->data;
			fprint_button(fp, abutton);
			it = it->next;
		}
	}

	it = axis_list;

	if (it->data != it) {
		it = it->next;
		while (it != NULL) {
			abutton = it->data;
			fprint_axis(fp, abutton);
			it = it->next;
		}
	}
	fclose(fp);

		

	/* The following does not work, the GUI remains and hangs... lame */

	/*int fork_return;

	gtk_main_quit();
	SDL_Quit();
	fork_return = fork();
	
	if (fork_return < 0) {
		error("Could not create child process");
	}
	if (fork_return > 0) {
	
	} else {
		init_sdl();
		loop_and_wait();	

		g_print("execv(global_argv[0], \"-d\");\n");
		g_print("execv(%s, \"-d\");\n", global_argv[0]);
	}*/

	
	
	return FALSE;
}

void load_keys()
{

	assign_button*	abutton;
	FILE*		fp; 
	char*		str;
	int		i;
	
	button_list = list_new();
	axis_list = list_new();

	/* If the file doesn't exist we have to fill the button_list and axis_list manually */
	if (access(filename, F_OK != 0)) {
		g_print("No file to load, creating\n");
		/* Fill button_list... */
		int nbr_buttons = SDL_JoystickNumButtons(js);		

		for (i = 0; i < nbr_buttons; i++) {
			abutton = malloc(sizeof(assign_button));
			list_add(button_list, abutton);
			abutton->phys_button.type = SDL_JOYBUTTONDOWN;
                	abutton->phys_button.keyboard_keycode = 0;

                	abutton->phys_button.button.event.type = SDL_JOYBUTTONDOWN;
                	abutton->phys_button.button.event.which = device_index;
                	abutton->phys_button.button.event.button = i;
                	abutton->phys_button.button.event.state = SDL_PRESSED; /* Won't really matter though */
		}

		int nbr_axis = SDL_JoystickNumAxes(js);
		/* Fill axis_list... */

		for (i = 0; i < nbr_axis; i++) {
			abutton = malloc(sizeof(assign_button));
			list_add(axis_list, abutton);
			abutton->phys_button.type = SDL_JOYAXISMOTION;
                	abutton->phys_button.keyboard_keycode = 0;
                	
			abutton->phys_button.axis.limit = 10000;
                	abutton->phys_button.axis.event.type = SDL_JOYAXISMOTION;
                	abutton->phys_button.axis.event.which = device_index;
                	abutton->phys_button.axis.event.axis = i;
                	abutton->phys_button.axis.event.value = 1;
			
			abutton = malloc(sizeof(assign_button));
			list_add(axis_list, abutton);
			abutton->phys_button.type = SDL_JOYAXISMOTION;
                	abutton->phys_button.keyboard_keycode = 0;
                	
			abutton->phys_button.axis.limit = 10000;
                	abutton->phys_button.axis.event.type = SDL_JOYAXISMOTION;
                	abutton->phys_button.axis.event.which = device_index;
                	abutton->phys_button.axis.event.axis = i;
                	abutton->phys_button.axis.event.value = -1;
		}

	} else {
		fp = fopen(filename, "r");
		if (fp < 0) {
			error("Failed to to open file");
		}

		str = malloc(sizeof(char)*10);

		while (1) {
			if (fscanf(fp, "%s", str) == EOF) {
				break;
			}
			abutton = malloc(sizeof(assign_button));
			if (strcmp(str, "jbutton") == 0) {
				list_add(button_list, abutton);
				fscanf(fp, "%s %i %s %i %s %i %s %i %s %i", 
						str, &abutton->phys_button.button.event.type, 
						str, &abutton->phys_button.button.event.which, 
						str, &abutton->phys_button.button.event.button, 
						str, &abutton->phys_button.button.event.state, 
						str, &abutton->phys_button.keyboard_keycode);
			} else if (strcmp(str, "jaxis") == 0) {
				list_add(axis_list, abutton);
				fscanf(fp, "%s %i %s %i %s %i %s %i %s %i %s %i", 
						str, &abutton->phys_button.axis.event.type, 
						str, &abutton->phys_button.axis.event.which, 
						str, &abutton->phys_button.axis.event.axis, 
						str, &abutton->phys_button.axis.event.value, 
						str, &abutton->phys_button.axis.limit, 
						str, &abutton->phys_button.keyboard_keycode);
			}
		}
	}
}

void print_keys(GtkWidget* widget, gpointer data)
{

	list_t* it;
	assign_button* abutton;

	it = button_list;

	if (it->data != it) {
		it = it->next;
		g_print("----- Buttons ------\n");
		while (it != NULL) {
			abutton = it->data;
			print_button(abutton);
			it = it->next;
		}
	}

	it = axis_list;

	if (it->data != it) {
		it = it->next;
		g_print("----- Axis ------\n");
		while (it != NULL) {
			abutton = it->data;
			print_axis(abutton);
			it = it->next;
		}
	}
	/*save_keys();*/
}

