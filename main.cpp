#include <SDL.h>
#include <GL/gl.h>

#include "main.h"

#include "game/signal.h"
#include "game/game.h"
#include "game/timer.h"

void init_GL(int w,int h) {
	modGame.updateScreen(w,h);
}

#ifdef SCREENSAVER
int initGtk(int argc, char *argv[])
{
	GtkWidget *window;
	GtkWidget *drawingArea;
	
	gtk_init(&argc, &argv);
	
	gboolean fullscreen=FALSE;
	for(int i=1; i<argc; i++) {
		if(strcmp("-root",argv[i])==0)
			fullscreen=TRUE;
	}
	
	window = gs_theme_window_new();
	
	GdkScreen *screen = gtk_window_get_screen(GTK_WINDOW(window));
	int width = gdk_screen_get_width(screen);
	int height = gdk_screen_get_height(screen);
	gtk_window_set_default_size(GTK_WINDOW(window), width/2, height/2);
	if(fullscreen)
		gtk_window_fullscreen(GTK_WINDOW(window));
	
	drawingArea = gtk_drawing_area_new();
	gtk_container_add(GTK_CONTAINER(window), drawingArea);
	
	g_signal_connect_swapped(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
	g_signal_connect(G_OBJECT(window), "delete-event", G_CALLBACK(deleteEvent), NULL);
	gtk_widget_set_events(drawingArea, GDK_EXPOSURE_MASK);
	
	GdkGLConfig *glConfig;
	gtk_gl_init(&argc, &argv);
	
	glConfig = gdk_gl_config_new_by_mode( GdkGLConfigMode(GDK_GL_MODE_RGBA | GDK_GL_MODE_DEPTH | GDK_GL_MODE_DOUBLE) );
	if(!glConfig)
		g_assert_not_reached();
	if(!gtk_widget_set_gl_capability(drawingArea, glConfig, NULL, TRUE, GDK_GL_RGBA_TYPE) )
		g_assert_not_reached();
	
	g_signal_connect(drawingArea, "configure-event", G_CALLBACK(configureCb), NULL);
	g_signal_connect(drawingArea, "expose-event", G_CALLBACK(exposeCb), NULL);
	
#ifdef FPS_DEF
	const gdouble TIMEOUT_PERIOD = 1000 / double(FPS_DEF);
#else
	const gdouble TIMEOUT_PERIOD = 1000 / double(100.0);
#endif
	g_timeout_add(TIMEOUT_PERIOD, idleCb, drawingArea);
	
	gtk_widget_show_all(window);
	return 1;
}

gboolean deleteEvent(GtkWidget* widget, GdkEvent* event, gpointer data)
{
	return FALSE;
}

gboolean configureCb(GtkWidget* drawingArea, GdkEventConfigure* event, gpointer data)
{
	GdkGLContext *glContext = gtk_widget_get_gl_context(drawingArea);
	GdkGLDrawable *glDrawable = gtk_widget_get_gl_drawable(drawingArea);
	
	if(!gdk_gl_drawable_gl_begin(glDrawable, glContext) )
		g_assert_not_reached();
	
	glViewport(0,0, drawingArea->allocation.width, drawingArea->allocation.height);
	init_GL(drawingArea->allocation.width, drawingArea->allocation.height);
	
	gdk_gl_drawable_gl_end(glDrawable);
	
	return TRUE;
}

gboolean exposeCb(GtkWidget* drawingArea, GdkEventExpose* event, gpointer data)
{
	GdkGLContext *glContext = gtk_widget_get_gl_context(drawingArea);
	GdkGLDrawable *glDrawable = gtk_widget_get_gl_drawable(drawingArea);
	
	if( !gdk_gl_drawable_gl_begin(glDrawable, glContext) )
		g_assert_not_reached();
	
	modGame.runFrame();
	
	if( gdk_gl_drawable_is_double_buffered(glDrawable))
		gdk_gl_drawable_swap_buffers(glDrawable);
	else
		glFlush();
	
	gdk_gl_drawable_gl_end(glDrawable);
	
	return TRUE;
}

gboolean idleCb(gpointer userData)
{
	GtkWidget *drawingArea = GTK_WIDGET(userData);
	
	gdk_window_invalidate_rect(drawingArea->window, &drawingArea->allocation, FALSE);
	
	return TRUE;
}
#endif

void startup(int argc, char *argv[]) {
#ifdef SCREENSAVER
	initGtk(argc,argv);
#else
	SDL_Init( SDL_INIT_VIDEO );
	
	const SDL_VideoInfo *info = SDL_GetVideoInfo();
	int size = info->current_h<info->current_w ? info->current_h : info->current_w;
	size = double(size)*0.85;
	SDL_Surface *sdlScreen = SDL_SetVideoMode(size,size,0, SDL_OPENGL);
	init_GL(size,size);
#endif
	
	mTimer.setup();
	
	modSignal.startup();
}

void shutdown() {
	modSignal.shutdown();
	
	SDL_Quit();
}

int main(int argc, char *argv[]) {
	startup(argc,argv);
#ifndef SCREENSAVER
		modGame.run();
#else
		modGame.init();
		gtk_main();
#endif
	shutdown();
	
	return 0;
}
