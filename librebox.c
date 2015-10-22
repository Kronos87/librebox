#include <X11/Xlib.h>
#include <stdio.h>

#define MAX(a, b) ((a) > (b) ? (a) : (b))

Display *display;
Screen *screen;

XEvent event;
XWindowAttributes attr;
XWindowAttributes attr1;
XButtonEvent btnEvnt;

int lastWidth = 0, lastHeight = 0;

int loosing = 0;

//int docking_top, docking_bottom, docking_left, docking_right;

void mainLoop() {
	printf("mainLoop");
	
	while(1) {
		printf("before");
		
		//Process event
		XNextEvent(display, &event);
		
		printf("after");
		
		if(event.type == KeyPress && event.xkey.subwindow != None) {
			XRaiseWindow(display, event.xkey.subwindow);
		} else if(event.type == ButtonPress && event.xbutton.subwindow != None) {
			printf("subwindow: %lu\n", event.xbutton.subwindow);
			
			XGetWindowAttributes(display, event.xbutton.subwindow, &attr1);
			btnEvnt = event.xbutton;
		} else if(event.type == MotionNotify && btnEvnt.subwindow != None) {
			XSetWindowBorderWidth(display, btnEvnt.subwindow, 3);
			
			int xdiff = event.xbutton.x_root - btnEvnt.x_root;
			int ydiff = event.xbutton.y_root - btnEvnt.y_root;
			
			if(btnEvnt.button == 1) {
				XGetWindowAttributes(display, btnEvnt.subwindow, &attr);
				
				if(attr.y <= 1 && !loosing) {
					lastWidth = attr.width;
					lastHeight = attr.height;
					XMoveResizeWindow(display, btnEvnt.subwindow, 0, 0, screen->width, screen->height);
				} else {
					//TODO workaroung
					if(lastWidth == 0) {
						lastWidth = attr.width;	
						lastHeight = attr.height;
					}
					
					XMoveResizeWindow(display, btnEvnt.subwindow, MAX(1, attr1.x + xdiff), MAX(1, attr1.y + ydiff), lastWidth, lastHeight);
					
					//Move and loose from Top
					loosing = 1;
				}
			} else if(btnEvnt.button == 3) {
				//Resize
				lastWidth = MAX(1, attr1.width + xdiff);
				lastHeight = MAX(1, attr1.height + ydiff);
				XMoveResizeWindow(display, btnEvnt.subwindow, attr1.x, attr1.y, lastWidth, lastHeight);
			}
			
		} else if(event.type == ButtonRelease) {
			XSetWindowBorderWidth(display, btnEvnt.subwindow, 0);
			btnEvnt.subwindow = None;
			
			loosing = 1;
		}
	}
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
	
	//Cleanup
	XCloseDisplay(display);
	
	return 0;
}
