#include <X11/Xlib.h>
#include <stdio.h>

Display *display;
XEvent *event;


void mainLoop() {
	int running = 1;
	
	while(running) {
		//Process event
		XNextEvent(display, &event);
		
		if(event.type == KeyPress) {
			printf("key press\n");
		}
	}
}

int main(void) {

	//create display
	if(!(display = XOpenDisplay(0x0)))
		return 1;
	
	mainLoop();
	
	return 0;
}
