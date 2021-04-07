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


				mouse_click = 0;
				kboard.last_key_refresher();
			}
		}

		if (draw && al_is_event_queue_empty(event_queue)) {      // ��������� 
			draw = false;
			al_clear_to_color(al_map_rgb(0, 0, 0));
			//al_draw_filled_rectangle(0,0,width,height,al_map_rgba(0,0,0,10));
			

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
