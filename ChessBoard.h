#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <SFML/Graphics.hpp> // Dodano nagłówek SFML do obsługi grafiki
#include <iostream> // Dodano nagłówek do wejścia/wyjścia
#include <string> // Dodano nagłówek do obsługi stringów
#include <vector> // Dodano nagłówek do obsługi wektorów
#include <map> // Dodano nagłówek do obsługi map
#include <sstream> // Dodano nagłówek do operacji na string stream
#include <fstream> // Dodano nagłówek do obsługi plików

class ChessBoard
{
public:
    enum class Turn
    {
        white, // Tura białych
        black  // Tura czarnych
    } turn; // Aktualna tura

    enum class Piece
    {
        king, // Król
        queen, // Królowa
        white_pawn, // Biały pionek
        black_pawn, // Czarny pionek
        rook, // Wieża
        bishop, // Goniec
        knight // Skoczek
    };

    static std::map<Piece, int> pieceValues; // Mapa wartości punktowych dla poszczególnych figur
    bool show_coordinates = false; // Flaga do wyświetlania współrzędnych

    struct Pos
    {
        int x, y; // Współrzędne na szachownicy
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

    std::map<Pos, Piece> white_pieces, black_pieces; // Mapy pozycji i figur białych i czarnych
    std::map<Pos, Piece> &moverPieces() { return turn == Turn::white ? white_pieces : black_pieces; } // Zwraca mapę figur aktualnego gracza
    std::map<Pos, Piece> &opponentPieces() { return turn == Turn::white ? black_pieces : white_pieces; } // Zwraca mapę figur przeciwnika

    std::vector<std::string> move_history; // Kontener do przechowywania historii ruchów

    void reset(); // Resetuje szachownicę do stanu początkowego
    void flipTurn(); // Zmienia turę na przeciwną
    bool isCheck(Turn t); // Sprawdza, czy dany gracz jest w szachu
    bool isCheckmate(); // Sprawdza, czy jest mat
    bool makeMove(Pos from, Pos to); // Wykonuje ruch
    bool isMoveValid(Pos from, Pos to); // Sprawdza, czy ruch jest legalny
    std::vector<Pos> possibleMoves(const Pos &from); // Zwraca możliwe ruchy dla danej pozycji
    void drawBoard(sf::RenderWindow &window, sf::Texture &light_square_texture, sf::Texture &dark_square_texture, std::map<Piece, sf::Texture> &white_piece_textures, std::map<Piece, sf::Texture> &black_piece_textures, sf::Font &font); // Rysuje szachownicę
    bool print(); // Pyta użytkownika o ruch
    int score(); // Oblicza wynik gry
    bool parseMove(std::string move, Pos &from, Pos &to); // Parsuje ruch z formatu string

    struct Move
    {
        Pos from, to; // Pozycje początkowa i końcowa
        int score; // Wynik ruchu
    };

    Move minimax(int depth, bool minimize); // Algorytm minimax do podejmowania decyzji przez AI
    void AIMove(); // Wykonuje ruch AI
    void playerMove(); // Wykonuje ruch gracza
    bool checkGameOver(); // Sprawdza, czy gra się zakończyła
    std::string getWinner(); // Zwraca zwycięzcę
    void printMoves(); // Wyświetla historię ruchów

};

#endif // CHESSBOARD_H
