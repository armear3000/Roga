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


ALLEGRO_DISPLAY* display;
int width = 800;
int height = 600;

mouses mouse;
keyboards kboard;

ALLEGRO_FONT* font24 = NULL;

using namespace std;
////////////////////

// ����� 40 �� 30
const int max_x = 40;
const int max_y = 30;
const int title_size = 20;
int map[max_y][max_x];

void draw_map();
//

class player
{
public:
	player();
	~player();
	
	bool life;
	int len;
	int x, y;
	int dx, dy;
	int lx, ly;
	int ldx, ldy;
	int speed;
	deque<tuple<int,int,int,int>> kuveve;

	void control();
	void go();
	void draw();


private:

};
player::player()
{
	len = 6;
	speed = 1;
	x = 10;
	y = 10;
	dx = 1;
	dy = 0;
	lx = x - len;
	ly = y;
	ldx = dx;
	ldy = dy;
}
player::~player()
{
}
void player::control()
{
	/*if (dx == 0) {
		dx = -kboard.key[ALLEGRO_KEY_A];
		if(dx == 0)	dx = kboard.key[ALLEGRO_KEY_D];
		if (dx != 0) { 
			dy = 0;
			kuveve.push_back({ x, y, dx, dy });
		}
	} else if (dy == 0) {
		dy = -kboard.key[ALLEGRO_KEY_W];
		if (dy == 0) dy = kboard.key[ALLEGRO_KEY_S];
		if (dy != 0) {
			dx = 0;
			kuveve.push_back({ x, y, dx, dy });
		}
	}*/
	if (kboard.key[ALLEGRO_KEY_A] && dx != 1) {
		dx = -1;
		dy = 0;
		kuveve.push_back({ x, y, dx, dy });
	}
	else if (kboard.key[ALLEGRO_KEY_D] && dx != -1) {
		dx = 1;
		dy = 0;
		kuveve.push_back({ x, y, dx, dy });
	}
	else if (kboard.key[ALLEGRO_KEY_W] && dy != 1) {
		dx = 0;
		dy = -1;
		kuveve.push_back({ x, y, dx, dy });
	}
	else if (kboard.key[ALLEGRO_KEY_S] && dy != -1) {
		dx = 0;
		dy = 1;
		kuveve.push_back({ x, y, dx, dy });
	}
		
}
void player::go()
{
	map[y][x] = 1;
	map[ly][lx] = 0;

	x += dx * speed;
	y += dy * speed;

	lx += ldx * speed;
	ly += ldy * speed;

	int tx, ty, tdx, tdy;
	if (!kuveve.empty()) {
		tie(tx, ty, tdx, tdy) = kuveve.front();
		if (tie(tx, ty) == tie(lx, ly)) {
			tie(ldx, ldy) = tie(tdx, tdy);
			kuveve.pop_front();
		}
	}

}
void player::draw()
{
	al_draw_filled_rectangle(lx * title_size, ly * title_size, lx * title_size + title_size, ly * title_size + title_size, al_map_rgb(150, 150, 150));
	al_draw_filled_rectangle(x * title_size, y * title_size, x * title_size + title_size, y * title_size + title_size, al_map_rgb(255, 255, 255));

}

player snake;

int main()
{

	ALLEGRO_EVENT ev;
	ALLEGRO_EVENT_QUEUE* event_queue = NULL;
	ALLEGRO_TIMER* fps = NULL;
	ALLEGRO_TIMER* cps = NULL;
	bool flag_cps = false;

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
	cps = al_create_timer(1 / 5.0);

	al_register_event_source(event_queue, al_get_timer_event_source(fps));
	al_register_event_source(event_queue, al_get_timer_event_source(cps));
	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_mouse_event_source());
	al_register_event_source(event_queue, al_get_keyboard_event_source());

	srand(time(0));

	al_clear_to_color(al_map_rgb(0, 0, 0));

	font24 = al_load_font("resources/bin/BRUSH-N.TTF", 24, 0);
	if (!font24) abort_example("Error load fail. Resources/bin/BRUSH-N.TTF.");

	////////////////// init

	bool done = false, draw = false;
	al_start_timer(fps);
	al_start_timer(cps);

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

		}
		if (ev.type == ALLEGRO_EVENT_KEY_UP) {
			kboard.key[ev.keyboard.keycode] = false;
		}


		if (ev.type == ALLEGRO_EVENT_TIMER) {                    // ������� �������
			
			if (ev.timer.source == fps) {
				draw = true;


				snake.control();

				if (flag_cps) {
					snake.go();
					flag_cps = false;
				}
				
				mouse_click = 0;
			}
			if (ev.timer.source == cps) {
				flag_cps = true;
			}
		}

		if (draw && al_is_event_queue_empty(event_queue)) {      // ��������� 
			draw = false;
			al_clear_to_color(al_map_rgb(0, 0, 0));
			//al_draw_filled_rectangle(0,0,width,height,al_map_rgba(0,0,0,10));

			draw_map();

			snake.draw();


			al_flip_display();
		}
	}

	// ����� ������ 

	al_destroy_font(font24);
	al_destroy_timer(fps);
	al_destroy_timer(cps);
	al_destroy_display(display);
	al_destroy_event_queue(event_queue);
	return 0;
}

void draw_map()
{
	for (int y = 0; y < max_y; ++y) {
		for (int x = 0; x < max_x; ++x) {
				al_draw_rectangle(x * title_size, y * title_size, x * title_size + title_size, y * title_size + title_size, al_map_rgb(20, 20, 20), 1);
		}
	}
	for (int y = 0; y < max_y; ++y) {
		for (int x = 0; x < max_x; ++x) {
			 if (map[y][x] == 1)
				al_draw_filled_rectangle(x * title_size, y * title_size, x * title_size + title_size, y * title_size + title_size, al_map_rgb(200, 200, 200));
		}
	}
}

