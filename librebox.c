#include <X11/Xlib.h>

#define MAX(a, b) ((a) > (b) ? (a) : (b))

Display *display;

XEvent event;
XWindowAttributes attr;
XButtonEvent btnEvnt;

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
			int xdiff = event.xbutton.x_root - btnEvnt.x_root;
			int ydiff = event.xbutton.y_root - btnEvnt.y_root;
			
			XMoveResizeWindow(display, btnEvnt.subwindow, 
				attr.x + (btnEvnt.button == 1 ? xdiff : 0),
				attr.y + (btnEvnt.button == 1 ? ydiff : 0),
				MAX(1, attr.width + (btnEvnt.button == 2 ? xdiff : 0)),
				MAX(1, attr.height + (btnEvnt.button == 2 ? ydiff : 0)));
		} else if(event.type == ButtonRelease) {
			btnEvnt.subwindow = None;
		}
	}
}

int main(void) {

	//create display
	if(!(display = XOpenDisplay(NULL)))
		return 1;
	
	XGrabKey(display, XKeysymToKeycode(display, XStringToKeysym("F1")), Mod1Mask, DefaultRootWindow(display), True, GrabModeAsync, GrabModeAsync);
	XGrabButton(display, 1, Mod1Mask, DefaultRootWindow(display), True, ButtonPressMask|ButtonReleaseMask|PointerMotionMask, GrabModeAsync, GrabModeAsync, None, None);
	XGrabButton(display, 2, Mod1Mask, DefaultRootWindow(display), True, ButtonPressMask|ButtonReleaseMask|PointerMotionMask, GrabModeAsync, GrabModeAsync, None, None);
	
	btnEvnt.subwindow = None;
	
	mainLoop();
	
	return 0;
}
