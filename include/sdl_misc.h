#ifndef sdl_misc_h
#define sdl_misc_h

/* Might as well do them global */
SDL_Joystick *js;
int device_index = 0;

void init_sdl();
void button_axis_callback(GtkWidget *widget, gpointer data);
gboolean delete_event(GtkWidget *widget, GdkEvent *event, gpointer data);
void create_buttons(GtkWidget* box);
void create_axis(GtkWidget* box);

#endif
