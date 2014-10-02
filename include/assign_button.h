#ifndef assign_button_h
#define assign_button_h

#include "button_axis.h"
#include <gtk/gtk.h>

typedef struct assign_button assign_button;

struct assign_button {
	GtkWidget*	graph_button;
	button_axis	phys_button;
};

typedef struct assign_button_label assign_button_label;

/* Super ugly with this struct, but hey what are you gonna do? */
struct assign_button_label {
	GtkLabel*	label;
	assign_button*	abutton;
};

#endif
