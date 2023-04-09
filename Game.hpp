#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdlib>
enum class FieldState {
    Hidden, // pole nieodkryte
    Flagged, // pole oznaczone flagą
    Revealed, // pole odkryte
    Bomb // pole z bombą
};

enum class GameState{
    Options,
    Gameplay,
    GameOver
};

// Pole planszy
class Field {
    public:
    FieldState fieldState = FieldState::Hidden;
    bool hasBomb = false;
    int nNeighbours = 0;
    sf::RectangleShape shape;
};

class Game{
private:
    GameState gameState = GameState::Gameplay;
    sf::RenderWindow* window;
    sf::VideoMode vidMode;
    sf::Event ev;
    void countNeighBombs();
    void initVariables();
    void initWindow();
    int gridSize; //rozmiar kafelka
    int height;
    int width;
    int columns;  //ilosc kolumn
    int rows;     //ilosc rzedow
    int numberOfBombs;
    float bombRatio;
    std::vector<std::vector<Field>>* board;
    void revealField(int x,int y);
    //std::vector<std::vector<Field>> board(columns, std::vector<Field>(rows));
public:
    Game();
    virtual ~Game();
    const bool running() const;
    void pollEvents();
    void update();
    void render();
    void initBoard();
};

