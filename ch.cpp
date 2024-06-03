//#include <SFML/Graphics.hpp>
//#include <iostream>
//#include <string>
//#include <vector>
//#include <map>
//#include <algorithm>
//
//using namespace std;
//
//// Struktura reprezentująca szachownicę
//struct ChessBoard
//{
//    // Enumaracja reprezentująca turę gracza (biały lub czarny)
//    enum class Turn
//    {
//        white,
//        black
//    } turn;
//
//    // Enumaracja reprezentująca typy bierek
//    enum class Piece
//    {
//        king,
//        queen,
//        white_pawn,
//        black_pawn,
//        rook,
//        bishop,
//        knight
//    };
//
//    // Mapa przechowująca wartości punktowe poszczególnych bierek
//    static map<Piece, int> pieceValues;
//    bool show_coordinates = false; // Flaga do wyświetlania współrzędnych na szachownicy
//
//    // Struktura reprezentująca pozycję na szachownicy
//    struct Pos
//    {
//        int x, y;
//        Pos(const Pos &p, int dx = 0, int dy = 0)
//        {
//            *this = p;
//            x += dx;
//            y += dy;
//        }
//        Pos(int _x, int _y)
//        {
//            x = _x;
//            y = _y;
//        }
//        bool operator<(const Pos &p) const { return (x < p.x) || (x == p.x && y < p.y); }
//        bool operator==(const Pos &p) const { return x == p.x && y == p.y; }
//        Pos()
//        {
//            x = -1;
//            y = -1;
//        }
//    };
//
//    // Mapy przechowujące pozycje białych i czarnych bierek
//    map<Pos, Piece> white_pieces, black_pieces;
//
//    // Funkcje zwracające mapy bierek aktualnego gracza i przeciwnika
//    map<Pos, Piece> &moverPieces() { return turn == Turn::white ? white_pieces : black_pieces; }
//    map<Pos, Piece> &opponentPieces() { return turn == Turn::white ? black_pieces : white_pieces; }
//
//    // Funkcja resetująca szachownicę do stanu początkowego
//    void reset()
//    {
//        turn = Turn::white;
//        white_pieces.clear();
//        black_pieces.clear();
//        for (int i = 1; i < 9; ++i)
//        {
//            white_pieces[Pos(i, 2)] = Piece::white_pawn;
//            black_pieces[Pos(i, 7)] = Piece::black_pawn;
//        }
//        int n = 1;
//        for (auto piece : {Piece::rook, Piece::knight, Piece::bishop, Piece::king})
//        {
//            white_pieces[Pos(n, 1)] = white_pieces[Pos(9 - n, 1)] = piece;
//            black_pieces[Pos(n, 8)] = black_pieces[Pos(9 - n, 8)] = piece;
//            ++n;
//        }
//        white_pieces[Pos(4, 1)] = Piece::queen;
//        black_pieces[Pos(4, 8)] = Piece::queen;
//    }
//
//    // Funkcja zmieniająca turę gracza
//    void flipTurn() { turn = turn == Turn::white ? Turn::black : Turn::white; }
//
//    // Funkcja wykonująca ruch bierką z pozycji 'from' na pozycję 'to'
//    bool makeMove(Pos from, Pos to)
//    {
//        vector<Pos> allowed = possibleMoves(from);
//        if (find(allowed.begin(), allowed.end(), to) == allowed.end())
//            return false;
//        opponentPieces().erase(to);
//        moverPieces()[to] = moverPieces()[from];
//        moverPieces().erase(from);
//        if ((moverPieces()[to] == Piece::white_pawn || moverPieces()[to] == Piece::black_pawn) && (to.y == 1 || to.y == 8))
//            moverPieces()[to] = Piece::queen;
//        flipTurn();
//        return true;
//    }
//
//    // Funkcja zwracająca możliwe ruchy dla bierki na pozycji 'from'
//    vector<Pos> possibleMoves(const Pos &from)
//    {
//        vector<Pos> moves;
//        auto isOwn = [&](int dx, int dy) -> bool
//        { return moverPieces().count(Pos(from, dx, dy)); };
//        auto isOpponent = [&](int dx, int dy) -> bool
//        { return opponentPieces().count(Pos(from, dx, dy)); };
//        auto isInsideBoard = [&](int dx, int dy) -> bool
//        { Pos p(from,dx,dy); return p.x < 9 && p.x > 0 && p.y < 9 && p.y > 0; };
//        auto isFree = [&](int dx, int dy) -> bool
//        { return !isOwn(dx, dy) && isInsideBoard(dx, dy) && !isOpponent(dx, dy); };
//
//        auto addMove = [&](int dx, int dy) -> bool
//        {
//            if (isFree(dx, dy) || isOpponent(dx, dy))
//            {
//                moves.push_back(Pos(from, dx, dy));
//                return true;
//            }
//            return false;
//        };
//
//        if (!isOwn(0, 0))
//            return moves;
//
//        auto moving_piece = moverPieces()[from];
//        switch (moving_piece)
//        {
//            case Piece::white_pawn:
//                if (isFree(0, 1))
//                    addMove(0, 1);
//                if (isFree(0, 1) && isFree(0, 2) && from.y == 2)
//                    addMove(0, 2);
//                if (isOpponent(-1, 1))
//                    addMove(-1, 1);
//                if (isOpponent(1, 1))
//                    addMove(1, 1);
//                break;
//
//            case Piece::black_pawn:
//                if (isFree(0, -1))
//                    addMove(0, -1);
//                if (isFree(0, -1) && isFree(0, -2) && from.y == 7)
//                    addMove(0, -2);
//                if (isOpponent(-1, -1))
//                    addMove(-1, -1);
//                if (isOpponent(1, -1))
//                    addMove(1, -1);
//                break;
//
//            case Piece::knight:
//                addMove(-2, -1);
//                addMove(-2, 1);
//                addMove(2, -1);
//                addMove(2, 1);
//                addMove(-1, -2);
//                addMove(-1, 2);
//                addMove(1, -2);
//                addMove(1, 2);
//                break;
//
//            case Piece::king:
//                for (auto dy : {-1, 0, 1})
//                    for (auto dx : {-1, 0, 1})
//                        addMove(dx, dy);
//                break;
//
//            case Piece::queen:
//            case Piece::rook:
//                for (int n = 1; n < 9 && addMove(0, n) && !isOpponent(0, n); ++n)
//                    ;
//                for (int n = 1; n < 9 && addMove(0, -n) && !isOpponent(0, -n); ++n)
//                    ;
//                for (int n = 1; n < 9 && addMove(n, 0) && !isOpponent(n, 0); ++n)
//                    ;
//                for (int n = 1; n < 9 && addMove(-n, 0) && !isOpponent(-n, 0); ++n)
//                    ;
//                if (moving_piece != Piece::queen)
//                    break;
//
//            case Piece::bishop:
//                for (int n = 1; n < 9 && addMove(n, n) && !isOpponent(n, n); ++n)
//                    ;
//                for (int n = 1; n < 9 && addMove(n, -n) && !isOpponent(n, -n); ++n)
//                    ;
//                for (int n = 1; n < 9 && addMove(-n, n) && !isOpponent(-n, n); ++n)
//                    ;
//                for (int n = 1; n < 9 && addMove(-n, -n) && !isOpponent(-n, -n); ++n)
//                    ;
//                break;
//        }
//
//        return moves;
//    }
//
//    // Funkcja rysująca szachownicę i bierki na oknie
//    void drawBoard(sf::RenderWindow &window, sf::Texture &light_square_texture, sf::Texture &dark_square_texture, map<Piece, sf::Texture> &white_piece_textures, map<Piece, sf::Texture> &black_piece_textures, sf::Font &font)
//    {
//        sf::Sprite light_square(light_square_texture);
//        sf::Sprite dark_square(dark_square_texture);
//
//        for (int y = 0; y < 8; y++)
//        {
//            for (int x = 0; x < 8; x++)
//            {
//                if ((x + y) % 2 == 0)
//                {
//                    light_square.setPosition(x * 100, y * 100);
//                    window.draw(light_square);
//                }
//                else
//                {
//                    dark_square.setPosition(x * 100, y * 100);
//                    window.draw(dark_square);
//                }
//
//                Pos pos(x + 1, 8 - y);
//                if (white_pieces.count(pos))
//                {
//                    sf::Sprite piece_sprite(white_piece_textures[white_pieces[pos]]);
//                    piece_sprite.setPosition(x * 100, y * 100);
//                    window.draw(piece_sprite);
//                }
//                else if (black_pieces.count(pos))
//                {
//                    sf::Sprite piece_sprite(black_piece_textures[black_pieces[pos]]);
//                    piece_sprite.setPosition(x * 100, y * 100);
//                    window.draw(piece_sprite);
//                }
//            }
//        }
//
//        // Rysowanie współrzędnych
//        for (int i = 0; i < 8; ++i)
//        {
//            sf::Text text;
//            text.setFont(font);
//            text.setFillColor(sf::Color::Black);
//            text.setCharacterSize(24);
//
//            // Rysowanie liter (kolumny)
//            text.setString(string(1, 'a' + i));
//            text.setPosition(i * 100 + 50, 800);
//            window.draw(text);
//
//            // Rysowanie cyfr (rzędy)
//            text.setString(to_string(8 - i));
//            text.setPosition(800, i * 100 + 50);
//            window.draw(text);
//        }
//    }
//
//    // Funkcja wyświetlająca pomoc
//    void printHelp()
//    {
//        cout << endl
//             << "* h: help, q: quit, p: show board, c: toggle show coordinates inside squares" << endl
//             << "* Input format: e2e4 moves piece from e2 to e4" << endl
//             << endl;
//    }
//
//    // Funkcja obsługująca wejście użytkownika
//    bool promptInput()
//    {
//        string move;
//        illegalmove:
//        if (turn == Turn::white)
//            cout << "White move: ";
//        else
//            cout << "Black move: ";
//        if (move == "")
//            cin >> move;
//        if (move == "q")
//        {
//            cout << "Good bye" << endl
//                 << endl;
//            return false;
//        }
//        if (move == "?" || move == "h" || move == "help")
//        {
//            printHelp();
//            move = "";
//            goto illegalmove;
//        }
//        if (move == "c")
//        {
//            show_coordinates = !show_coordinates;
//            move = "";
//            goto illegalmove;
//        }
//
//        if (move == "p")
//        {
//            move = "";
//            goto illegalmove;
//        }
//        Pos from(-1, -1), to(-1, -1);
//        if (move.length() == 4)
//        {
//            from.x = move[0] - 'a' + 1;
//            from.y = move[1] - '0';
//            to.x = move[2] - 'a' + 1;
//            to.y = move[3] - '0';
//        }
//        if (!makeMove(from, to))
//        {
//            cout << "* Illegal move" << endl;
//            move = "";
//            goto illegalmove;
//        }
//        return true;
//    }
//
//    // Funkcja obliczająca wynik gry
//    int score()
//    {
//        int sumWhite = 0;
//        for (auto &p : white_pieces)
//            sumWhite += pieceValues[p.second];
//        int sumBlack = 0;
//        for (auto &p : black_pieces)
//            sumBlack += pieceValues[p.second];
//        return sumWhite - sumBlack;
//    }
//
//    // Funkcja sprawdzająca czy na planszy jest król
//    bool hasKing()
//    {
//        for (auto &p : moverPieces())
//            if (p.second == Piece::king)
//                return true;
//        return false;
//    }
//
//    struct Move
//    {
//        Pos from, to;
//        int score;
//    };
//
//    // Funkcja implementująca algorytm minimax
//    Move minimax(int depth, bool minimize)
//    {
//        Move best_move;
//        best_move.score = -1000000 + 2000000 * minimize;
//        if (0 == depth)
//        {
//            best_move.score = score();
//            return best_move;
//        }
//
//        for (auto &from : moverPieces())
//        {
//            for (auto &to : possibleMoves(from.first))
//            {
//                ChessBoard branch = *this;
//                branch.makeMove(from.first, to);
//                Move option = branch.minimax(depth - 1, !minimize);
//                if ((option.score > best_move.score && !minimize) || (option.score < best_move.score && minimize))
//                {
//                    best_move.score = option.score;
//                    best_move.from = from.first;
//                    best_move.to = to;
//                }
//            }
//        }
//        return best_move;
//    }
//
//    // Funkcja wykonująca ruch AI
//    void AIMove()
//    {
//        bool minimize = turn == Turn::black ? true : false;
//        Move m = minimax(4, minimize);
//        makeMove(m.from, m.to);
//    }
//};
//
//// Inicjalizacja wartości bierek
//map<ChessBoard::Piece, int> ChessBoard::pieceValues{
//        {ChessBoard::Piece::king, 10000},
//        {ChessBoard::Piece::queen, 9},
//        {ChessBoard::Piece::black_pawn, 1},
//        {ChessBoard::Piece::white_pawn, 1},
//        {ChessBoard::Piece::bishop, 3},
//        {ChessBoard::Piece::knight, 3},
//        {ChessBoard::Piece::rook, 5},
//};
//
//int main()
//{
//    ChessBoard game;
//    cout << endl
//         << "* Chesscomputer v.0.1" << endl;
//    game.printHelp();
//
//    bool gameon = true;
//    game.reset();
//    string pick_side = "";
//    while (pick_side != "b" && pick_side != "w" && pick_side != "q")
//    {
//        cout << endl
//             << "Play as (b)lack or (w)hite or (q)uit? ";
//        cin >> pick_side;
//    }
//    if (pick_side == "q")
//    {
//        cout << "Good bye." << endl;
//        return 0;
//    }
//
//    if (pick_side == "b")
//        game.AIMove();
//    sf::RenderWindow window(sf::VideoMode(900, 900), "Chess");
//
//    sf::Texture light_square_texture;
//    sf::Texture dark_square_texture;
//    map<ChessBoard::Piece, sf::Texture> white_piece_textures;
//    map<ChessBoard::Piece, sf::Texture> black_piece_textures;
//    sf::Font font;
//
//    // Wczytywanie tekstur i czcionek
//    if (!light_square_texture.loadFromFile("images/square_light.png") ||
//        !dark_square_texture.loadFromFile("images/square_dark.png") ||
//        !white_piece_textures[ChessBoard::Piece::white_pawn].loadFromFile("images/wP.png") ||
//        !black_piece_textures[ChessBoard::Piece::black_pawn].loadFromFile("images/bP.png") ||
//        !white_piece_textures[ChessBoard::Piece::rook].loadFromFile("images/wR.png") ||
//        !black_piece_textures[ChessBoard::Piece::rook].loadFromFile("images/bR.png") ||
//        !white_piece_textures[ChessBoard::Piece::knight].loadFromFile("images/wN.png") ||
//        !black_piece_textures[ChessBoard::Piece::knight].loadFromFile("images/bN.png") ||
//        !white_piece_textures[ChessBoard::Piece::bishop].loadFromFile("images/wB.png") ||
//        !black_piece_textures[ChessBoard::Piece::bishop].loadFromFile("images/bB.png") ||
//        !white_piece_textures[ChessBoard::Piece::king].loadFromFile("images/wK.png") ||
//        !black_piece_textures[ChessBoard::Piece::king].loadFromFile("images/bK.png") ||
//        !white_piece_textures[ChessBoard::Piece::queen].loadFromFile("images/wQ.png") ||
//        !black_piece_textures[ChessBoard::Piece::queen].loadFromFile("images/bQ.png") ||
//        !font.loadFromFile("images/arial.ttf"))
//    {
//        cout << "Error loading textures or font!" << endl;
//        return 1;
//    }
//
//    // Pętla zdarzeń
//    while (window.isOpen())
//    {
//        sf::Event event;
//        while (window.pollEvent(event))
//        {
//            if (event.type == sf::Event::Closed)
//                window.close();
//        }
//
//        window.clear();
//        game.drawBoard(window, light_square_texture, dark_square_texture, white_piece_textures, black_piece_textures, font);
//        window.display();
//
//        if (gameon)
//        {
//            if (pick_side == "b")
//            {
//                if (game.turn == ChessBoard::Turn::white)
//                {
//                    // Tura użytkownika
//                    if (!game.promptInput())
//                        gameon = false;
//                }
//                else
//                {
//                    // Tura AI
//                    game.AIMove();
//                }
//            }
//            else
//            {
//                if (game.turn == ChessBoard::Turn::black)
//                {
//                    // Tura AI
//                    game.AIMove();
//                }
//                else
//                {
//                    // Tura użytkownika
//                    if (!game.promptInput())
//                        gameon = false;
//                }
//            }
//
//            if (!game.hasKing())
//            {
//                if (game.turn == ChessBoard::Turn::black)
//                    cout << "* You are victorious!" << endl;
//                else
//                    cout << "* You are defeated!" << endl;
//                gameon = false;
//            }
//        }
//    }
//
//    return 0;
//}
