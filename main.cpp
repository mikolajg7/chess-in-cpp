#include "ChessBoard.h" // Dołączenie nagłówka z logiką szachownicy
#include <sstream>  // Dołączenie nagłówka dla operacji na string stream

int main()
{
    ChessBoard game; // Utworzenie obiektu ChessBoard do zarządzania logiką gry
    game.reset(); // Inicjalizacja gry do pozycji startowej

    sf::RenderWindow window(sf::VideoMode(900, 900), "Chess"); // Utworzenie okna dla gry szachowej o określonych wymiarach

    sf::Texture light_square_texture; // Tekstura dla jasnych pól na szachownicy
    sf::Texture dark_square_texture; // Tekstura dla ciemnych pól na szachownicy
    std::map<ChessBoard::Piece, sf::Texture> white_piece_textures; // Mapa przechowująca tekstury białych figur
    std::map<ChessBoard::Piece, sf::Texture> black_piece_textures; // Mapa przechowująca tekstury czarnych figur
    sf::Font font; // Czcionka do wyświetlania tekstu

    // Wczytywanie tekstur i sprawdzanie błędów
    if (!light_square_texture.loadFromFile("images/square_light.png") || // Wczytanie tekstury jasnego pola
        !dark_square_texture.loadFromFile("images/square_dark.png") || // Wczytanie tekstury ciemnego pola
        !white_piece_textures[ChessBoard::Piece::white_pawn].loadFromFile("images/wP.png") || // Wczytanie tekstury białego pionka
        !black_piece_textures[ChessBoard::Piece::black_pawn].loadFromFile("images/bP.png") || // Wczytanie tekstury czarnego pionka
        !white_piece_textures[ChessBoard::Piece::rook].loadFromFile("images/wR.png") || // Wczytanie tekstury białej wieży
        !black_piece_textures[ChessBoard::Piece::rook].loadFromFile("images/bR.png") || // Wczytanie tekstury czarnej wieży
        !white_piece_textures[ChessBoard::Piece::knight].loadFromFile("images/wN.png") || // Wczytanie tekstury białego konia
        !black_piece_textures[ChessBoard::Piece::knight].loadFromFile("images/bN.png") || // Wczytanie tekstury czarnego konia
        !white_piece_textures[ChessBoard::Piece::bishop].loadFromFile("images/wB.png") || // Wczytanie tekstury białego gońca
        !black_piece_textures[ChessBoard::Piece::bishop].loadFromFile("images/bB.png") || // Wczytanie tekstury czarnego gońca
        !white_piece_textures[ChessBoard::Piece::king].loadFromFile("images/wK.png") || // Wczytanie tekstury białego króla
        !black_piece_textures[ChessBoard::Piece::king].loadFromFile("images/bK.png") || // Wczytanie tekstury czarnego króla
        !white_piece_textures[ChessBoard::Piece::queen].loadFromFile("images/wQ.png") || // Wczytanie tekstury białej królowej
        !black_piece_textures[ChessBoard::Piece::queen].loadFromFile("images/bQ.png") || // Wczytanie tekstury czarnej królowej
        !font.loadFromFile("images/arial.ttf")) // Wczytanie czcionki
    {
        std::cout << "Error loading textures or font!" << std::endl; // Wyświetlenie błędu, jeśli wczytanie się nie powiodło
        return 1; // Zakończenie programu w przypadku błędu
    }

    // Pętla zdarzeń
    while (window.isOpen()) // Dopóki okno jest otwarte
    {
        sf::Event event; // Utworzenie obiektu zdarzenia
        while (window.pollEvent(event)) // Pętla sprawdzająca wszystkie zdarzenia
        {
            if (event.type == sf::Event::Closed) // Jeśli zdarzenie to zamknięcie okna
                window.close(); // Zamknięcie okna
        }

        window.clear(); // Wyczyść okno
        game.drawBoard(window, light_square_texture, dark_square_texture, white_piece_textures, black_piece_textures, font); // Rysowanie szachownicy i figur
        window.display(); // Wyświetlenie zawartości okna

        if (!game.checkGameOver()) // Jeśli gra się nie skończyła
        {
            game.playerMove(); // Wykonaj ruch gracza
            game.printMoves(); // Wyświetl ruchy
            if (game.checkGameOver()) // Sprawdź, czy gra się skończyła
            {
                std::cout << game.getWinner() << std::endl; // Wyświetl zwycięzcę
                window.close(); // Zamknij okno
            }
        }
    }
    return 0; // Zakończenie programu
}
