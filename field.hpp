
enum class FieldState{
    Flagged,
    Hidden,
    Bomb,
    Revealed
};

struct Field{
    FieldState state = FieldState::Hidden;
    bool hasBomb = false;
    int nNeighbours = 0;
    sf::RectangleShape shape;
}