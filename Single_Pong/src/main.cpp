#include "./include/Game.hpp"
#include "./include/MenuStart.hpp"

void clearStr(char* str, uint32_t size) {for (; size; size--, str[size] = '\0');}

int main(int argc, char** args) {

    Game*      game      = new Game("Single Pong 1.0", 1920, 1080);
    MenuStart* menuStart = new MenuStart;

    game->setupWindow();
    game->setupGameAndObjects();


    menuStart->run(game);
    while (!menuStart->wantQuit) {
        menuStart->processInputs();
        menuStart->update();
        menuStart->render();
        game->waitForNextFPS();
    }

    game->gameRunning = menuStart->startGame;
   
	time_t now = time(0);
	uint32_t fpsCount = 0;
    char fpsCountChar[7];
	
	
	while (game->gameRunning) {
        game->processInput();
        game->update();
        game->render();
        
		if (time(0) - now) {
			now = time(0);
			clearStr(fpsCountChar, sizeof(fpsCountChar));
			sprintf(fpsCountChar, "%li", fpsCount);
			game->drawText(fpsCountChar, game->WINDOW_WIDTH, game->WINDOW_HEIGHT, 30, 30, 0, 0, 0);
			fpsCount = 0;
		}
		
		
		game->waitForNextFPS();
    }    
    game->closeGame();

    delete game;


    return 0;
}

