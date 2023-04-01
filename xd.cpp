#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

const int width = 400;
const int height = 400;
const int gridSize = 20;
const int nCols = width / gridSize;
const int nRows = height / gridSize;
const int nBombs = 30;

// Stan pola planszy
enum class FieldState {
    Hidden, // pole nieodkryte
    Flagged, // pole oznaczone flagą
    Revealed, // pole odkryte
    Bomb // pole z bombą
};

// Pole planszy
struct Field {
    FieldState state = FieldState::Hidden;
    bool hasBomb = false;
    int nNeighbours = 0;
    sf::RectangleShape shape;
};

std::vector<std::vector<Field>> board(nCols, std::vector<Field>(nRows));

// Funkcja zliczająca sąsiadów z bombami dla każdego pola
void countNeighbours() {
    for (int i = 0; i < nCols; i++) {
        for (int j = 0; j < nRows; j++) {
            // Sprawdzamy każde pole planszy
            int count = 0;
            if (!board[i][j].hasBomb) {
                // Pola z bombami już mają określoną liczbę sąsiadów z bombami
                if (i > 0 && j > 0 && board[i - 1][j - 1].hasBomb) count++;
                if (j > 0 && board[i][j - 1].hasBomb) count++;
                if (i < nCols - 1 && j > 0 && board[i + 1][j - 1].hasBomb) count++;
                if (i > 0 && board[i - 1][j].hasBomb) count++;
                if (i < nCols - 1 && board[i + 1][j].hasBomb) count++;
                if (i > 0 && j < nRows - 1 && board[i - 1][j + 1].hasBomb) count++;
                if (j < nRows - 1 && board[i][j + 1].hasBomb) count++;
                if (i < nCols - 1 && j < nRows - 1 && board[i + 1][j + 1].hasBomb) count++;
                board[i][j].nNeighbours = count;
            }
        }
    }
}

// Funkcja inicjująca planszę
void initBoard() {
    // Losowo rozmieszczamy bomby na planszy
    std::srand(std::time(nullptr));
    for (int i = 0; i < nBombs; i++) {
        int x = std::rand() % nCols;
        int y = std::rand() % nRows;
        if (!board[x][y].hasBomb) {
            board[x][y].hasBomb = true;
        }
        else {
            // Powtarzamy, jeśli wylosowano pole z już rozmieszczoną bombą
            i--;
        }
    }
    countNeighbours();

    // Tworzymy kształty graficzne pól planszy
    for (int i = 0; i < nCols; i++) {
        for (int j = 0; j < nRows; j++) {
        // Kształt pola
        sf::RectangleShape shape(sf::Vector2f(gridSize, gridSize));
        shape.setPosition(i * gridSize, j * gridSize);
        shape.setOutlineThickness(1.f);
        shape.setOutlineColor(sf::Color::Black);
        shape.setFillColor(sf::Color::White);

        board[i][j].shape = shape;
    }
}
}

// Funkcja odkrywająca pole planszy
void revealField(int x, int y) {
    // Jeśli pole jest ukryte
    if (board[x][y].state == FieldState::Hidden) {
    // Jeśli pole ma bombę, gra kończy się porażką
        if (board[x][y].hasBomb) {
             std::cout << "Przegrałeś!\n";
            // Odkrywamy wszystkie pola z bombami
             for (int i = 0; i < nCols; i++) {
                    for (int j = 0; j < nRows; j++) {
                        if (board[i][j].hasBomb) {
                            board[i][j].shape.setFillColor(sf::Color::Red);
                            board[i][j].state = FieldState::Revealed;
                    }
                    }
            }
        }else {
            // Jeśli pole nie ma bomby, odkrywamy je i przyległe pola bez bomb
            board[x][y].state = FieldState::Revealed;
            board[x][y].shape.setFillColor(sf::Color::Yellow);
            if (board[x][y].nNeighbours == 0) {
                if (x > 0 && y > 0) revealField(x - 1, y - 1);
                if (y > 0) revealField(x, y - 1);
                if (x < nCols - 1 && y > 0) revealField(x + 1, y - 1);
                if (x > 0) revealField(x - 1, y);
                if (x < nCols - 1) revealField(x + 1, y);
                if (x > 0 && y < nRows - 1) revealField(x - 1, y + 1);
                if (y < nRows - 1) revealField(x, y + 1);
                if (x < nCols - 1 && y < nRows - 1) revealField(x + 1, y + 1);
            }
        }
    }
}

int main()
{
sf::RenderWindow window(sf::VideoMode(width, height), "Saper");
initBoard();

while (window.isOpen())
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            window.close();
        else if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                int x = event.mouseButton.x / gridSize;
                int y = event.mouseButton.y / gridSize;
                revealField(x, y);
            }
            else if (event.mouseButton.button == sf::Mouse::Right) {
                int x = event.mouseButton.x / gridSize;
                int y = event.mouseButton.y / gridSize;
                if (board[x][y].state == FieldState::Hidden) {
                    board[x][y].state = FieldState::Flagged;
                    board[x][y].shape.setFillColor(sf::Color::Blue);
                }
                else

                if (board[x][y].state == FieldState::Flagged) {
                    board[x][y].state = FieldState::Hidden;
                    board[x][y].shape.setFillColor(sf::Color::White);
                }
            }
        }
    }
    window.clear();
    // Rysujemy planszę
    for (int i = 0; i < nCols; i++) {
        for (int j = 0; j < nRows; j++) {
            window.draw(board[i][j].shape);
        }
    }
    window.display();
}

return 0;
}   