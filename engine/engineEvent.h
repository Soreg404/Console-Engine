class cEvent {
	
	struct Mouse {
		float x = 0, y = 0;
	};
	
public:
	
	Mouse mouse;
	
	cEvent();
	
	static void handleEvents(cEvent *e, HANDLE c_in);
	
	
};

cEvent::cEvent() {
	mouse = {0, 0};
}

void cEvent::handleEvents(cEvent *e, HANDLE c_in) {
	
	DWORD events = 0;
	GetNumberOfConsoleInputEvents(c_in, &events);
	
	INPUT_RECORD in_buf[32];
	
	if(events > 0)
		ReadConsoleInput(c_in, in_buf, events, &events);
	
	for (int i = 0; i < events; i++) {
		switch(in_buf[i].EventType) {
			case MOUSE_EVENT:
				switch(in_buf[i].Event.MouseEvent.dwEventFlags){
					case MOUSE_MOVED:
						e->mouse.x = in_buf[i].Event.MouseEvent.dwMousePosition.X;
						e->mouse.y = in_buf[i].Event.MouseEvent.dwMousePosition.Y;
					break;
				}
			break;
		}
	}
	
}