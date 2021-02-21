#include <iostream>
#include <windows.h>

#include <ctime>
#include <vector>

#include "engine/include.h"

#define SQ(x) (x) * (x)

const float PI = 3.14159265;

struct vec2 {
	float x, y;
};

struct ball {
	float x = 0, y = 0, r = 2.f, speed = 1.f;
	vec2 direction = {1, 0};
};

const unsigned int WIDTH = 130, HEIGHT = 90;

int main() {
	
	Engine sc(120, 80);
	sc.init();
	
	srand(time(NULL));
	
	std::vector<ball> objects;
	
	ball b1 = {20, 15, 4};
	
	for(int i = 0; i < 1; i++) {
		float a = (float)(rand()%200-99) / 100.f;
		objects.push_back({ rand() % WIDTH, rand() % HEIGHT,  4 + rand() % 6, .3 + rand()%40/100.f, { a, (1.f - abs(a)) * (rand()%2?-1:1) } });
	}
	
	float a = 0;
	
	cEvent e;
	
	while(1) {
		
		cEvent::handleEvents(&e, sc.console_in);
		
		// objects[0].x = WIDTH/2 + cos(a*5) * (WIDTH/2);
		// objects[0].y = HEIGHT/2 + sin(a*3) * (HEIGHT/2);
		
		for (int i = 0; i < objects.size(); i++) {
			objects[i].x += objects[i].direction.x * objects[i].speed;
			objects[i].y += objects[i].direction.y * objects[i].speed;
			if(objects[i].speed >= 0.00002) objects[i].speed -= 0.00002;
			else objects[i].speed = 0;
		}
		
		
		sc.clearBuffer();
		
		sc.drawCharacter('O', { e.mouse.x, e.mouse.y });
		
		for(int i = 0; i < objects.size(); i++) {
			
			if(objects[i].x < 0) objects[i].x = WIDTH-1;
			if(objects[i].x >= WIDTH) objects[i].x = 0;
			if(objects[i].y < 0) objects[i].y = HEIGHT-1;
			if(objects[i].y >= HEIGHT) objects[i].y = 0;
			
			for(int j = 0; j < objects.size(); j++) {
				
				if(i == j) continue;
				
				float in_x = objects.at(j).x - objects.at(i).x, in_y = objects.at(j).y - objects.at(i).y, d = SQ(in_x) + SQ(in_y);
				
				if(d <= SQ(objects.at(i).r + objects.at(j).r)) {
					vec2 cld = { in_x / d, in_y / d };
					float halfin = ((objects.at(i).r + objects.at(j).r) - sqrt(d)) / 2 + .5;
					objects.at(i).x -= cld.x * halfin; objects.at(i).y -= cld.y * halfin;
					objects.at(j).x += cld.x * halfin; objects.at(j).y += cld.y * halfin;
					sc.drawLine('@', {objects.at(i).x+1, objects.at(i).y}, {objects.at(j).x+1, objects.at(j).y});
					
					
				}
			}
		}
		
		for(auto i = objects.begin(); i < objects.cend(); i++) {
			sc.drawLine('.', { i->x, i->y }, { i->x + i->direction.x * i->r, i->y + i->direction.y * i->r });
			for(float x = 0; x < 2 * PI; x += .2) {
				sc.drawLine('#', { i->x + cos(x) * i->r, i->y + sin(x) * i->r }, { i->x + cos(x+.2) * i->r, i->y + sin(x+.2) * i->r });
			}
		}
		
		for (int i = 0; i < objects.size(); i++) {
			sc.drawCharacter((char)(i+48), { objects[i].x, objects[i].y-1 });
		}
		
		
		sc.draw();
		
		a += .002;
		
	}
	objects.~vector();
	getchar();
}