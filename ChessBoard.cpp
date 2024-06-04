#include "ChessBoard.h" // Dołączenie nagłówka z logiką szachownicy
#include <iostream> // Dołączenie nagłówka do operacji wejścia/wyjścia
#include <vector> // Dołączenie nagłówka dla wektorów
#include <map> // Dołączenie nagłówka dla map

// Ustawienie wartości punktowych dla każdego typu figury
std::map<ChessBoard::Piece, int> ChessBoard::pieceValues{
        {ChessBoard::Piece::king, 10000},
        {ChessBoard::Piece::queen, 9},
        {ChessBoard::Piece::rook, 5},
        {ChessBoard::Piece::bishop, 3},
        {ChessBoard::Piece::knight, 3},
        {ChessBoard::Piece::white_pawn, 1},
        {ChessBoard::Piece::black_pawn, 1},
};

bool gameOver = false; // Zmienna globalna określająca, czy gra się zakończyła

void ChessBoard::reset() {
    turn = Turn::white; // Ustawienie tury na białe figury
    gameOver = false; // Zresetowanie zmiennej gameOver
    white_pieces.clear(); // Wyczyść białe figury
    black_pieces.clear(); // Wyczyść czarne figury
    move_history.clear(); // Wyczyść historię ruchów

    // Umieszczanie pionków
    for (int i = 1; i < 9; ++i) {
        white_pieces[Pos(i, 2)] = Piece::white_pawn;
        black_pieces[Pos(i, 7)] = Piece::black_pawn;
    }

    // Umieszczanie wież
    white_pieces[Pos(1, 1)] = Piece::rook;
    white_pieces[Pos(8, 1)] = Piece::rook;
    black_pieces[Pos(1, 8)] = Piece::rook;
    black_pieces[Pos(8, 8)] = Piece::rook;

    // Umieszczanie koni
    white_pieces[Pos(2, 1)] = Piece::knight;
    white_pieces[Pos(7, 1)] = Piece::knight;
    black_pieces[Pos(2, 8)] = Piece::knight;
    black_pieces[Pos(7, 8)] = Piece::knight;

    // Umieszczanie gońców
    white_pieces[Pos(3, 1)] = Piece::bishop;
    white_pieces[Pos(6, 1)] = Piece::bishop;
    black_pieces[Pos(3, 8)] = Piece::bishop;
    black_pieces[Pos(6, 8)] = Piece::bishop;

    // Umieszczanie królowych
    white_pieces[Pos(4, 1)] = Piece::queen;
    black_pieces[Pos(4, 8)] = Piece::queen;

    // Umieszczanie królów
    white_pieces[Pos(5, 1)] = Piece::king;
    black_pieces[Pos(5, 8)] = Piece::king;
}

void ChessBoard::flipTurn() {
    turn = (turn == Turn::white) ? Turn::black : Turn::white; // Zmiana tury na przeciwną
}

bool ChessBoard::isCheck(Turn t) {
    Pos kingPos; // Pozycja króla
    bool kingFound = false; // Flaga, czy król został znaleziony
    std::map<Pos, Piece>& pieces = (t == Turn::white) ? white_pieces : black_pieces; // Wybór odpowiednich figur
    for (const auto& piece : pieces) {
        if (piece.second == Piece::king) { // Szukanie króla
            kingPos = piece.first;
            kingFound = true;
            break;
        }
    }
    if (!kingFound) return false; // Jeśli król nie został znaleziony, nie ma szacha

    std::map<Pos, Piece>& opponent = (t == Turn::white) ? black_pieces : white_pieces; // Wybór figur przeciwnika
    for (const auto& piece : opponent) {
        std::vector<Pos> moves = possibleMoves(piece.first); // Pobranie możliwych ruchów przeciwnika

        // Sprawdzenie, czy którykolwiek z ruchów może zaatakować pozycję króla
        if (std::find(moves.begin(), moves.end(), kingPos) != moves.end()) {
            return true; // Jeśli tak, jest szach
        }
    }
    return false; // W przeciwnym razie nie ma szacha
}

bool ChessBoard::isCheckmate() {
    if (!isCheck(turn)) return false; // Jeśli król nie jest w szachu, to nie jest mat

    // Sprawdzenie, czy gracz ma jakiekolwiek legalne ruchy
    for (const auto& piece : moverPieces()) {
        std::vector<Pos> moves = possibleMoves(piece.first);
        for (const auto& move : moves) {
            ChessBoard tempBoard = *this;
            if (tempBoard.makeMove(piece.first, move)) {
                return false; // Jeśli istnieje legalny ruch, to nie jest mat
            }
        }
    }

    return true; // Jeśli król jest w szachu i nie ma legalnych ruchów, to jest mat
}

bool ChessBoard::makeMove(Pos from, Pos to) {
    if (!isMoveValid(from, to)) return false; // Sprawdzenie, czy ruch jest legalny

    opponentPieces().erase(to); // Usunięcie figury przeciwnika z docelowej pozycji
    moverPieces()[to] = moverPieces()[from]; // Przeniesienie figury na docelową pozycję
    moverPieces().erase(from); // Usunięcie figury z początkowej pozycji

    // Dodanie ruchu do historii
    std::ostringstream move_notation;
    move_notation << static_cast<char>('a' + from.x - 1) << from.y;
    if (opponentPieces().count(to) > 0) {
        move_notation << 'x';
    }
    move_notation << static_cast<char>('a' + to.x - 1) << to.y;
    move_history.push_back(move_notation.str());

    // Promocja pionka
    if ((moverPieces()[to] == Piece::white_pawn && to.y == 8) ||
        (moverPieces()[to] == Piece::black_pawn && to.y == 1)) {
        moverPieces()[to] = Piece::queen;
    }

    flipTurn(); // Zmiana tury

    return true;
}

bool ChessBoard::isMoveValid(Pos from, Pos to) {
    std::vector<Pos> allowed = possibleMoves(from); // Pobranie możliwych ruchów dla danej pozycji
    if (std::find(allowed.begin(), allowed.end(), to) == allowed.end()) return false; // Sprawdzenie, czy docelowa pozycja jest dozwolona

    ChessBoard tempBoard = *this;
    tempBoard.opponentPieces().erase(to);
    tempBoard.moverPieces()[to] = tempBoard.moverPieces()[from];
    tempBoard.moverPieces().erase(from);
    tempBoard.flipTurn();
    return !tempBoard.isCheck(turn); // Sprawdzenie, czy ruch nie zostawia króla w szachu
}

std::vector<ChessBoard::Pos> ChessBoard::possibleMoves(const Pos& from) {
    std::vector<Pos> moves;
    auto isOwn = [&](int dx, int dy) -> bool {
        return moverPieces().count(Pos(from.x + dx, from.y + dy)) > 0; // Sprawdzenie, czy na danej pozycji jest własna figura
    };
    auto isOpponent = [&](int dx, int dy) -> bool {
        return opponentPieces().count(Pos(from.x + dx, from.y + dy)) > 0; // Sprawdzenie, czy na danej pozycji jest figura przeciwnika
    };
    auto isInsideBoard = [&](int dx, int dy) -> bool {
        Pos p(from.x + dx, from.y + dy);
        return p.x <= 8 && p.x >= 1 && p.y <= 8 && p.y >= 1; // Sprawdzenie, czy pozycja znajduje się w granicach szachownicy
    };
    auto isFree = [&](int dx, int dy) -> bool {
        return !isOwn(dx, dy) && isInsideBoard(dx, dy) && !isOpponent(dx, dy); // Sprawdzenie, czy pozycja jest wolna
    };

    auto addMove = [&](int dx, int dy) -> bool {
        if (isFree(dx, dy) || isOpponent(dx, dy)) {
            moves.push_back(Pos(from.x + dx, from.y + dy)); // Dodanie pozycji do możliwych ruchów
            return !isOpponent(dx, dy); // Kontynuuj w tym samym kierunku, chyba że trafimy na figurę przeciwnika
        }
        return false;
    };

    if (!isOwn(0, 0)) return moves; // Jeśli na danej pozycji nie ma własnej figury, zwróć pustą listę ruchów

    auto moving_piece = moverPieces()[from];
    switch (moving_piece) {
        case Piece::white_pawn:
            if (isFree(0, 1)) addMove(0, 1);
            if (isFree(0, 1) && isFree(0, 2) && from.y == 2) addMove(0, 2);
            if (isOpponent(-1, 1)) addMove(-1, 1);
            if (isOpponent(1, 1)) addMove(1, 1);
            break;
        case Piece::black_pawn:
            if (isFree(0, -1)) addMove(0, -1);
            if (isFree(0, -1) && isFree(0, -2) && from.y == 7) addMove(0, -2);
            if (isOpponent(-1, -1)) addMove(-1, -1);
            if (isOpponent(1, -1)) addMove(1, -1);
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
            for (int dx = -1; dx <= 1; ++dx)
                for (int dy = -1; dy <= 1; ++dy)
                    if (dx != 0 || dy != 0) addMove(dx, dy);
            break;
        case Piece::queen:
        case Piece::rook:
            for (int n = 1; n < 8 && addMove(0, n); ++n);
            for (int n = 1; n < 8 && addMove(0, -n); ++n);
            for (int n = 1; n < 8 && addMove(n, 0); ++n);
            for (int n = 1; n < 8 && addMove(-n, 0); ++n);
            if (moving_piece != Piece::queen) break;
        case Piece::bishop:
            for (int n = 1; n < 8 && addMove(n, n); ++n);
            for (int n = 1; n < 8 && addMove(n, -n); ++n);
            for (int n = 1; n < 8 && addMove(-n, n); ++n);
            for (int n = 1; n < 8 && addMove(-n, -n); ++n);
            break;
    }

    return moves;
}

void ChessBoard::drawBoard(sf::RenderWindow& window, sf::Texture& light_square_texture, sf::Texture& dark_square_texture, std::map<Piece, sf::Texture>& white_piece_textures, std::map<Piece, sf::Texture>& black_piece_textures, sf::Font& font) {
    sf::Sprite light_square(light_square_texture);
    sf::Sprite dark_square(dark_square_texture);

    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            if ((x + y) % 2 == 0) {
                light_square.setPosition(x * 100, y * 100);
                window.draw(light_square);
            } else {
                dark_square.setPosition(x * 100, y * 100);
                window.draw(dark_square);
            }

            Pos pos(x + 1, 8 - y);
            if (white_pieces.count(pos)) {
                sf::Sprite piece_sprite(white_piece_textures[white_pieces[pos]]);
                piece_sprite.setPosition(x * 100, y * 100);
                window.draw(piece_sprite);
            } else if (black_pieces.count(pos)) {
                sf::Sprite piece_sprite(black_piece_textures[black_pieces[pos]]);
                piece_sprite.setPosition(x * 100, y * 100);
                window.draw(piece_sprite);
            }
        }
    }

    // Rysowanie współrzędnych
    for (int i = 0; i < 8; ++i) {
        sf::Text text;
        text.setFont(font);
        text.setFillColor(sf::Color::Black);
        text.setCharacterSize(24);

        // Rysowanie liter
        text.setString(std::string(1, 'a' + i));
        text.setPosition(i * 100 + 50, 800);
        window.draw(text);

        // Rysowanie cyfr
        text.setString(std::to_string(8 - i));
        text.setPosition(800, i * 100 + 50);
        window.draw(text);
    }
}

bool ChessBoard::parseMove(std::string move, Pos& from, Pos& to) {
    if (move.length() < 4) return false;

    int index = 0;
    Piece piece = Piece::white_pawn;

    if (isupper(move[0])) {
        switch (move[0]) {
            case 'K': piece = Piece::king; break;
            case 'Q': piece = Piece::queen; break;
            case 'R': piece = Piece::rook; break;
            case 'B': piece = Piece::bishop; break;
            case 'N': piece = Piece::knight; break;
            default: return false;
        }
        index++;
    }

    if (move[index] == 'x') index++; // Pomijanie 'x' jeśli jest obecne

    from.x = move[index] - 'a' + 1;
    from.y = move[index + 1] - '0';
    index += 2;

    to.x = move[index] - 'a' + 1;
    to.y = move[index + 1] - '0';

    return from.x >= 1 && from.x <= 8 && from.y >= 1 && from.y <= 8 &&
           to.x >= 1 && to.x <= 8 && to.y >= 1 && to.y <= 8;
}

bool ChessBoard::print() {
    std::string move;
    illegalmove:
    std::cout << "Enter move (e.g. e2e4): ";
    std::cin >> move;
    Pos from(-1, -1), to(-1, -1);
    if (!parseMove(move, from, to) || !makeMove(from, to)) {
        std::cout << "* Illegal move" << std::endl;
        move = "";
        goto illegalmove; // Powtarzanie pytania o ruch jeśli ruch jest nielegalny
    }
    return true;
}

void ChessBoard::printMoves() {
    for (size_t i = 0; i < move_history.size(); i += 2) {
        if (i + 1 < move_history.size()) {
            std::cout << (i / 2) + 1 << ". " << move_history[i] << " " << move_history[i + 1] << std::endl; // Wyświetlanie historii ruchów
        }
    }
}

int ChessBoard::score() {
    int sumWhite = 0;
    for (const auto& p : white_pieces) sumWhite += pieceValues[p.second]; // Suma punktów białych figur
    int sumBlack = 0;
    for (const auto& p : black_pieces) sumBlack += pieceValues[p.second]; // Suma punktów czarnych figur
    return sumWhite - sumBlack; // Zwracanie różnicy punktów
}

ChessBoard::Move ChessBoard::minimax(int depth, bool minimize) {
    Move best_move;
    best_move.score = minimize ? 1000000 : -1000000;

    if (depth == 0 || isCheckmate()) {
        best_move.score = score(); // Zwracanie wyniku jeśli osiągnięto maksymalną głębokość lub mat
        return best_move;
    }

    for (const auto& from : moverPieces()) {
        for (const auto& to : possibleMoves(from.first)) {
            ChessBoard branch = *this;
            if (branch.makeMove(from.first, to)) { // Unikanie rekurencyjnego wywoływania sprawdzenia mata
                Move option = branch.minimax(depth - 1, !minimize);
                if ((option.score > best_move.score && !minimize) || (option.score < best_move.score && minimize)) {
                    best_move.score = option.score;
                    best_move.from = from.first;
                    best_move.to = to;
                }
            }
        }
    }
    return best_move;
}

void ChessBoard::AIMove() {
    if (isCheckmate()) {
        gameOver = true; // Ustawienie gameOver jeśli mat
        return;
    }
    bool minimize = (turn == Turn::black);
    Move m = minimax(1, minimize); // Zwiększ głębokość dla lepszych decyzji AI
    makeMove(m.from, m.to);
}

void ChessBoard::playerMove() {
    if (turn == Turn::white) {
        if (!print()) {
            playerMove(); // Ponowne pytanie o ruch jeśli ruch jest nielegalny
        }
    } else {
        AIMove();
    }
    if (isCheckmate()) {
        gameOver = true; // Ustawienie gameOver jeśli mat
    }
}

bool ChessBoard::checkGameOver() {
    if (isCheckmate()) {
        gameOver = true; // Ustawienie gameOver jeśli mat
    }
    return gameOver;
}

std::string ChessBoard::getWinner() {
    return (turn == Turn::white) ? "Black wins!" : "White wins!"; // Zwracanie zwycięzcy
}
