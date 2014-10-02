#include <gtk/gtk.h>
#include <SDL/SDL.h>
#include <X11/Xlib.h>

#include <assign_button.h>
#include <list.h>

extern SDL_Joystick* js;
extern int device_index;
extern list_t* button_list;
extern list_t* axis_list;

extern Display* dpy;


void init_sdl()
{
	/* Initialize SDL stuff */
	if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_JOYSTICK) < 0) {
                error("Failed to init sdl with joystick");
        }

        /* Register SDL_Quit as the exit function,
	   probably not neccesary since gtk will have stuff to say as well */
        if (atexit(SDL_Quit) != 0) {
                error("Failed to set SDL_Quit as exit function");
        }

        /* Enable joystick events */
        if (SDL_JoystickEventState(SDL_ENABLE) != SDL_ENABLE) {
                error("Failed to enable joystick events");
        }

        /* Open the device */
        if ((js = SDL_JoystickOpen(device_index)) == NULL) {
                error("Failed to open joystick device");
        }

	dpy = XOpenDisplay(0);
}


static gboolean button_keypress_callback(GtkWidget *widget, GdkEvent *event, gpointer data)
{
	assign_button_label* abuttonlabel = data;
	assign_button* abutton = abuttonlabel->abutton;;

	abutton->phys_button.keyboard_keycode = event->key.hardware_keycode;


	gtk_label_set_text(abuttonlabel->label, XKeysymToString(XKeycodeToKeysym(dpy, abutton->phys_button.keyboard_keycode,0)));

	return FALSE;
}

void assign_key_callback(GtkWidget *widget, gpointer data)
{
	GtkWidget* window;
	GtkWidget* button;	/* Looks better with a label but had problem with event name */

	assign_button_label* abuttonlabel = data;

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	button = gtk_button_new_with_label("Press a keyboard key");

	gtk_container_add(GTK_CONTAINER(window), button);

	g_signal_connect (G_OBJECT(button), "key_press_event", 
			G_CALLBACK (button_keypress_callback), abuttonlabel);

	g_signal_connect_swapped (G_OBJECT(button), "key_press_event", 
			G_CALLBACK (gtk_widget_destroy), window);


	gtk_widget_show(button);
	gtk_widget_show(window);
}

void button_axis_callback(GtkWidget* widget, gpointer data)
{
	GtkWidget* window;
	GtkWidget* vbox;
	GtkWidget* label;
	GtkWidget* label_keycode;
	GtkWidget* read_button; /* Click to assign a new key for the "key" */
	GtkWidget* close_button;

	assign_button_label* abuttonlabel = malloc(sizeof(assign_button_label));
	abuttonlabel->abutton = (assign_button*)data;

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), gtk_button_get_label((GtkButton*)abuttonlabel->abutton->graph_button));
	gtk_container_set_border_width(GTK_CONTAINER(window), 10);
	vbox = gtk_vbox_new(FALSE,10);

	label = gtk_label_new("Current button/keycode: ");
	label_keycode = gtk_label_new(XKeysymToString(XKeycodeToKeysym(dpy, abuttonlabel->abutton->phys_button.keyboard_keycode,0)));
	read_button = gtk_button_new_with_label("Change key");
	close_button = gtk_button_new_with_label("Close");

	abuttonlabel->label = (GtkLabel*)label_keycode;


	/* Signal connections */

	g_signal_connect(G_OBJECT(read_button), "clicked", 
			G_CALLBACK(assign_key_callback), (gpointer)abuttonlabel);

	g_signal_connect_swapped (G_OBJECT (close_button), "clicked",
			G_CALLBACK (gtk_widget_destroy), G_OBJECT (window));

	/* Design stuff */

	gtk_box_pack_start(GTK_BOX(vbox), label, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(vbox), label_keycode, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(vbox), read_button, TRUE, FALSE, 10);
	gtk_box_pack_start(GTK_BOX(vbox), close_button, TRUE, TRUE, 0);

	gtk_container_add(GTK_CONTAINER(window), vbox);

	gtk_widget_show(label);
	gtk_widget_show(label_keycode);
	gtk_widget_show(read_button);
	gtk_widget_show(close_button);
	gtk_widget_show(vbox);
	gtk_widget_show(window);
}



/*void button_axis_callback(GtkWidget *widget, gpointer data)
{
	assign_button* button = data;
	button_function(button);
}*/

gboolean delete_event(GtkWidget *widget, GdkEvent *event, gpointer data)
{
	gtk_main_quit();
	return FALSE;
}


void create_buttons(GtkWidget* box)
{
	assign_button*	abutton;
	list_t*		it;
	int 		nbr_buttons;
	int 		i;
	char*		label;

	label = malloc(sizeof(char)*20);
	nbr_buttons = SDL_JoystickNumButtons(js);
	it = button_list;
	/*nbr_buttons = 10;*/

	i = 0;
	if (it->data != it) {
		it = it->next;
		while (it != NULL) {
			abutton = it->data;
			sprintf(label, "Button %i", i);
			abutton->graph_button = gtk_button_new_with_label(label);

			g_signal_connect (G_OBJECT (abutton->graph_button), "clicked", 
					G_CALLBACK (button_axis_callback), (gpointer)abutton);
			gtk_box_pack_start(GTK_BOX(box), abutton->graph_button, TRUE, TRUE, 0);
			gtk_widget_show(abutton->graph_button);	
			i++;
			it = it->next;
		}

	}

	free(label);
}

void create_axis(GtkWidget* box)
{
	GtkWidget*	hbox;
	assign_button*	button;
	list_t*		it;
	int 		nbr_axis;
	int 		i;
	char*		label;

	label = malloc(sizeof(char)*20);

	nbr_axis = SDL_JoystickNumAxes(js);
	/*nbr_axis = 6;*/

	it = axis_list;
	i = 0;	

	if (it->data != it) {
		it = it->next;
		while (it != NULL) {
			button = it->data;
			hbox = gtk_hbox_new(FALSE, 0);
			gtk_box_pack_start(GTK_BOX(box), hbox, TRUE, TRUE, 0);
			/* The "negative axis button" */

			sprintf(label, "Axis %i (neg)", i);
			button->graph_button = gtk_button_new_with_label(label);

			g_signal_connect (G_OBJECT (button->graph_button), "clicked", 
					G_CALLBACK (button_axis_callback), (gpointer)button);	
			gtk_widget_show(button->graph_button);
			gtk_box_pack_start(GTK_BOX(hbox), button->graph_button, TRUE, TRUE, 0);

			/* The "positive axis button" */
			i++;
			it = it->next;
			button = it->data;

			sprintf(label, "Axis %i (pos)", i);
			button->graph_button = gtk_button_new_with_label(label);
			
			g_signal_connect (G_OBJECT (button->graph_button), "clicked", 
					G_CALLBACK (button_axis_callback), (gpointer)button);	
			gtk_widget_show(button->graph_button);


			gtk_box_pack_start(GTK_BOX(hbox), button->graph_button, TRUE, TRUE, 0);
			gtk_widget_show(hbox);
			
			i++;
			it = it->next;

		}

	}


	free(label);
}


