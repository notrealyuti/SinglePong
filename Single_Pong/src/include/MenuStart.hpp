

class MenuStart {

    public:
        SDL_Renderer* renderer;
        Game* game;
        bool wantQuit = false;
        bool startGame = false;
        SDL_Texture* tex_startMenuBackground;
        
        struct ball_t menuBall = {640, 360, 20, 20, 5, -5};

        void run(Game* p_game);
        void processInputs();
        void update();
        void render(); 

};


void MenuStart::run(Game* p_game) {
    game = p_game;

    tex_startMenuBackground = IMG_LoadTexture(game->renderer, "./assets/startMenu.png");
}



void MenuStart::processInputs() {
    SDL_Event event;
    SDL_PollEvent(&event);

    switch (event.type) {

        case SDL_QUIT:
            wantQuit = true;
            break;

        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_SPACE) {
                startGame = true;
                wantQuit = true;
            } 
            else if (event.key.keysym.sym == SDLK_q) {
                wantQuit = true;
            }

            break;
    }
}

void MenuStart::update() {

    menuBall.x += menuBall.x_velocity;
    menuBall.y += menuBall.y_velocity;

    srand(SDL_GetTicks());
    // Toccato Bordo Alto
    if (!menuBall.y) {
        menuBall.y_velocity *= -1;
        menuBall.x_velocity *= rand()%2 ? -1 : +1;
        srand(SDL_GetTicks());
    }

    // Toccato Bordo Basso
    else if (menuBall.y == game->WINDOW_HEIGHT) {
        srand(SDL_GetTicks());
        menuBall.y_velocity *= -1;
        menuBall.x_velocity *= rand()%2 ? -1 : +1;
    }

    // Toccato Bordo Sinitro
    if (!menuBall.x) {
        menuBall.x_velocity *= -1;
        menuBall.y_velocity *= rand()%2 ? -1 : +1;
        srand(SDL_GetTicks());
    }
    
    // Toccato Bordo Destro
    else if (menuBall.x == game->WINDOW_WIDTH-menuBall.width+10) {
        menuBall.x_velocity *= -1;
        menuBall.y_velocity *= rand()%2 ? -1 : +1;
        srand(SDL_GetTicks());
    }
}

void MenuStart::render() {
    SDL_SetRenderDrawColor(game->renderer, 255, 255, 255, 255);
    SDL_RenderClear(game->renderer);

    SDL_Rect sas = {menuBall.x, menuBall.y, menuBall.width, menuBall.height};

    SDL_SetRenderDrawColor(game->renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(game->renderer, &sas);


    SDL_RenderCopy(game->renderer, tex_startMenuBackground, NULL, NULL);
    SDL_RenderPresent(game->renderer);
}