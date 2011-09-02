#ifndef MAIN_H
#define MAIN_H

#ifdef SCREENSAVER
	#include <gtk/gtk.h>
	#include <gtk/gtkgl.h>
	
	static gboolean deleteEvent(GtkWidget *widget, GdkEvent *event, gpointer data);
	static gboolean exposeCb(GtkWidget *drawingArea, GdkEventExpose *event, gpointer data);
	static gboolean idleCb(gpointer userData);
	static gboolean configureCb(GtkWidget *drawingArea, GdkEventConfigure *event, gpointer data);
	
	static void startEventLook();
	int initGtk(int argc, char *argv[]);
	
	#include "gs-theme-window.h"
#endif

#endif
