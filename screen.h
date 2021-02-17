#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <fcntl.h>
#include <math.h>

class Screen {

	int cols, rows;

	struct Pixel {
		char color = '#';
		int z = 1;
	};
	
	enum colors { fullBlock = (char)219, hash = '#', blank = ' ' };

	void clearSC() const;
	
	int status;
	
public:
	
	enum st {
		init_ = 0b1,
		cBuffer = 0b10,
	};
	
	Pixel *buffer;
	Pixel *currentBuffer = nullptr;
	
	wchar_t *screenBuffer;
	HANDLE console;
	
	struct Vec2i {
		int x, y;
	};

	Screen(int W, int H);


	int init();

	void clearBuffer();
	void draw();
	
	int drawCharacter(wchar_t character, COORD pos);
	int drawLine(wchar_t character, COORD p1, COORD p2);
	
	int fillRect(wchar_t character, RECT rect);
	int fillRect(wchar_t character, COORD pos, COORD size);

};

/*
	--------------------------------------------------------------------------------------------------------------------------------
	DEFINITIONS
*/

Screen::Screen(int W, int H): cols(W), rows(H), status(0) {}

int Screen::init() {
	
	screenBuffer = new wchar_t[rows * cols + 1];
	
	// buffer = new Pixel[rows * cols];
	
	console = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	
	SetConsoleDisplayMode(console, 2, new COORD { (short)cols, (short)rows });
	RECT consoleDim;
	HWND consoleWindow = GetConsoleWindow();
	GetWindowRect(consoleWindow, &consoleDim);
	MoveWindow(consoleWindow, 600, 200, (cols + 2) * 8, (rows + 2) * 16, TRUE);
	
	SetConsoleActiveScreenBuffer(console);
	
	status |= st::init_;
	return 1;
}

void Screen::clearSC() const {
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), {0, 0});
}

void Screen::clearBuffer() {
	if(!(status & st::init_)) return;
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			// buffer[cols * i + j].color = colors::blank;
			// buffer[cols * i + j].z = 1;
			screenBuffer[cols * i + j] = colors::blank;
		}
	}
	screenBuffer[cols * rows] = '\0';
}

void Screen::draw() {
	// for(int i = 0; i < cols * rows; i++) {
	// }
	if(status & st::init_) {
		DWORD bytesWritten = 0;
		WriteConsoleOutputCharacterW(console, screenBuffer, cols * rows + 1, { 0, 0 }, &bytesWritten);
	} else std::cout << "not initialised\n";
	
	/* std::cout way
	
	if(status & st::init_) {
		char *tmpBuf = new char[rows * cols + rows];
		for (int i = 0; i < rows; i++) {
			for(int j = 0; j < cols; j++) {
				tmpBuf[cols * i + j] = currentBuffer[cols * i + j].color;
			}
			tmpBuf[cols * (i + 1) - 1] = '\n';
		}
		std::cout << tmpBuf;
		delete tmpBuf;
	} else std::cout << "not initialised\n";
	clearSC(); */
	
}

int Screen::drawCharacter(wchar_t character, COORD pos) {
	if((pos.X >= 0 && pos.X < cols) && (pos.Y >= 0 && pos.Y < rows)) {
		screenBuffer[pos.Y * cols + pos.X] = character;
		return 0;
	}
	return -1;
}

int Screen::drawLine(wchar_t character, COORD p1, COORD p2) {
	
	// TODO: check if points inside screen (clip)
	
	int c_x = p2.X - p1.X, c_y = p2.Y - p1.Y;
	
	int start_y = p1.Y - 1;
	
	int xS = (c_x < 0) ? -1 : 1;
	int yS = (c_y < 0) ? -1 : 1;
	
	float _tan = abs((float)c_x / (float)c_y);
	
	for(int i = 0; i <= abs(c_y); i++) {
		for(int j = 0; j <= abs(c_x); j++) {
			if(abs(c_y) == 0 || abs(c_x) == 0 || (j <= (float)i * _tan + .5 && j > (float)(i>0?i-1:0) * _tan - .5)) //std::cout << "X";
				screenBuffer[(start_y + (i * yS)) * cols + (p1.X + (j * xS))] = character;
		}
	}
	
	return 1;
}

int Screen::fillRect(wchar_t character, RECT rect) {
	
	if(	(rect.left >= 0 && rect.left < cols) &&
		(rect.right >= 0 && rect.right < cols) &&
		(rect.top >= 0 && rect.top < rows) &&
		(rect.bottom >= 0 && rect.bottom < rows)) {
		
		for(int i = rect.top; i < rect.bottom; i++) {
			for (int j = rect.left; j < rect.right; j++) {
				screenBuffer[i * cols + j] = character;
			}
		}
		return 0;
	}
	
	return -1;
}

int Screen::fillRect(wchar_t character, COORD pos, COORD size) {
	
	RECT rect = { pos.X, pos.Y, pos.X + size.X, pos.Y + size.Y };
	
	return fillRect(character, rect);
}