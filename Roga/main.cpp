#define _CRT_SECURE_NO_WARNINGS



ClassGamePlay Gameplay;
ClassNewGame NewGame;
ClassLoadGame LoadGame;
ClassMenu Menu;
ClassSettings Settings;
ClassAbout About;
ClassExit Exit;

////////////////////

int main()
{

	ALLEGRO_EVENT ev;
	ALLEGRO_EVENT_QUEUE* event_queue = NULL;
	ALLEGRO_TIMER* fps = NULL;

	if (!al_init()) exit(1);
	display = al_create_display(width, height);
	al_set_display_flag(display, ALLEGRO_FULLSCREEN_WINDOW, true);
	FullScreenXY();
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

	font24 = al_load_font("resources/bin/BRUSH-N.TTF", 24, 0);
	if (!font24) abort_example("Error load fail. Resources/bin/BRUSH-N.TTF.");

	////////////////// init

	Gameplay.init();
	NewGame.init();
	LoadGame.init();
	Menu.init();
	Settings.init();
	About.init();
	Exit.init();

	bool done = false, draw = false;
	al_start_timer(fps);

	for (; !done;) {
		al_wait_for_event(event_queue, &ev);
		if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {            // закрытие окна
			done = true;
		}

		mouse.CoordinatesInit(ev);

		// обработка мыши
		if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
			mouse.key[ev.mouse.button - 1] = true;
			mouse_click = ev.mouse.button;

			switch (state) {
			case GAMEPLAY:
				Gameplay.matmousedown();
				break;
			case NEWGAME:
				NewGame.matmousedown();
				break;
			case LOADGAME:
				LoadGame.matmousedown();
				break;
			case MENU:
				Menu.matmousedown();
				break;
			case SETTINGS:
				Settings.matmousedown();
				break;
			case ABOUT:
				About.matmousedown();
				break;
			case EXIT:

				break;
			default:
				break;
			}

		}
		if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
			mouse.key[ev.mouse.button - 1] = false;
		}

		// обработка клавиатуры 
		if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
			kboard.key[ev.keyboard.keycode] = true;

			if (kboard.key[ALLEGRO_KEY_LSHIFT]) {
				if (kboard.key[ALLEGRO_KEY_1]) SetState(GAMEPLAY);
				if (kboard.key[ALLEGRO_KEY_2]) SetState(NEWGAME);
				if (kboard.key[ALLEGRO_KEY_3]) SetState(LOADGAME);
				if (kboard.key[ALLEGRO_KEY_4]) SetState(MENU);
				if (kboard.key[ALLEGRO_KEY_5]) SetState(SETTINGS);
				if (kboard.key[ALLEGRO_KEY_6]) SetState(ABOUT);
				if (kboard.key[ALLEGRO_KEY_7]) SetState(EXIT);

			}

			switch (state) {
			case GAMEPLAY:
				Gameplay.matdown();
				break;
			case NEWGAME:
				NewGame.matdown();
				break;
			case LOADGAME:

				break;
			case MENU:

				break;
			case SETTINGS:

				break;
			case ABOUT:

				break;
			case EXIT:

				break;
			default:
				break;
			}
		}
		if (ev.type == ALLEGRO_EVENT_KEY_UP) {
			kboard.key[ev.keyboard.keycode] = false;
		}


		if (ev.type == ALLEGRO_EVENT_TIMER) {                    // игровой процесс
			if (ev.timer.source == fps) {
				draw = true;

				switch (state) {
				case GAMEPLAY:
					Gameplay.mat();
					break;
				case NEWGAME:
					NewGame.mat();
					break;
				case LOADGAME:
					LoadGame.mat();
					break;
				case MENU:

					break;
				case SETTINGS:
					Settings.mat();
					break;
				case ABOUT:
					About.mat();
					break;
				case EXIT:

					return 1;
					break;
				default:
					break;
				}


				mouse_click = 0;
			}
			NewGame.timers(ev);

		}



		if (draw && al_is_event_queue_empty(event_queue)) {      // отрисовка 
			draw = false;
			al_clear_to_color(al_map_rgb(0, 0, 0));
			//al_draw_filled_rectangle(0,0,width,height,al_map_rgba(0,0,0,10));

			switch (state) {
			case GAMEPLAY:
				Gameplay.draw();
				break;
			case NEWGAME:
				NewGame.draw();
				break;
			case LOADGAME:
				LoadGame.draw();
				break;
			case MENU:
				Menu.draw();
				break;
			case SETTINGS:
				Settings.draw();
				break;
			case ABOUT:
				About.draw();
				break;
			case EXIT:

				break;
			default:
				break;
			}


			al_flip_display();
		}
	}

	// конец работы 
	Gameplay.destroy();
	LoadGame.destroy();
	NewGame.destroy();
	Menu.destroy();
	Settings.destroy();
	About.destroy();
	Exit.destroy();

	al_destroy_font(font24);
	al_destroy_timer(fps);
	al_destroy_display(display);
	al_destroy_event_queue(event_queue);
	return 0;
}



