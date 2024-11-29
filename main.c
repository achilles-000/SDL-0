#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdbool.h>




    struct direction {
        bool right;
        bool left;
        bool up;
        bool down;
        int vertical;
    };

void middle_Line(SDL_Renderer *renderer){
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for(int y = 0; y <= 480; y += 65){ 
        SDL_Rect line = {317, y, 6, 25};
        SDL_RenderFillRect(renderer, &line);
    }


}


int main(int argc, char *argv[]) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    // Initialize SDL_ttf
    if (TTF_Init() == -1) {
        fprintf(stderr, "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
        return 1;
    }

    // Variables:
    struct direction dir0 = { false, false, false, false, 300 };
    struct direction dir1 = { false, false, false, false, 300 };
    int ballX = 300, ballY = 220, scoreP1 = 0, scoreP2 = 0;
    float speedX = 1.0, speedY = 1.0;
    char scoreText[50];
    char scoreText2[50];


    // Create a window
    SDL_Window *window = SDL_CreateWindow("Pong",
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          640, 480,
                                          SDL_WINDOW_SHOWN);  //SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE); for resizeable
    if (window == NULL) {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // Create renderer
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }



    // Main loop
    SDL_bool running = SDL_TRUE;
    while (running && scoreP1 < 5 && scoreP2 < 5) {
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
                        dir0.up = true;
                    }
                    if (event.key.keysym.sym == SDLK_s) {
                        dir0.down = true;
                    }
                    if (event.key.keysym.sym == SDLK_UP) {
                        dir1.up = true;
                    }
                    if (event.key.keysym.sym == SDLK_DOWN) {
                        dir1.down = true;
                    }
                    break;

                }
                case SDL_KEYUP: {
                    if (event.key.keysym.sym == SDLK_w) {
                        dir0.up = false;
                    }
                    if (event.key.keysym.sym == SDLK_s) {
                        dir0.down = false;
                    }
                    if (event.key.keysym.sym == SDLK_UP) {
                        dir1.up = false;
                    }
                    if (event.key.keysym.sym == SDLK_DOWN) {
                        dir1.down = false;
                    }
                    break;

                }
                default:
                    break;
            }
        }
        



        if (dir0.up == true && dir0.vertical > 0) dir0.vertical -= 3;
        if (dir0.down == true && dir0.vertical < 380) dir0.vertical += 3;
        if (dir1.up == true && dir1.vertical > 0) dir1.vertical -= 3;
        if (dir1.down == true && dir1.vertical < 380) dir1.vertical += 3;
        
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Set the draw color to red and draw a filled rectangle
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

        SDL_Rect rect0 = {5, dir0.vertical, 25, 100};
        SDL_Rect rect1 = {610, dir1.vertical, 25, 100};
        SDL_RenderFillRect(renderer, &rect0);
        SDL_RenderFillRect(renderer, &rect1);

        //Boundries of the ball
        if(ballX >= 620) {
            speedX *= -1;
            scoreP1 += 1;
            printf("\nScore P1: %d", scoreP1);
        }
        if(ballX <= 0){
            speedX *= -1;
            scoreP2 += 1;
            printf("\nScore P2: %d", scoreP2);
        }

        if(ballY >= 460 || ballY <= 0) speedY *= -1;
        
        //Makes the ball move by 1 every iteration
        ballX += speedX;
        ballY += speedY;

        SDL_Rect ball = {ballX, ballY, 20, 20};
        SDL_RenderFillRect(renderer, &ball);




        //Check if ball and player are overlapping
        if(ballY > dir0.vertical && (ballY < dir0.vertical + 100 ) && ballX <= 30) {
            speedX *= -1;
        }
        if(ballY > dir1.vertical && (ballY < dir1.vertical + 100 ) && ballX >= 595) {
            speedX *= -1;
        }
        
        // Function to draw middle line
        middle_Line(renderer);



        //Text ---------------------------------------------------
        

        sprintf(scoreText, "%d", scoreP1);
        sprintf(scoreText2, "%d", scoreP2);

        //Font and size and color
        TTF_Font* Sans = TTF_OpenFont("bit5x3.ttf", 24);
        SDL_Color White = {255, 255, 255};


        // TTF_RenderText_Solid can only be used on a surface
        // create a surface:
        SDL_Surface *surfaceMessage = 
            TTF_RenderText_Solid(Sans, scoreText, White);
        SDL_Surface *surfaceMessage2 = 
            TTF_RenderText_Solid(Sans, scoreText2, White);

        
        SDL_Texture *Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
        SDL_Texture *Message2 = SDL_CreateTextureFromSurface(renderer, surfaceMessage2);

        SDL_Rect Message_rect; 
        Message_rect.x = 230; //x cord
        Message_rect.y = 10; //y cord
        Message_rect.w = 50;  //width
        Message_rect.h = 50; //height
    
        SDL_Rect Message_rect2; 
        Message_rect2.x = 370; //x cord
        Message_rect2.y = 10; //y cord
        Message_rect2.w = 50;  //width
        Message_rect2.h = 50; //height

        SDL_RenderCopy(renderer, Message, NULL, &Message_rect);
        SDL_RenderCopy(renderer, Message2, NULL, &Message_rect2);

        




        // Update the screen
        SDL_RenderPresent(renderer);







    }

    // Clean up


    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
    return 0;
}
