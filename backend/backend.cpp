#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <cctype>
#include <fstream>
#include <windows.h>
#include <filesystem>
using namespace std;

// Forward declarations
class Board;
class Game;

// Helper functions
void renderWindow();
void closeWindow();

// ===== Piece Class =====
class Piece {
protected:
    string color;
    bool hasMoved;
public:
    Piece(string color) : color(color), hasMoved(false) {}
    virtual ~Piece() = default;
    virtual vector<pair<int, int>> getValidMoves(Board& board, int row, int col) = 0;
    virtual string toString() const = 0;

    string getColor() const { return color; }
    bool getMoved() const { return hasMoved; }
    void setMoved() { hasMoved = true; }
};

// ===== Derived Piece Classes =====
class Pawn : public Piece {
public:
    Pawn(string color) : Piece(color) {}
    vector<pair<int, int>> getValidMoves(Board& board, int row, int col) override;
    string toString() const override { return color[0] + string("P"); }
};

class Rook : public Piece {
public:
    Rook(string color) : Piece(color) {}
    vector<pair<int, int>> getValidMoves(Board& board, int row, int col) override;
    string toString() const override { return color[0] + string("R"); }
};

class Knight : public Piece {
public:
    Knight(string color) : Piece(color) {}
    vector<pair<int, int>> getValidMoves(Board& board, int row, int col) override;
    string toString() const override { return color[0] + string("N"); }
};

class Bishop : public Piece {
public:
    Bishop(string color) : Piece(color) {}
    vector<pair<int, int>> getValidMoves(Board& board, int row, int col) override;
    string toString() const override { return color[0] + string("B"); }
};

class Queen : public Piece {
public:
    Queen(string color) : Piece(color) {}
    vector<pair<int, int>> getValidMoves(Board& board, int row, int col) override;
    string toString() const override { return color[0] + string("Q"); }
};

class King : public Piece {
public:
    King(string color) : Piece(color) {}
    vector<pair<int, int>> getValidMoves(Board& board, int row, int col) override;
    string toString() const override { return color[0] + string("K"); }
};

// ===== Board Class =====
class Board {
private:
    shared_ptr<Piece> board[8][8];
public:
    Board() { setup(); }

    void setup() {
        // Clear the board
        for (int i = 0; i < 8; ++i) {
            for (int j = 0; j < 8; ++j) {
                board[i][j] = nullptr;
            }
        }

        // Set up pawns
        for (int i = 0; i < 8; ++i) {
            board[1][i] = make_shared<Pawn>("black");
            board[6][i] = make_shared<Pawn>("white");
        }

        // Set up rooks
        board[0][0] = make_shared<Rook>("black");
        board[0][7] = make_shared<Rook>("black");
        board[7][0] = make_shared<Rook>("white");
        board[7][7] = make_shared<Rook>("white");

        // Set up knights
        board[0][1] = make_shared<Knight>("black");
        board[0][6] = make_shared<Knight>("black");
        board[7][1] = make_shared<Knight>("white");
        board[7][6] = make_shared<Knight>("white");

        // Set up bishops
        board[0][2] = make_shared<Bishop>("black");
        board[0][5] = make_shared<Bishop>("black");
        board[7][2] = make_shared<Bishop>("white");
        board[7][5] = make_shared<Bishop>("white");

        // Set up queens
        board[0][3] = make_shared<Queen>("black");
        board[7][3] = make_shared<Queen>("white");

        // Set up kings
        board[0][4] = make_shared<King>("black");
        board[7][4] = make_shared<King>("white");
    }

    bool isValid(int row, int col) const {
        return row >= 0 && row < 8 && col >= 0 && col < 8;
    }

    bool isEmpty(int row, int col) const {
        return isValid(row, col) && !board[row][col];
    }

    bool isEnemy(int row, int col, string color) const {
        return isValid(row, col) && board[row][col] && board[row][col]->getColor() != color;
    }

    shared_ptr<Piece> getPiece(int row, int col) const {
        if (isValid(row, col)) return board[row][col];
        return nullptr;
    }

    bool movePiece(int sr, int sc, int er, int ec, string color) {
        auto piece = getPiece(sr, sc);
        if (!piece || piece->getColor() != color) return false;

        auto validMoves = piece->getValidMoves(*this, sr, sc);
        for (auto& move : validMoves) {
            if (move.first == er && move.second == ec) {
                // Handle pawn promotion
                if (piece->toString()[1] == 'P' && (er == 0 || er == 7)) {
                    board[er][ec] = make_shared<Queen>(color); // Auto-promote to queen
                } else {
                    board[er][ec] = board[sr][sc];
                }
                board[sr][sc].reset();
                board[er][ec]->setMoved();
                return true;
            }
        }
        return false;
    }

    void print() const {
        ofstream outfile("data.txt", ios::out | ios::trunc);
        for (int r = 0; r < 8; ++r) {
            for (int c = 0; c < 8; ++c) {
                if (board[r][c]) outfile << board[r][c]->toString() << " ";
                else outfile << ".. ";
            }
            outfile << endl;
        }
        outfile.close();
        closeWindow();
        renderWindow();
    }

    vector<pair<int, int>> getLinearMoves(int row, int col, string color, vector<pair<int, int>> directions) {
        vector<pair<int, int>> moves;
        for (auto dir : directions) {
            int dr = dir.first, dc = dir.second;
            int r = row + dr, c = col + dc;
            while (isValid(r, c)) {
                if (isEmpty(r, c)) {
                    moves.push_back({r, c});
                } else if (isEnemy(r, c, color)) {
                    moves.push_back({r, c});
                    break;
                } else break;
                r += dr;
                c += dc;
            }
        }
        return moves;
    }

    // Check if the king of given color is in check
    bool isInCheck(string color) {
        pair<int, int> kingPos = findKing(color);
        if (kingPos.first == -1) return false; // Should not happen

        // Check if any enemy piece can attack the king
        for (int r = 0; r < 8; ++r) {
            for (int c = 0; c < 8; ++c) {
                auto piece = getPiece(r, c);
                if (piece && piece->getColor() != color) {
                    auto moves = piece->getValidMoves(*this, r, c);
                    for (auto move : moves) {
                        if (move.first == kingPos.first && move.second == kingPos.second) {
                            return true;
                        }
                    }
                }
            }
        }
        return false;
    }

private:
    pair<int, int> findKing(string color) {
        for (int r = 0; r < 8; ++r) {
            for (int c = 0; c < 8; ++c) {
                auto piece = getPiece(r, c);
                if (piece && piece->toString()[1] == 'K' && piece->getColor() == color) {
                    return {r, c};
                }
            }
        }
        return {-1, -1}; // Should not happen
    }
};

// ===== Piece Move Implementations =====
vector<pair<int, int>> Pawn::getValidMoves(Board& board, int row, int col) {
    vector<pair<int, int>> moves;
    int dir = (color == "white") ? -1 : 1;
    int startRow = (color == "white") ? 6 : 1;

    // Forward move
    if (board.isEmpty(row + dir, col)) {
        moves.push_back({row + dir, col});
        if (row == startRow && board.isEmpty(row + 2 * dir, col)) {
            moves.push_back({row + 2 * dir, col});
        }
    }

    // Captures
    for (int dc = -1; dc <= 1; dc += 2) {
        if (board.isEnemy(row + dir, col + dc, color)) {
            moves.push_back({row + dir, col + dc});
        }
    }

    return moves;
}

vector<pair<int, int>> Rook::getValidMoves(Board& board, int row, int col) {
    return board.getLinearMoves(row, col, color, {{1, 0}, {-1, 0}, {0, 1}, {0, -1}});
}

vector<pair<int, int>> Knight::getValidMoves(Board& board, int row, int col) {
    vector<pair<int, int>> moves;
    int drc[8][2] = {{-2, -1}, {-2, 1}, {-1, -2}, {-1, 2}, {1, -2}, {1, 2}, {2, -1}, {2, 1}};
    for (int i = 0; i < 8; ++i) {
        int r = row + drc[i][0], c = col + drc[i][1];
        if (board.isValid(r, c) && (board.isEmpty(r, c) || board.isEnemy(r, c, color))) {
            moves.push_back({r, c});
        }
    }
    return moves;
}

vector<pair<int, int>> Bishop::getValidMoves(Board& board, int row, int col) {
    return board.getLinearMoves(row, col, color, {{1, 1}, {1, -1}, {-1, 1}, {-1, -1}});
}

vector<pair<int, int>> Queen::getValidMoves(Board& board, int row, int col) {
    return board.getLinearMoves(row, col, color, {{1, 0}, {-1, 0}, {0, 1}, {0, -1}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}});
}

vector<pair<int, int>> King::getValidMoves(Board& board, int row, int col) {
    vector<pair<int, int>> moves;
    int drc[8][2] = {{-1, 0}, {1, 0}, {0, 1}, {0, -1}, {-1, -1}, {-1, 1}, {1, -1}, {1, 1}};
    for (int i = 0; i < 8; ++i) {
        int r = row + drc[i][0], c = col + drc[i][1];
        if (board.isValid(r, c) && (board.isEmpty(r, c) || board.isEnemy(r, c, color))) {
            moves.push_back({r, c});
        }
    }
    return moves;
}

// ===== Game Class =====
class Game {
private:
    Board board;
    string currentTurn;

    pair<int, int> parsePosition(string pos) {
        if (pos.length() != 2) return {-1, -1};
        char file = tolower(pos[0]);
        char rank = pos[1];
        if (file < 'a' || file > 'h' || rank < '1' || rank > '8') return {-1, -1};
        int col = file - 'a';
        int row = 8 - (rank - '0');
        return {row, col};
    }

public:
    Game() : currentTurn("white") {}

    void switchTurn() {
        currentTurn = (currentTurn == "white") ? "black" : "white";
    }

    void play() {
        while (true) {
            board.print();
            cout << currentTurn << "'s move (e.g., e2 e4 or 'exit'): ";
            string startPos, endPos;
            cin >> startPos;
            if (startPos == "exit") break;
            cin >> endPos;

            auto [sr, sc] = parsePosition(startPos);
            auto [er, ec] = parsePosition(endPos);

            if (sr == -1 || sc == -1 || er == -1 || ec == -1) {
                cout << "Invalid input! Use format like 'e2 e4'.\n";
                continue;
            }

            if (board.movePiece(sr, sc, er, ec, currentTurn)) {
                cout << "Move successful!\n";
                switchTurn();
            } else {
                cout << "Invalid move. Try again.\n";
            }
        }
    }
};

// ===== Helper Functions =====
void renderWindow() {
    ofstream render("render");
    render.close();
}

void closeWindow() {
    filesystem::remove("render");
}

// ===== Main Function =====
int main() {
    Game game;
    game.play();
    cout << "Game ended. Thanks for playing!\n";
    closeWindow();
    return 0;
}