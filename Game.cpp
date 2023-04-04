#include "Game.hpp"

void Game::initVariables(){
    this->window=nullptr;
    this->gridSize=20;
    this->height=400;
    this->width=400;

    this->vidMode.width=width;
    this->vidMode.height=height;
    this->window=new sf::RenderWindow(this->vidMode, "Saper",sf::Style::Titlebar | sf::Style::Close);
    
    this->columns = vidMode.width / gridSize;
    this->rows = vidMode.height / gridSize;
    this->bombRatio=0.1;
    int numberOfBombs = static_cast<int>(columns * rows * bombRatio);
    this->board = new std::vector<std::vector<Field>>(columns, std::vector<Field>(rows));
    
    
}
// void Game::initWindow(){
//     this->vidMode.width=width;
//     this->vidMode.height=height;
//     this->window=new sf::RenderWindow(this->vidMode, "Saper",sf::Style::Titlebar | sf::Style::Close);
    
// }
Game::Game(){
    this->initVariables();
    //this->initWindow();
}

Game::~Game(){
    delete this->window;
    delete this->board;
}

const bool Game::running() const{
    return this->window->isOpen();
}

void Game::pollEvents(){
    while(this->window->pollEvent(this->ev)){
        switch(ev.type){
                case sf::Event::Closed:
                    this->window->close();
                    break;
                case sf::Event::KeyPressed:
                    if(this->ev.key.code == sf::Keyboard::Escape){
                        this->window->close();
                    }    
                    break;
                }
    }
}    

void Game::update(){
    this->pollEvents();
}
void Game::render(){
    this->window->clear();

    if(this->gameState==GameState::Gameplay){
        printf("%d",columns);
            for (int j = 0; j < rows; j++) {
                  for (int i = 0; i < columns; i++) {
                      window->draw((*boar)[i][j].shape);
                
                }
            }       
    }
    if(this->gameState==GameState::Options){
        //TODO Kod wyświetlania opcji
    }
    if(this->gameState==GameState::GameOver){
        //TODO kod do wyświetlania game over
    }

    window->display();
}

void Game::countNeighBombs(){
    int count=0;
    for(int i=0;i<columns;i++){
        for(int j=0;j<rows;j++){
            if(!(*(this->board))[i][j].hasBomb){
                if (i > 0 && j > 0 && (*board)[i - 1][j - 1].hasBomb) count++;
                if (j > 0 && (*board)[i][j - 1].hasBomb) count++;
                if (i < columns - 1 && j > 0 && (*board)[i + 1][j - 1].hasBomb) count++;
                if (i > 0 && (*board)[i - 1][j].hasBomb) count++;
                if (i < columns - 1 && (*board)[i + 1][j].hasBomb) count++;
                if (i > 0 && j < rows - 1 && (*board)[i - 1][j + 1].hasBomb) count++;
                if (j < rows - 1 && (*board)[i][j + 1].hasBomb) count++;
                if (i < columns - 1 && j < rows - 1 && (*board)[i + 1][j + 1].hasBomb) count++;
                

                (*(this->board))[i][j].nNeighbours=count;
            }
            count=0;
        }
    }
}

void Game::initBoard(){
    std::srand(std::time(NULL));
    int x;
    int y;
    for(int i=0;i<numberOfBombs;i++){
        int x = std::rand() % columns;
        int y = std::rand() % rows;
        if((*(this->board))[x][y].hasBomb){
            i--;
        }else{
            (*(this->board))[x][y].hasBomb=true;
        } 
    }
    countNeighBombs();
    
    for(int i=0;i<columns;i++){
        for(int j=0;j<rows;j++){
    
            sf::RectangleShape shape(sf::Vector2f(rows, columns));
            shape.setPosition(i * gridSize, j * gridSize);
            shape.setOutlineThickness(1.f);
            shape.setOutlineColor(sf::Color::Black);
            shape.setFillColor(sf::Color::White);
            (*board)[i][j].shape = shape;

        }
    }    

}