#include <SDL3/SDL.h>
#include <stdlib.h>
#include <time.h>
#define WIN32_DWM_HACK
#if defined(_WIN32) && defined(WIN32_DWM_HACK)
#define WIN32_LEAN_AND_MEAN
#include <dwmapi.h>
#endif

SDL_Window* win;
SDL_Renderer* ren;

static inline float random_float(float min_value, float max_value) {
    int res = rand();
    return (float)((double)res * ((double)max_value - (double)min_value) / (double)RAND_MAX) + min_value;
}

static inline float random_col(void) {
	int res = rand();
	return (float)((double)res / (double)RAND_MAX);
}

static void main_loop(void) {
	SDL_Event ev;
	float cur_col[3] = { 0.f, 0.f, 0.f };
    float now_col[3];
    float target_col[3] = { random_col(), random_col(), random_col() };
    float ch_time = 1.f;
    float timer = 0.f;
    float speed[3] = { 
        (target_col[0] - cur_col[0]) / ch_time,
        (target_col[1] - cur_col[1]) / ch_time,
        (target_col[2] - cur_col[2]) / ch_time
    };
	Uint64 last_tick = SDL_GetTicks();
	while (1) {
		while (SDL_PollEvent(&ev)) {
			switch (ev.type) {
#ifdef _WIN32
				case SDL_EVENT_WINDOW_HIDDEN:
				case SDL_EVENT_WINDOW_MINIMIZED:
				case SDL_EVENT_WINDOW_FOCUS_LOST:
#endif
				case SDL_EVENT_QUIT:
				case SDL_EVENT_MOUSE_BUTTON_DOWN:
				case SDL_EVENT_KEY_DOWN: {
					return;
				}
			}
		}
		Uint64 now_tick = SDL_GetTicks();
		float timer = (float)(now_tick - last_tick) / 1000.f;
		if (timer >= ch_time) {
			timer = 0.f;
            last_tick = now_tick;
			cur_col[0] = target_col[0];
            cur_col[1] = target_col[1];
            cur_col[2] = target_col[2];
            target_col[0] = random_col();
            target_col[1] = random_col();
            target_col[2] = random_col();
            ch_time = random_float(1.5f, 4.f);
            speed[0] = (target_col[0] - cur_col[0]) / ch_time;
            speed[1] = (target_col[1] - cur_col[1]) / ch_time;
            speed[2] = (target_col[2] - cur_col[2]) / ch_time;
		}
        now_col[0] = cur_col[0] + timer * speed[0];
        now_col[1] = cur_col[1] + timer * speed[1];
        now_col[2] = cur_col[2] + timer * speed[2];
		SDL_SetRenderDrawColorFloat(ren, now_col[0], now_col[1], now_col[2], 1.f);
		SDL_RenderClear(ren);
		SDL_RenderPresent(ren);
#if defined(_WIN32) && defined(WIN32_DWM_HACK)
		DwmFlush();
#endif
	}
}

int main(int argc, char* argv[]) {
	(void)argc;
	(void)argv;
	win = NULL;
	ren = NULL;
	srand((unsigned int)time(NULL));
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)) {
		win = SDL_CreateWindow(
			"minisaver", 640, 480,
			SDL_WINDOW_BORDERLESS | SDL_WINDOW_MOUSE_GRABBED | SDL_WINDOW_KEYBOARD_GRABBED |
			SDL_WINDOW_HIGH_PIXEL_DENSITY | SDL_WINDOW_ALWAYS_ON_TOP | SDL_WINDOW_FULLSCREEN
		);
		if (win) {
			SDL_PropertiesID props = SDL_CreateProperties();
			if (props) {
				SDL_SetPointerProperty(props, SDL_PROP_RENDERER_CREATE_WINDOW_POINTER, (void*)win);
#if defined(_WIN32) && defined(WIN32_DWM_HACK)
				SDL_SetNumberProperty(props, SDL_PROP_RENDERER_CREATE_PRESENT_VSYNC_NUMBER, 0);
#else
				SDL_SetNumberProperty(props, SDL_PROP_RENDERER_CREATE_PRESENT_VSYNC_NUMBER, 1);
#endif
				ren = SDL_CreateRendererWithProperties(props);
				SDL_DestroyProperties(props);
				if (ren) {
					SDL_HideCursor();
					main_loop();
					SDL_DestroyRenderer(ren);
				}
			}
			SDL_DestroyWindow(win);
		}
		SDL_Quit();
	}
	return (ren == NULL) ? 1 : 0;
}
