#include <gtk/gtk.h>
#include <SDL/SDL.h>
#include <X11/Xlib.h>
#include <gdk/gdk.h>
#include <glib.h>
#include <pthread.h>
#include <unistd.h>

#include <assign_button.h>
#include <button_axis.h>
#include <sdl_misc.h>
#include <list.h>
#include <io.h>
#include <backend.h>


#define		KEYFILE "/.rejoystickrc"
list_t* 	button_list;
list_t* 	axis_list;
Display* 	dpy;
char*		filename;
char**		global_argv;


gboolean quit_callback(GtkWidget* widget, GdkEvent* event, gpointer data)
{
	gtk_main_quit();	

	return TRUE;
}

static void handle_event(SDL_Event* key)
{	assign_button* abutton;
	list_t* it;

	GdkColor color;
	GdkColormap* colormap;
	gdk_color_parse ("red", &color);

	switch (key->type) {
		case SDL_JOYBUTTONDOWN:
			it = button_list;
			if (it->data != it) {
				it = it->next;
			}
			while (it != NULL) {
				abutton = it->data;
				/* Perhaps unnecessary, but what the heck! */
				if (abutton->phys_button.button.event.type == SDL_JOYBUTTONDOWN) {
					if (abutton->phys_button.button.event.button == key->jbutton.button) {
						gdk_threads_enter();
						abutton->phys_button.button.event.state = SDL_PRESSED;
						gtk_widget_modify_bg(abutton->graph_button, 0, &color);
						gdk_threads_leave();
						break;
					}
				}
				it = it->next;
			}
			break;
		case SDL_JOYBUTTONUP:
			it = button_list;
			if (it->data != it) {
				it = it->next;
			}
			while (it != NULL) {
				abutton = it->data;
				/* Perhaps unnecessary, but what the heck! */
				if (abutton->phys_button.button.event.type == SDL_JOYBUTTONDOWN) {
					if (abutton->phys_button.button.event.button == key->jbutton.button) {
						gdk_threads_enter();
						abutton->phys_button.button.event.state = SDL_RELEASED;
						gtk_widget_modify_bg(abutton->graph_button, 0, NULL);
						gdk_threads_leave();
						break;
					}
				}
				it = it->next;
			}
			break;
		case SDL_JOYAXISMOTION:
			it = axis_list;
			if (it->data != it) {
				it = it->next;
			}
			while (it != NULL) {
				abutton = it->data;
				if (abutton != NULL && abutton->phys_button.axis.event.axis == key->jaxis.axis) {	
					if (abutton->phys_button.axis.event.value*key->jaxis.value > 0) {
						gdk_threads_enter();
						gtk_widget_modify_bg((GtkWidget*)(abutton->graph_button), 0, &color);
						gdk_threads_leave();
					} else if (key->jaxis.value == 0) {
						gdk_threads_enter();	
						gtk_widget_modify_bg(abutton->graph_button, 0, NULL);
						gdk_threads_leave();
					}
				}
				it = it->next;
			}

			break;
		default:
			break;
	}
}

static void joystick_button_check(void* data)
{
	SDL_Event key;

	
        /* Read away junk */
        while(SDL_PollEvent(&key));

        while (TRUE) {
                if (SDL_WaitEvent(&key) == 0) {
                        error("Failed to read from device");
                } else {
			handle_event(&key);
			usleep(1);
		}
        }
}

int main(int argc, char* argv[])
{
	GtkWidget* 	window;
	GtkWidget* 	hbox;
	GtkWidget* 	vbox;
	GtkWidget* 	bottom_hbox;
        GtkWidget* 	button_box;
        GtkWidget* 	axis_box;

	GtkWidget* 	ok_button;
	GtkWidget* 	cancel_button;

	pthread_t	sdl_thread;

	char*		home;
	char*		keyfile;

	home = (char*)getenv("HOME");
	keyfile = KEYFILE;
	filename = strcat(home, keyfile);

	global_argv = argv;


	if (argc > 1) {
		if (!strcmp(argv[1], "-d")) {
			int forkreturn;

			forkreturn = fork();
			if (forkreturn < 0) {
				error("Could not create child process");
			}
			if (forkreturn > 0) {
				return 1;
			} else {
				init_sdl();	
				load_keys();
				loop_and_wait();
				/* We should never get out of this loop */
				return 0;
			}
		}
	}
	init_sdl();	

	/* init threads */
	g_thread_init(NULL);
	gdk_threads_init();
	gdk_threads_enter();


        gtk_init(&argc, &argv);

        window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	

        gtk_window_set_title(GTK_WINDOW(window), "Rejoystick");
        gtk_container_set_border_width(GTK_CONTAINER(window), 10);

	ok_button = gtk_button_new_with_label("OK");
	cancel_button = gtk_button_new_with_label("Cancel");

	hbox = gtk_hbox_new(FALSE, 10);
	vbox = gtk_vbox_new(FALSE, 10);
	bottom_hbox = gtk_hbox_new(FALSE, 10);
        button_box = gtk_vbox_new(FALSE, 0);
        axis_box = gtk_vbox_new(FALSE, 0);

	gtk_container_add(GTK_CONTAINER(hbox), button_box);
	gtk_container_add(GTK_CONTAINER(hbox), axis_box);
	
	gtk_container_add(GTK_CONTAINER(vbox), hbox);


	gtk_container_add(GTK_CONTAINER(bottom_hbox), ok_button);
	gtk_container_add(GTK_CONTAINER(bottom_hbox), cancel_button);

	gtk_container_add(GTK_CONTAINER(vbox), bottom_hbox);
        

	gtk_container_add(GTK_CONTAINER(window), vbox);

	load_keys();

	create_buttons(button_box);
	create_axis(axis_box);

	gtk_widget_show(ok_button);
	gtk_widget_show(cancel_button);
	gtk_widget_show(button_box);
	gtk_widget_show(axis_box);
	gtk_widget_show(hbox);
	gtk_widget_show(vbox);
	gtk_widget_show(bottom_hbox);

        gtk_widget_show(window);


	g_signal_connect(G_OBJECT(window), "delete_event", G_CALLBACK(delete_event), NULL);
	g_signal_connect(G_OBJECT(cancel_button), "clicked", G_CALLBACK(delete_event), NULL);
	g_signal_connect(G_OBJECT(ok_button), "clicked", G_CALLBACK(save_keys), NULL);
	g_signal_connect(G_OBJECT(ok_button), "clicked", G_CALLBACK(quit_callback), NULL);


	pthread_create(&sdl_thread, NULL, (void*)joystick_button_check, NULL);


        gtk_main();
	gdk_threads_leave();

	return 0;
}
