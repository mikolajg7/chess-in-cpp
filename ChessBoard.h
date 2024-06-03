#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;
class ChessBoard
{
public:
    enum class Turn
    {
        white,
        black
    } turn;

    enum class Piece
    {
        king,
        queen,
        white_pawn,
        black_pawn,
        rook,
        bishop,
        knight
    };

    static std::map<Piece, int> pieceValues;
    bool show_coordinates = false;

    struct Pos
    {
        int x, y;
        Pos(const Pos &p, int dx = 0, int dy = 0)
        {
            *this = p;
            x += dx;
            y += dy;
        }
        Pos(int _x, int _y)
        {
            x = _x;
            y = _y;
        }
        bool operator<(const Pos &p) const { return (x < p.x) || (x == p.x && y < p.y); }
        bool operator==(const Pos &p) const { return x == p.x && y == p.y; }
        Pos()
        {
            x = -1;
            y = -1;
        }
    };

    void reset();
    void flipTurn();
    bool makeMove(Pos from, Pos to);
    bool isMoveValid(Pos from, Pos to);
    bool isInCheck(Turn t);
    std::vector<Pos> possibleMoves(const Pos &from);
    void drawBoard(sf::RenderWindow &window, sf::Texture &light_square_texture, sf::Texture &dark_square_texture, std::map<Piece, sf::Texture> &white_piece_textures, std::map<Piece, sf::Texture> &black_piece_textures, sf::Font &font);
    void printHelp();
    bool promptInput();
    int score();
    bool hasKing();

    struct Move
    {
        Pos from, to;
        int score;
    };

    Move minimax(int depth, bool minimize);
    void AIMove();

    std::map<Pos, Piece> white_pieces, black_pieces;
    std::map<Pos, Piece> &moverPieces() { return turn == Turn::white ? white_pieces : black_pieces; }
    std::map<Pos, Piece> &opponentPieces() { return turn == Turn::white ? black_pieces : white_pieces; }
};

#endif // CHESSBOARD_H
