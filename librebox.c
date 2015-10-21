#include <X11/Xlib.h>
#include <stdio.h>

#define MAX(a, b) ((a) > (b) ? (a) : (b))

Display *display;
Screen *screen;

XEvent event;
XWindowAttributes attr;
XButtonEvent btnEvnt;

int lastWidth = 0, lastHeight = 0;

void mainLoop() {
	
	int running = 1;
	
	while(running) {
		//Process event
		XNextEvent(display, &event);
		
		if(event.type == KeyPress && event.xkey.subwindow != None) {
			XRaiseWindow(display, event.xkey.subwindow);
		} else if(event.type == ButtonPress && event.xbutton.subwindow != None) {
			XGetWindowAttributes(display, event.xbutton.subwindow, &attr);
			btnEvnt = event.xbutton;
		} else if(event.type == MotionNotify && btnEvnt.subwindow != None) {
			XSetWindowBorderWidth(display, btnEvnt.subwindow, 3);
			
			int xdiff = event.xbutton.x_root - btnEvnt.x_root;
			int ydiff = event.xbutton.y_root - btnEvnt.y_root;
			
			XMoveResizeWindow(display, btnEvnt.subwindow, 
				attr.x + (btnEvnt.button == 1 ? xdiff : 0),
				attr.y + (btnEvnt.button == 1 ? ydiff : 0),
				MAX(1, attr.width + (btnEvnt.button == 3 ? xdiff : 0)),
				MAX(1, attr.height + (btnEvnt.button == 3 ? ydiff : 0)));
			
			if(btnEvnt.button == 1) {
				if(event.xbutton.y_root <= 1) {
					XGetWindowAttributes(display, btnEvnt.subwindow, &attr);
					lastWidth = attr.width;
					lastHeight = attr.height;
					XMoveResizeWindow(display, btnEvnt.subwindow, 0, 0, screen->width, screen->height);
				} else {
					XMoveResizeWindow(display, btnEvnt.subwindow, MAX(1, attr.x + xdiff), MAX(1, attr.y + ydiff), lastWidth, lastHeight);
				}
			} else if(btnEvnt.button == 3) {
				XMoveResizeWindow(display, btnEvnt.subwindow, attr.x, attr.y, MAX(1, attr.width + xdiff), MAX(1, attr.height + ydiff));
			}
			
		} else if(event.type == ButtonRelease) {
			btnEvnt.subwindow = None;
		}
	}
	
	//Cleanup
	
	XCloseDisplay(display);
}

int main(void) {

	//create display
	if(!(display = XOpenDisplay(NULL)))
		return 1;
	
	screen = DefaultScreenOfDisplay(display);
	printf("Screen: width=%d, height=%d\n", screen->width, screen->height);
	
	
	XGrabKey(display, XKeysymToKeycode(display, XStringToKeysym("F1")), Mod1Mask, DefaultRootWindow(display), True, GrabModeAsync, GrabModeAsync);
	XGrabButton(display, 1, Mod1Mask, DefaultRootWindow(display), True, ButtonPressMask|ButtonReleaseMask|PointerMotionMask, GrabModeAsync, GrabModeAsync, None, None);
	XGrabButton(display, 3, Mod1Mask, DefaultRootWindow(display), True, ButtonPressMask|ButtonReleaseMask|PointerMotionMask, GrabModeAsync, GrabModeAsync, None, None);
	
	btnEvnt.subwindow = None;
	
	mainLoop();
	
	return 0;
}
