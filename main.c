#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>

int main(int argc, char *argv[]) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    // Variables:
    int i = 1;
    bool right = false;
    bool left = false;
    bool up = false;
    bool down = false;
    int horizontal = 100;
    int vertical = 300;

    struct direction {
        bool up =false;




    }

    // Create a window
    SDL_Window *window = SDL_CreateWindow("Movable Window",
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          640, 480,
                                          SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (window == NULL) {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // Create renderer
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Main loop
    SDL_bool running = SDL_TRUE;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT: {
                    running = SDL_FALSE;
                    break;
                }
                case SDL_MOUSEBUTTONDOWN: {
                    if (event.button.button == SDL_BUTTON_LEFT) {
                        // Save the initial window position
                        int initialX, initialY;
                        SDL_GetWindowPosition(window, &initialX, &initialY);

                        // Move the window with the mouse
                        int windowX, windowY;
                        while (SDL_PollEvent(&event)) {
                            if (event.type == SDL_MOUSEMOTION) {
                                windowX = initialX + event.motion.x - event.button.x;
                                windowY = initialY + event.motion.y - event.button.y;
                                SDL_SetWindowPosition(window, windowX, windowY);
                            } else if (event.type == SDL_MOUSEBUTTONUP &&
                                       event.button.button == SDL_BUTTON_LEFT) {
                                break;
                            }
                        }
                    }
                    break;
                }
                case SDL_KEYDOWN: {
                    if (event.key.keysym.sym == SDLK_w) {
                        up = true;
                    }
                    if (event.key.keysym.sym == SDLK_a) {
                        left = true;
                    }
                    if (event.key.keysym.sym == SDLK_s) {
                        down = true;
                    }
                    if (event.key.keysym.sym == SDLK_d) {
                        right = true;
                    }
                    break;

                }
                case SDL_KEYUP: {
                    if (event.key.keysym.sym == SDLK_w) {
                        up = false;
                    }
                    if (event.key.keysym.sym == SDLK_a) {
                        left = false;
                    }
                    if (event.key.keysym.sym == SDLK_s) {
                        down = false;
                    }
                    if (event.key.keysym.sym == SDLK_d) {
                        right = false;
                    }
                    break;

                }
                default:
                    break;
            }
        }

        if(up == true)vertical -= 1;
        if(down == true)vertical += 1;
        if(left == true)horizontal -= 1;
        if(right == true)horizontal += 1;
        

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Set the draw color to red and draw a filled rectangle
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_Rect rect = {horizontal, vertical, 25, 100};
        SDL_RenderFillRect(renderer, &rect);

        // Update the screen
        SDL_RenderPresent(renderer);


    }

    // Clean up
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
