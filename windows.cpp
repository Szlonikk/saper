#include "windows.hpp"
#include <SFML/Graphics.hpp>

enum GameState { OPTIONS, GAMEPLAY, GAMEOVER };
GameState state = OPTIONS;


void DisplayMainWindow(){
    sf::RenderWindow window(sf::VideoMode(width, height), "Saper",sf::Style::TitleBar | sf::Style::Close);
    sf::Event evListener;
    while(window.isOpen()){ 
        while(window.pollEvent(evListener)){
            switch(evListener.type){
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::KeyPressed:
                    if(ev.key.code == sf::Keyboard::Escape)
                        window.close();

                    break;
            }
        }
    }
    
    window.clear(sf::Color::Grey);
    window.display();

}