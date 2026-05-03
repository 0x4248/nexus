/* CPP_SDL
 * Ignore my crap code please
 */

#include <SDL.h>

int main(int argc, char** argv)
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_SetWindowResizable(window, SDL_TRUE);

    int cursor_last_x = 1, cursor_last_y = 1;
    int pixel_size = 10;
    bool running = true;
    SDL_Rect box = {0, 0, pixel_size, pixel_size};

    while (running)
    {
        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            switch (e.type)
            {
                case SDL_QUIT:
                    running = false;
                    break;
                case SDL_KEYDOWN:
                    switch (e.key.keysym.sym)
                    {
                        case SDLK_ESCAPE:
                            running = false;
                            break;
                        case SDLK_UP:
                            box.y--;
                            break;
                        case SDLK_DOWN:
                            box.y++;
                            break;
                        case SDLK_LEFT:
                            box.x--;
                            break;
                        case SDLK_RIGHT:
                            box.x++;
                            break;
                        default:
                            break;
                    }
                    break;
                default:
                    break;
            }
        }

        if (box.x != cursor_last_x || box.y != cursor_last_y)
        {
            SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
            SDL_RenderClear(renderer);

            SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
            box.x = box.x * pixel_size;
            box.y = box.y * pixel_size;
            SDL_RenderFillRect(renderer, &box);

            SDL_RenderPresent(renderer);

            cursor_last_x = box.x;
            cursor_last_y = box.y;
        }
        SDL_Delay(1000 / 60);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
