#include <iostream>
#include <windows.h>

#include "screen.h"

const float PI = 3.14159265;

int main() {
	
	Screen sc(120, 60);
	sc.init();
	
	float an = 0.f, an2 = 0.f;
	
	while(1) {
		sc.clearBuffer();
		
		sc.drawCharacter(0x2588, { (short)(100 + cos(an2)*8), (short)(12 + sin(an2)*7) });
		
		for(float i = 0.f; i < 2 * PI; i+=.01f) {
			sc.drawCharacter('.', { (short)(60 + cos((i+an2)*2)*14), (short)(20 + sin(i+an)*13) });
		}
		
		
		for(float i = 0.f; i < 2 * PI; i+=PI/5) {
			sc.drawLine('.', { (short)(20 + cos(i + an2)*14), (short)(20 + sin((i + an) * 2)*13) }, { 20, 20 });
		}
		
		sc.drawLine('*', { (short)(40 + cos(-an)*8 + cos(-an*3)*2), (short)(20 + sin(-an)*5 + sin(-an*9)*1) }, { (short)(40 + cos(an)*15 + cos(an*4)*6 + cos(an*8)*3), (short)(20 + sin(an)*10 + sin(an*4)*5 + sin(an*7)*2) });
		
		sc.drawCharacter('A', { 60 + sin(an) * 40, 20 + cos(an*3) * 6 });
		
		sc.draw();
		
		an += 0.002f;
		an2 += 0.001f;
		
	}
	getchar();
}