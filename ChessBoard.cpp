#include "ChessBoard.h"

std::map<ChessBoard::Piece, int> ChessBoard::pieceValues{
        {ChessBoard::Piece::king, 10000},
        {ChessBoard::Piece::queen, 9},
        {ChessBoard::Piece::black_pawn, 1},
        {ChessBoard::Piece::white_pawn, 1},
        {ChessBoard::Piece::bishop, 3},
        {ChessBoard::Piece::knight, 3},
        {ChessBoard::Piece::rook, 5},
};

void ChessBoard::reset()
{
    turn = Turn::white;
    white_pieces.clear();
    black_pieces.clear();
    for (int i = 1; i < 9; ++i)
    {
        white_pieces[ChessBoard::Pos(i, 2)] = Piece::white_pawn;
        black_pieces[ChessBoard::Pos(i, 7)] = Piece::black_pawn;
    }
    int n = 1;
    for (auto piece : {Piece::rook, Piece::knight, Piece::bishop, Piece::king})
    {
        white_pieces[ChessBoard::Pos(n, 1)] = white_pieces[ChessBoard::Pos(9 - n, 1)] = piece;
        black_pieces[ChessBoard::Pos(n, 8)] = black_pieces[ChessBoard::Pos(9 - n, 8)] = piece;
        ++n;
    }
    white_pieces[ChessBoard::Pos(4, 1)] = Piece::queen;
    black_pieces[ChessBoard::Pos(4, 8)] = Piece::queen;
}

void ChessBoard::flipTurn() { turn = turn == Turn::white ? Turn::black : Turn::white; }

bool ChessBoard::isInCheck(Turn t)
{
    ChessBoard::Pos kingPos;
    std::map<ChessBoard::Pos, Piece> &pieces = t == Turn::white ? white_pieces : black_pieces;
    for (const auto &piece : pieces)
    {
        if (piece.second == Piece::king)
        {
            kingPos = piece.first;
            break;
        }
    }
    std::map<ChessBoard::Pos, Piece> &opponent = t == Turn::white ? black_pieces : white_pieces;
    for (const auto &piece : opponent)
    {
        std::vector<ChessBoard::Pos> moves = possibleMoves(piece.first);
        if (find(moves.begin(), moves.end(), kingPos) != moves.end())
        {
            return true;
        }
    }
    return false;
}

bool ChessBoard::makeMove(ChessBoard::Pos from, ChessBoard::Pos to)
{
    if (!isMoveValid(from, to))
        return false;
    opponentPieces().erase(to);
    moverPieces()[to] = moverPieces()[from];
    moverPieces().erase(from);
    if ((moverPieces()[to] == Piece::white_pawn || moverPieces()[to] == Piece::black_pawn) && (to.y == 1 || to.y == 8))
        moverPieces()[to] = Piece::queen;
    flipTurn();
    return true;
}

bool ChessBoard::isMoveValid(ChessBoard::Pos from, ChessBoard::Pos to)
{
    std::vector<ChessBoard::Pos> allowed = possibleMoves(from);
    if (find(allowed.begin(), allowed.end(), to) == allowed.end())
        return false;

    ChessBoard tempBoard = *this;
    tempBoard.opponentPieces().erase(to);
    tempBoard.moverPieces()[to] = tempBoard.moverPieces()[from];
    tempBoard.moverPieces().erase(from);
    tempBoard.flipTurn();

    return !tempBoard.isInCheck(turn);
}

vector<ChessBoard::Pos> ChessBoard::possibleMoves(const ChessBoard::Pos &from)
{
    vector<ChessBoard::Pos> moves;
    auto isOwn = [&](int dx, int dy) -> bool
    { return moverPieces().count(ChessBoard::Pos(from, dx, dy)); };
    auto isOpponent = [&](int dx, int dy) -> bool
    { return opponentPieces().count(ChessBoard::Pos(from, dx, dy)); };
    auto isInsideBoard = [&](int dx, int dy) -> bool
    { ChessBoard::Pos p(from, dx, dy); return p.x < 9 && p.x > 0 && p.y < 9 && p.y > 0; };
    auto isFree = [&](int dx, int dy) -> bool
    { return !isOwn(dx, dy) && isInsideBoard(dx, dy) && !isOpponent(dx, dy); };

    auto addMove = [&](int dx, int dy) -> bool
    {
        if (isFree(dx, dy) || isOpponent(dx, dy))
        {
            moves.push_back(ChessBoard::Pos(from, dx, dy));
            return true;
        }
        return false;
    };

    if (!isOwn(0, 0))
        return moves;

    auto moving_piece = moverPieces()[from];
    switch (moving_piece)
    {
        case Piece::white_pawn:
            if (isFree(0, 1))
                addMove(0, 1);
            if (isFree(0, 1) && isFree(0, 2) && from.y == 2)
                addMove(0, 2);
            if (isOpponent(-1, 1))
                addMove(-1, 1);
            if (isOpponent(1, 1))
                addMove(1, 1);
            break;

        case Piece::black_pawn:
            if (isFree(0, -1))
                addMove(0, -1);
            if (isFree(0, -1) && isFree(0, -2) && from.y == 7)
                addMove(0, -2);
            if (isOpponent(-1, -1))
                addMove(-1, -1);
            if (isOpponent(1, -1))
                addMove(1, -1);
            break;

        case Piece::knight:
            addMove(-2, -1);
            addMove(-2, 1);
            addMove(2, -1);
            addMove(2, 1);
            addMove(-1, -2);
            addMove(-1, 2);
            addMove(1, -2);
            addMove(1, 2);
            break;

        case Piece::king:
            for (auto dy : {-1, 0, 1})
                for (auto dx : {-1, 0, 1})
                    addMove(dx, dy);
            break;

        case Piece::queen:
        case Piece::rook:
            for (int n = 1; n < 9 && addMove(0, n) && !isOpponent(0, n); ++n)
                ;
            for (int n = 1; n < 9 && addMove(0, -n) && !isOpponent(0, -n); ++n)
                ;
            for (int n = 1; n < 9 && addMove(n, 0) && !isOpponent(n, 0); ++n)
                ;
            for (int n = 1; n < 9 && addMove(-n, 0) && !isOpponent(-n, 0); ++n)
                ;
            if (moving_piece != Piece::queen)
                break;

        case Piece::bishop:
            for (int n = 1; n < 9 && addMove(n, n) && !isOpponent(n, n); ++n)
                ;
            for (int n = 1; n < 9 && addMove(n, -n) && !isOpponent(n, -n); ++n)
                ;
            for (int n = 1; n < 9 && addMove(-n, n) && !isOpponent(-n, n); ++n)
                ;
            for (int n = 1; n < 9 && addMove(-n, -n) && !isOpponent(-n, -n); ++n)
                ;
            break;
    }

    return moves;
}

void ChessBoard::drawBoard(sf::RenderWindow &window, sf::Texture &light_square_texture, sf::Texture &dark_square_texture, map<Piece, sf::Texture> &white_piece_textures, map<Piece, sf::Texture> &black_piece_textures, sf::Font &font)
{
    sf::Sprite light_square(light_square_texture);
    sf::Sprite dark_square(dark_square_texture);

    for (int y = 0; y < 8; y++)
    {
        for (int x = 0; x < 8; x++)
        {
            if ((x + y) % 2 == 0)
            {
                light_square.setPosition(x * 100, y * 100);
                window.draw(light_square);
            }
            else
            {
                dark_square.setPosition(x * 100, y * 100);
                window.draw(dark_square);
            }

            ChessBoard::Pos pos(x + 1, 8 - y);
            if (white_pieces.count(pos))
            {
                sf::Sprite piece_sprite(white_piece_textures[white_pieces[pos]]);
                piece_sprite.setPosition(x * 100, y * 100);
                window.draw(piece_sprite);
            }
            else if (black_pieces.count(pos))
            {
                sf::Sprite piece_sprite(black_piece_textures[black_pieces[pos]]);
                piece_sprite.setPosition(x * 100, y * 100);
                window.draw(piece_sprite);
            }
        }
    }

    // Draw coordinates
    for (int i = 0; i < 8; ++i)
    {
        sf::Text text;
        text.setFont(font);
        text.setFillColor(sf::Color::Black);
        text.setCharacterSize(24);

        // Draw letters
        text.setString(string(1, 'a' + i));
        text.setPosition(i * 100 + 50, 800);
        window.draw(text);

        // Draw numbers
        text.setString(to_string(8 - i));
        text.setPosition(800, i * 100 + 50);
        window.draw(text);
    }
}

void ChessBoard::printHelp()
{
    cout << endl
         << "* h: help, q: quit, p: show board, c: toggle show coordinates inside squares" << endl
         << "* Input format: e2e4 moves piece from e2 to e4" << endl
         << endl;
}

bool ChessBoard::promptInput()
{
    string move;
    illegalmove:
    if (turn == Turn::white)
        cout << "White move: ";
    else
        cout << "Black move: ";
    if (move == "")
        cin >> move;
    if (move == "q")
    {
        cout << "Good bye" << endl
             << endl;
        return false;
    }
    if (move == "?" || move == "h" || move == "help")
    {
        printHelp();
        move = "";
        goto illegalmove;
    }
    if (move == "c")
    {
        show_coordinates = !show_coordinates;
        move = "";
        goto illegalmove;
    }

    if (move == "p")
    {
        move = "";
        goto illegalmove;
    }
    ChessBoard::Pos from(-1, -1), to(-1, -1);
    if (move.length() == 4)
    {
        from.x = move[0] - 'a' + 1;
        from.y = move[1] - '0';
        to.x = move[2] - 'a' + 1;
        to.y = move[3] - '0';
    }
    if (!makeMove(from, to))
    {
        cout << "* Illegal move" << endl;
        move = "";
        goto illegalmove;
    }
    return true;
}

int ChessBoard::score()
{
    int sumWhite = 0;
    for (auto &p : white_pieces)
        sumWhite += pieceValues[p.second];
    int sumBlack = 0;
    for (auto &p : black_pieces)
        sumBlack += pieceValues[p.second];
    return sumWhite - sumBlack;
}

bool ChessBoard::hasKing()
{
    for (auto &p : moverPieces())
        if (p.second == Piece::king)
            return true;
    return false;
}

ChessBoard::Move ChessBoard::minimax(int depth, bool minimize)
{
    Move best_move;
    best_move.score = -1000000 + 2000000 * minimize;
    if (0 == depth)
    {
        best_move.score = score();
        return best_move;
    }

    for (auto &from : moverPieces())
    {
        for (auto &to : possibleMoves(from.first))
        {
            ChessBoard branch = *this;
            branch.makeMove(from.first, to);
            Move option = branch.minimax(depth - 1, !minimize);
            if ((option.score > best_move.score && !minimize) || (option.score < best_move.score && minimize))
            {
                best_move.score = option.score;
                best_move.from = from.first;
                best_move.to = to;
            }
        }
    }
    return best_move;
}

void ChessBoard::AIMove()
{
    bool minimize = turn == Turn::black ? true : false;
    Move m = minimax(1, minimize);
    makeMove(m.from, m.to);
}
