#include <iostream>
#include "Game.hpp"


int main(){
    Game game;

    while(game.running()){

        game.update();
        game.render();



    }
}
