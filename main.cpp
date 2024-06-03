#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include "ChessBoard.h"

int main()
{
    ChessBoard game;
    std::cout << std::endl
              << "* Chesscomputer v.0.1" << std::endl;
    game.printHelp();

    bool gameon = true;
    game.reset();
    std::string pick_side = "";
    while (pick_side != "b" && pick_side != "w" && pick_side != "q")
    {
        std::cout << std::endl
                  << "Play as (b)lack or (w)hite or (q)uit? ";
        std::cin >> pick_side;
    }
    if (pick_side == "q")
    {
        std::cout << "Good bye." << std::endl;
        return 0;
    }

    if (pick_side == "b")
        game.AIMove();
    sf::RenderWindow window(sf::VideoMode(900, 900), "Chess");

    sf::Texture light_square_texture;
    sf::Texture dark_square_texture;
    std::map<ChessBoard::Piece, sf::Texture> white_piece_textures;
    std::map<ChessBoard::Piece, sf::Texture> black_piece_textures;
    sf::Font font;

    // Wczytywanie tekstur i czcionek
    if (!light_square_texture.loadFromFile("images/square_light.png") ||
        !dark_square_texture.loadFromFile("images/square_dark.png") ||
        !white_piece_textures[ChessBoard::Piece::white_pawn].loadFromFile("images/wP.png") ||
        !black_piece_textures[ChessBoard::Piece::black_pawn].loadFromFile("images/bP.png") ||
        !white_piece_textures[ChessBoard::Piece::rook].loadFromFile("images/wR.png") ||
        !black_piece_textures[ChessBoard::Piece::rook].loadFromFile("images/bR.png") ||
        !white_piece_textures[ChessBoard::Piece::knight].loadFromFile("images/wN.png") ||
        !black_piece_textures[ChessBoard::Piece::knight].loadFromFile("images/bN.png") ||
        !white_piece_textures[ChessBoard::Piece::bishop].loadFromFile("images/wB.png") ||
        !black_piece_textures[ChessBoard::Piece::bishop].loadFromFile("images/bB.png") ||
        !white_piece_textures[ChessBoard::Piece::king].loadFromFile("images/wK.png") ||
        !black_piece_textures[ChessBoard::Piece::king].loadFromFile("images/bK.png") ||
        !white_piece_textures[ChessBoard::Piece::queen].loadFromFile("images/wQ.png") ||
        !black_piece_textures[ChessBoard::Piece::queen].loadFromFile("images/bQ.png") ||
        !font.loadFromFile("images/arial.ttf"))
    {
        std::cout << "Error loading textures or font!" << std::endl;
        return 1;
    }

    // Pętla zdarzeń
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        game.drawBoard(window, light_square_texture, dark_square_texture, white_piece_textures, black_piece_textures, font);
        window.display();

        if (gameon)
        {
            if (pick_side == "b")
            {
                if (game.turn == ChessBoard::Turn::white)
                {
                    // Tura użytkownika
                    if (!game.promptInput())
                        gameon = false;
                }
                else
                {
                    // Tura AI
                    game.AIMove();
                }
            }
            else
            {
                if (game.turn == ChessBoard::Turn::black)
                {
                    // Tura AI
                    game.AIMove();
                }
                else
                {
                    // Tura użytkownika
                    if (!game.promptInput())
                        gameon = false;
                }
            }

            if (!game.hasKing())
            {
                if (game.turn == ChessBoard::Turn::black)
                    std::cout << "* You are victorious!" << std::endl;
                else
                    std::cout << "* You are defeated!" << std::endl;
                gameon = false;
            }
        }
    }

    return 0;
}
