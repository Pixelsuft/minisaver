#include <stdlib.h>
#include <time.h>
#include <SDL.h>

double random_double(double min_value, double max_value) {
    int res = rand();
    return (double)res * (max_value - min_value) / (double)RAND_MAX + min_value;
}

int rand_col(void) {
    return (int)((double)rand() * 256.0 / ((double)RAND_MAX + 1.0));
}

int main(int argc, char* argv[]) {
    (void)argc;
    (void)argv;
    srand((unsigned int)time(NULL));
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER) < 0) {
        return 1;
    }
    SDL_Window* win = SDL_CreateWindow(
        "minisaver",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        640, 480,
        SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_ALWAYS_ON_TOP | SDL_WINDOW_BORDERLESS |
        SDL_WINDOW_INPUT_FOCUS | SDL_WINDOW_MOUSE_FOCUS | SDL_WINDOW_FULLSCREEN_DESKTOP
    );
    if (win == NULL) {
        SDL_Quit();
        return 1;
    }
    SDL_Renderer* ren = SDL_CreateRenderer(
        win,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );
    if (ren == NULL) {
        SDL_DestroyWindow(win);
        SDL_Quit();
        return 1;
    }
    SDL_ShowCursor(SDL_DISABLE);
    SDL_Event event;
    int running = 1;
    double cur_col[3] = { 0.0, 0.0, 0.0 };
    double now_col[3];
    double target_col[3] = { (double)rand_col(), (double)rand_col(), (double)rand_col() };
    double ch_time = 1.0;
    double timer = 0.0;
    double speed[3] = { 
        (target_col[0] - cur_col[0]) / ch_time,
        (target_col[1] - cur_col[1]) / ch_time,
        (target_col[2] - cur_col[2]) / ch_time
    };
    Uint64 now_tick;
    Uint64 last_tick = SDL_GetTicks64();
    int big_delay = 0;
    while (running) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT: {
                    running = 0;
                    break;                    
                }
                case SDL_KEYDOWN: {
                    if (event.key.keysym.scancode == SDL_SCANCODE_Q) {
                        running = 0;
                        break;
                    }
                    break;
                }
                case SDL_MOUSEBUTTONDOWN: {
                    if (big_delay) {
                        big_delay = 0;
                        SDL_SetWindowFullscreen(win, SDL_WINDOW_FULLSCREEN_DESKTOP);
                    }
                    else {
                        big_delay = 1;
                        SDL_SetWindowFullscreen(win, 0);
                    }
                    break;
                }
            }
        }
        now_tick = SDL_GetTicks64();
        timer = (double)(now_tick - last_tick) / 1000.0;
        if (timer >= ch_time) {
            timer = 0.0;
            last_tick = now_tick;
            cur_col[0] = target_col[0];
            cur_col[1] = target_col[1];
            cur_col[2] = target_col[2];
            target_col[0] = (double)rand_col();
            target_col[1] = (double)rand_col();
            target_col[2] = (double)rand_col();
            ch_time = (double)random_double(1.5, 4.0);
            speed[0] = (target_col[0] - cur_col[0]) / ch_time;
            speed[1] = (target_col[1] - cur_col[1]) / ch_time;
            speed[2] = (target_col[2] - cur_col[2]) / ch_time;
        }
        now_col[0] = cur_col[0] + timer * speed[0];
        now_col[1] = cur_col[1] + timer * speed[1];
        now_col[2] = cur_col[2] + timer * speed[2];
        SDL_SetRenderDrawColor(ren, (Uint8)now_col[0], (Uint8)now_col[1], (Uint8)now_col[2], 255);
        SDL_RenderClear(ren);
        SDL_RenderPresent(ren);
        if (big_delay)
            SDL_Delay(500);
    }
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}
