#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h>

//librebox Window dock value

/*

Erzeuge LBWindow Array nach XQueryTree...
vergleiche in regelmässigen Abständen diese beiden Listen und erzeuge evtl. neues Array !!!


*/


#define DockingTop		1
#define DockingRight	2
#define DockingBottom	3
#define DockingLeft		4


typedef struct {
	unsigned long xwindow;
	int docking;
	unsigned int lastWidth, lastHeight;
} LBWindow;

int main(void) {
	
	Display *dpy;
	
	if(!(dpy = XOpenDisplay(NULL)))
		return 1;
	
	int screen_num = DefaultScreen(dpy);

	/*
	Window w = XCreateSimpleWindow(dpy, RootWindow(dpy, screen_num), 100, 100, 400, 400, 0, BlackPixel(dpy, screen_num), BlackPixel(dpy, screen_num));
	
	XMapWindow(dpy, w);
	
	GC gc;
	
	gc = XCreateGC(dpy, w, 0, 0);
	
	XSetForeground(dpy, gc, WhitePixel(dpy, screen_num));
	XSetFillStyle(dpy, gc, FillSolid);
	
	XDrawRectangle(dpy, w, gc, 25, 25, 55, 55);
	
	//XFlush(dpy);
	
	*/
	
	Window w_root;
	Window w_parent;
	Window *w_children;
	unsigned int no_children;
	
	
	
	//XEvent event;
	
	while(1) {
		//XNextEvent(dpy, &event);
		
		system("clear");
	
		XQueryTree(dpy, XRootWindow(dpy, screen_num), &w_root, &w_parent, &w_children, &no_children);
	
		//LBWindow Array
		LBWindow *windows = malloc(no_children * sizeof(LBWindow)); 
		printf("root: %lu | parent: %lu | No. children: %d\n", w_root, w_parent, no_children);
	
		int i=0;
	
		for(;i<no_children;i++) {
			//printf("#%d: %lu\n", i, w_children[i]);
		
			//new LBWindow ELement
			windows[i] = malloc(sizeof(LBWindow));
			windows[i]->xwindow = w_children[i];
			printf("#%d: %lu\n", i, windows[i]->xwindow);
		}
		
		
		//Am Ende löschen
		for(i=0;i<no_children;i++)
			free(windows[i]);
		
		free(windows);
		
	}
	

	return 0;
}