#include <iostream>
#include "Game.hpp"


int main(){
    Game game;
    game.initBoard();
    while(game.running()){

        game.update();
        game.render();



    }
}
