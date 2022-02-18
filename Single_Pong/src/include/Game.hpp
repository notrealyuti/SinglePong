#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "Structures.h"
#define MAX_TITLE_SIZE 30


class Game {
        uint16_t FPS            = 60;
        float FRAME_TARGET_TIME = 1000.0f / FPS;
        const uint16_t SLIDER_MOV_DIS = 500;
        const uint16_t BALL_MOV_DIS   = 5;

    private:
        SDL_Window* window     = nullptr;
        char title[MAX_TITLE_SIZE] = ""; 
        char scoreString[35];
        uint32_t score;
        struct slider_t slider;
        struct ball_t ball;
        TTF_Font* font;

        

    public:
        void drawText(const char* p_text, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t r, uint8_t g, uint8_t b);
		bool gameRunning;
        SDL_Renderer* renderer = nullptr;
        uint16_t WINDOW_WIDTH;
        uint16_t WINDOW_HEIGHT;
        uint32_t lastFrameTime = 0;
        float deltaTime        = 0;
        uint16_t bestScore;

        Game(const char* title_local, uint16_t width, uint16_t height);

        void setupWindow();
        void closeGame();
        void setupGameAndObjects();
        void processInput();
        void update();
        void render();
        void pauseGame();
        void waitForNextFPS();   
        void gh0Encryption(char* str, char mode);   
};


Game::Game(const char* p_title, uint16_t p_width, uint16_t p_height) {
    strcpy(title, p_title);
    WINDOW_WIDTH = p_width;
    WINDOW_HEIGHT = p_height;    
}

void Game::setupWindow() {
    // Don't show cmd
    
    ShowWindow(GetConsoleWindow(), false);    

    if (SDL_Init(SDL_INIT_EVERYTHING)) {
        fprintf(stderr, "Error initialize SDL\n");
        gameRunning = false;
        return;
    }

    if (TTF_Init()) {
        fprintf(stderr, "Error init TTF\n");
        gameRunning = false;
        return;
    }

    window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_FULLSCREEN);

    if (!window) {
        fprintf(stderr, "Error creating window\n");
        gameRunning = false;
        return;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (!renderer) {
        fprintf(stderr, "Error creating renderer\n");
        gameRunning = false;
        return;
    }

    font = TTF_OpenFont("./assets/font.ttf", 50);
    if (!font) {
        fprintf(stderr, "Error open THE font\n");
        gameRunning = false;
        return;
    }

    gameRunning = true;
}

void Game::closeGame() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Game::setupGameAndObjects() {
    srand(SDL_GetTicks());

    // Setup slider
    slider.height     = 20;
    slider.width      = 120;
    slider.x          = (WINDOW_WIDTH/2) - (slider.width/2);  // Start from center
    slider.y          = WINDOW_HEIGHT-slider.height*3;
    slider.x_velocity = 0;

    // Setup ball 
    ball.height     = 15;
    ball.width      = 15;
    ball.x          = WINDOW_WIDTH/2;  // Start from center
    ball.y          = WINDOW_HEIGHT/2; // Start from center
    ball.x_velocity = rand()%2 ? -(BALL_MOV_DIS ) : BALL_MOV_DIS;
    ball.y_velocity = -(BALL_MOV_DIS);

    // Initialize vars
    score = 0;
    gameRunning = true;

}

void Game::pauseGame() {
    bool gamePaused = true;

    SDL_Texture* tex = IMG_LoadTexture(renderer, "./assets/paused.png");
    
    SDL_RenderCopy(renderer, tex, NULL, NULL);
    
    SDL_RenderPresent(renderer);

    while (gamePaused) {
        this->waitForNextFPS();
        SDL_Event event;
        SDL_PollEvent(&event);

        switch (event.type) {

            case SDL_QUIT:
                gamePaused = false;
                break;

            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    gamePaused = false;
                }
                else if (event.key.keysym.sym == SDLK_q) {
                    gamePaused = false;
                }
        }
    }
}

void Game::processInput() {
    SDL_Event event;
    SDL_PollEvent(&event);

    switch (event.type) {

        case SDL_QUIT:
            gameRunning = false;
            break;
    
        case SDL_KEYDOWN:

            if (event.key.keysym.sym == SDLK_d) {
                slider.x_velocity = SLIDER_MOV_DIS;
            }

            else if (event.key.keysym.sym == SDLK_a) {
                slider.x_velocity = -(SLIDER_MOV_DIS);
            }

            else if (event.key.keysym.sym == SDLK_ESCAPE) {
                Game::pauseGame();
            }

            else if (event.key.keysym.sym == SDLK_q) {
                gameRunning = false;
            }

            else if (event.key.keysym.sym == SDLK_r) {
                setupGameAndObjects();
            }
            break;

        case SDL_KEYUP:
            if (event.key.keysym.sym == SDLK_d) {
                    slider.x_velocity = 0;
            }
            
            else if (event.key.keysym.sym == SDLK_a) {
                    slider.x_velocity = 0;
            }
            break;

    }
}

void Game::waitForNextFPS() {
    int32_t timeToWait = FRAME_TARGET_TIME - (SDL_GetTicks() - lastFrameTime);

    if (timeToWait > 0 && timeToWait <= FRAME_TARGET_TIME) 
        SDL_Delay(timeToWait);

    deltaTime = (SDL_GetTicks() - lastFrameTime) / 1000.0f;
    lastFrameTime = SDL_GetTicks();
}

void Game::update() {
    // Movimento ball
    ball.x += ball.x_velocity;
    ball.y += ball.y_velocity;

    // Movimento slider
    if (slider.x + slider.x_velocity * deltaTime >= -10 && slider.x + slider.x_velocity * deltaTime <= WINDOW_WIDTH-slider.width+10)
        slider.x += slider.x_velocity * deltaTime;

    srand(SDL_GetTicks());
    // Toccato Bordo Alto
    if (ball.y <= 0) {
        ball.y_velocity *= -1;
        ball.x_velocity *= rand()%2 ? -1 : +1;
        srand(SDL_GetTicks());
    }

    // Toccato Bordo Basso
    else if (ball.y + ball.height>= WINDOW_HEIGHT) {
        setupGameAndObjects();
        srand(SDL_GetTicks());
    }

    // Toccato Bordo Sinitro
    if (ball.x <= 0) {
        ball.x_velocity *= -1;
        ball.y_velocity *= rand()%2 ? -1 : +1;
        srand(SDL_GetTicks());
    }
    
    // Toccato Bordo Destro
    else if (ball.x + ball.width>= WINDOW_WIDTH) {
        ball.x_velocity *= -1;
        ball.y_velocity *= rand()%2 ? -1 : +1;
        srand(SDL_GetTicks());
    }

    // Toccato Slider
    if (ball.y + ball.height >= slider.y && ball.x + ball.width >= slider.x && ball.x <= slider.x + slider.width) {
        score++;
        ball.y_velocity *= -1;
        ball.x_velocity *= rand()%2 ? -1 : +1;

    // Incrementare la velocita' della pallina
        ball.x_velocity = ball.x_velocity > 0 ? ball.x_velocity+1 : ball.x_velocity-1;
    }
}

void Game::render() {

    // Set background light blue
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    sprintf(scoreString, " %u ", score);

    this->drawText(scoreString, WINDOW_WIDTH/2-40, 0, 80, 100, 0, 0, 0);

    SDL_Rect SDL_slider = {slider.x, slider.y, slider.width, slider.height};

    // Set slider blue
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &SDL_slider);

    SDL_Rect SDL_ball = {ball.x, ball.y, ball.width, ball.height};

    // Set ball black
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderFillRect(renderer, &SDL_ball);

	// Show new frame
	SDL_RenderPresent(renderer);
}

void Game::drawText(const char* p_text, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t r, uint8_t g, uint8_t b) {

    SDL_Color color = {r, g, b};
    SDL_Rect textInfos = {x, y, w, h};

    SDL_Surface* surText = TTF_RenderText_Solid(font, p_text, color);
    SDL_Texture* texText = SDL_CreateTextureFromSurface(renderer, surText);
    SDL_FreeSurface(surText);

    SDL_RenderCopy(renderer, texText, NULL, &textInfos);
    SDL_DestroyTexture(texText);
}

void Game::gh0Encryption(char* str, char mode) {
    uint16_t toShift = SDL_strlen(str);

    if (mode == 'e') 
        for (uint16_t i=0; i < toShift; i++)
            str[i] += toShift;
    else if (mode == 'd')
        for (uint16_t i=0; i < toShift; i++)
            str[i] -= toShift;
}

