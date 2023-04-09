#include "Game.hpp"

void Game::initVariables(){
    this->window=nullptr;
    this->gridSize=20;
    this->height=400;
    this->width=400;

    this->vidMode.width=width;
    this->vidMode.height=height;
    this->window=new sf::RenderWindow(this->vidMode, "Saper",sf::Style::Titlebar | sf::Style::Close);
    
    this->columns = (vidMode.width) / gridSize;
    this->rows = (vidMode.height) / gridSize;
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
                    }else{
                        if (ev.mouseButton.button == sf::Mouse::Left) {
                            int x = ev.mouseButton.x / gridSize;
                            int y = ev.mouseButton.y / gridSize;
                            revealField(x, y);
                        }else if (ev.mouseButton.button == sf::Mouse::Right) {
                            int x = ev.mouseButton.x / gridSize;
                            int y = ev.mouseButton.y / gridSize;
                            if ((*board)[x][y].fieldState == FieldState::Hidden) {
                                (*board)[x][y].fieldState = FieldState::Flagged;
                                (*board)[x][y].shape.setFillColor(sf::Color::Blue);
                            }
                            else if ((*board)[x][y].fieldState == FieldState::Flagged) {
                                (*board)[x][y].fieldState = FieldState::Hidden;
                                (*board)[x][y].shape.setFillColor(sf::Color::White);
                             }
                        }
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
        
            for (int j = 0; j < rows; j++) {
                  for (int i = 0; i < columns; i++) {
                      window->draw((*board)[i][j].shape);
                
                }
            }       
    }
    if(this->gameState==GameState::Options){
        //TODO Kod wyświetlania opcji
    }
    if(this->gameState==GameState::GameOver){
        //TODO kod do wyświetlania game over
        
            for (int j = 0; j < rows; j++) {
                  for (int i = 0; i < columns; i++) {
                      window->draw((*board)[i][j].shape);
                
                }
            }       

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
void Game::revealField(int x, int y) {
    // Jeśli pole jest ukryte
    if ((*board)[x][y].fieldState == FieldState::Hidden) {
        if ((*board)[x][y].hasBomb) {
             //std::cout << "Przegrałeś!\n";
             for (int i = 0; i < columns; i++) {
                    for (int j = 0; j < rows; j++) {
                        if ((*board)[i][j].hasBomb) {
                            (*board)[i][j].shape.setFillColor(sf::Color::Red);
                            (*board)[i][j].fieldState = FieldState::Revealed;
                    }
                    }
            }
        }else {
            // Jeśli pole nie ma bomby, odkrywamy je i przyległe pola bez bomb
            (*board)[x][y].fieldState = FieldState::Revealed;
            (*board)[x][y].shape.setFillColor(sf::Color::Yellow);
            if ((*board)[x][y].nNeighbours == 0) {
                if (x > 0 && y > 0) revealField(x - 1, y - 1);
                if (y > 0) revealField(x, y - 1);
                if (x < columns - 1 && y > 0) revealField(x + 1, y - 1);
                if (x > 0) revealField(x - 1, y);
                if (x < columns - 1) revealField(x + 1, y);
                if (x > 0 && y < rows - 1) revealField(x - 1, y + 1);
                if (y < rows - 1) revealField(x, y + 1);
                if (x < columns - 1 && y < rows - 1) revealField(x + 1, y + 1);
            }
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