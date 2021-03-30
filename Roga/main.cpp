#define _CRT_SECURE_NO_WARNINGS

#include <cstdlib>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_native_dialog.h>
#include <math.h>
#include <stdio.h> 
#include <conio.h>
#include <stdlib.h>
#include <cstdlib>
#include <time.h>
#include <Windows.h>

#include <iostream>

#include <vector>
#include <deque>
#include <tuple>
#include <algorithm>
#include <iterator>

#include "Geometry.h"
#include "SystemFun.h"
#include "DevicesAndButtons.h"

using namespace std;


ALLEGRO_DISPLAY* display;
int width = 800;
int height = 600;

mouses mouse;
keyboards kboard;

ALLEGRO_FONT* font24 = NULL;


////////////////////

class player
{
public:
	player();
	~player();
	void draw();
	void control();
	void mathrun();
private:
	double x, y, w, h, speed;
};
player::player()
{
	y = double(height) - 100;
	x = double(width)/2 - 50;
	w = 100;
	h = 20;
	speed = 0;

}
player::~player()
{
}
void player::draw()
{
	al_draw_rectangle(x, y, x + w, y + h, al_map_rgb(255, 255, 255),2);
}
void player::control()
{
	speed = -kboard.key[ALLEGRO_KEY_A] + kboard.key[ALLEGRO_KEY_D];

}
void player::mathrun()
{
	x += speed*4;
	if (x < 0) x = 0;
	else if (x + w > width) x = width - w;
}

class balls
{
public:
	balls();
	~balls();
	void mathrun();
	void draw();

	double x, y, r;
	double speed,dx, dy, angle;
};
balls::balls()
{
	x = width / 2;
	y = height - 150.0;
	r = 5;
	dx = 0;
	dy = 0;
	speed = 3;
	angle = 300;
}
balls::~balls()
{
}
void balls::mathrun()
{
	
	dx = cos(angle / FROMRADTOGRAD);
	dy = sin(angle / FROMRADTOGRAD);

	x += dx*speed;
	y += -dy*speed;

	if (x + r > width) {
		dx = -dx;
		angle = atan2(y - (y - dy), x - (x - dx)) * FROMRADTOGRAD;
		x = width - r - 1;
	}else if (x - r < 0) {
		dx = -dx;
		angle = atan2(y - (y - dy), x - (x - dx)) * FROMRADTOGRAD;
		x = 0 + r + 1;

	}
	
	if (y + r > height) {
		dy = -dy;
		angle = atan2(y - (y - dy), x - (x - dx)) * FROMRADTOGRAD;
		y = height - r - 1;
	}else if (y - r < 0) {
		dy = -dy;
		angle = atan2(y - (y - dy), x - (x - dx)) * FROMRADTOGRAD;
		y = 0 + r + 1;
	}
	

}
void balls::draw()
{
	
	al_draw_filled_circle(x, y, r, al_map_rgb(255, 255, 255));

	al_draw_textf(font24, al_map_rgb(255, 255, 255), x + r, y - 24, 0, "dx: %.4f", dx);
	al_draw_textf(font24, al_map_rgb(255, 255, 255), x + r, y - 48, 0, "dy: %.4f", dy);

	al_draw_textf(font24, al_map_rgb(255, 255, 255), x + r, y, 0, "anlge: %.2f", angle);
	al_draw_line(x, y, x + r + 50, y, al_map_rgb(255, 255, 255), 1);
	al_draw_circle(x, y, r + 50, al_map_rgb(255, 255, 255), 1);
}

player p;
balls ball;

int main()
{
	srand(time(0));
	ALLEGRO_EVENT ev;
	ALLEGRO_EVENT_QUEUE* event_queue = NULL;
	ALLEGRO_TIMER* fps = NULL;

	if (!al_init()) exit(1);
	display = al_create_display(width, height);
	al_set_display_flag(display, 0, true);
	//al_set_display_flag(display, ALLEGRO_FULLSCREEN_WINDOW, true);
	if (!display) exit(1);

	al_init_font_addon();
	al_init_ttf_addon();
	al_init_primitives_addon();
	al_init_image_addon();
	al_install_mouse();
	al_install_keyboard();

	event_queue = al_create_event_queue();
	fps = al_create_timer(1 / 60.0);

	al_register_event_source(event_queue, al_get_timer_event_source(fps));
	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_mouse_event_source());
	al_register_event_source(event_queue, al_get_keyboard_event_source());

	srand(time(0));

	al_clear_to_color(al_map_rgb(0, 0, 0));

	font24 = al_load_font("resources/BRUSH-N.TTF", 24, 0);
	if (!font24) abort_example("Error load fail: resources/BRUSH-N.TTF.");

	////////////////// init

	bool done = false, draw = false;
	al_start_timer(fps);

	for (; !done;) {
		al_wait_for_event(event_queue, &ev);
		if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {            // �������� ����
			done = true;
		}

		mouse.CoordinatesInit(ev);

		// ��������� ����
		if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
			mouse.key[ev.mouse.button - 1] = true;
			mouse_click = ev.mouse.button;


		}
		if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
			mouse.key[ev.mouse.button - 1] = false;
		}

		// ��������� ���������� 
		if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
			kboard.key[ev.keyboard.keycode] = true;
			kboard.last_key[ev.keyboard.keycode] = true;
		}
		if (ev.type == ALLEGRO_EVENT_KEY_UP) {
			kboard.key[ev.keyboard.keycode] = false;
		}


		if (ev.type == ALLEGRO_EVENT_TIMER) {                    // ������� �������

			if (ev.timer.source == fps) {
				draw = true;

				if (kboard.key[ALLEGRO_KEY_SPACE]) ball.speed = 0; else ball.speed = 3;

				p.control();
				p.mathrun();

				ball.mathrun();

				mouse_click = 0;
				kboard.last_key_refresher();
			}
		}

		if (draw && al_is_event_queue_empty(event_queue)) {      // ��������� 
			draw = false;
			al_clear_to_color(al_map_rgb(0, 0, 0));
			//al_draw_filled_rectangle(0,0,width,height,al_map_rgba(0,0,0,10));
			 
			p.draw();
			ball.draw();

			al_flip_display();
		}
	}

	// ����� ������ 

	al_destroy_font(font24);
	al_destroy_timer(fps);
	al_destroy_display(display);
	al_destroy_event_queue(event_queue);
	return 0;
}