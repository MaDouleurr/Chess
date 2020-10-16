#include <iostream>
#include <fstream>
#include <cctype>
#include "Chess.h"

Chess::Chess() {
    WhiteKingOrLeftRookMove = false;
    WhiteKingOrRightRookMove = false;
    BlackKingOrLeftRookMove = false;
    BlackKingOrRightRookMove = false;
    WhiteKingPosition.first = 0;
    WhiteKingPosition.second = 4;
    BlackKingPosition.first = 7;
    BlackKingPosition.second = 4;
    figures.resize(SizeFigures, std::vector<std::string>(SizeFigures, ""));
    board.resize(SizeBoard);
    const int SizeOfTemp = 100;
    char* TempString = new char[SizeOfTemp];
    std::ifstream fin("board.bin", std::ios::binary);
    if (!fin) {
        std::cout << "file board.bin not found" << std::endl;
        exit(0);
    }
    for (int i = 0; i < SizeBoard; i++)
        fin.getline(TempString, SizeOfTemp, '\n'), board[i] = TempString;
    delete[]TempString;
    fin.close();
    return;
}

Chess::~Chess() {}


void Chess::ShowGame()const {
    for (int i = 0; i < SizeBoard; i++) {
        if (i > 2 and i < 18 and i % 2) {
            for (unsigned int j = 0; j < board[i].size(); j++) {
                if (j > 1 and j < board[i].size() - 1)
                    std::cout << " " + figures[(i - 3) / 2][j - 2] + " ";
                std::cout << board[i][j];
            }
            std::cout << std::endl;
        }
        else
            std::cout << board[i] << std::endl;
    }
    return;
}





std::string Chess::CharToLowerString(const char* str)const {
    std::string LowerString = "";
    for (unsigned int i = 0; i < strlen(str); i++)
        LowerString += tolower(str[i]);
    return LowerString;
}


void Chess::menu() {
    const std::string new_game = "new game";
    const std::string load_game = "load game";
    const std::string help = "help";
    const std::string exit = "exit";
    std::string instructionString;
    char* instructionChar = new char[20];
    bool flag;
    while (true) {
        flag = false;
        std::cout << "Enter instruction (instruction number)" << std::endl;
        std::cout << "1 - New game" << std::endl;
        std::cout << "2 - Load game" << std::endl;
        std::cout << "3 - Help" << std::endl;
        std::cout << "4 - exit" << std::endl;
        std::cin.getline(instructionChar, 20);
        instructionString = CharToLowerString(instructionChar);
        if (instructionString == "1" or instructionString == new_game)
            NewGame(), flag = true;
        if (instructionString == "2" or instructionString == load_game) {
            LoadGame();
            flag = true;
        }
        if (instructionString == "3" or instructionString == help)
            Help(), flag = true;
        if (instructionString == "4" or instructionString == exit)
            return;
        if (!flag)
            std::cout << "Wrong instruction!" << std::endl;
    }
    delete[]instructionChar;
    return;
}

void Chess::NewGame() {
    std::ifstream fin("figures.bin", std::ios::binary);
    if (!fin) {
        std::cout << "file figures.bin not found" << std::endl;
        exit(0);
    }
    for (int i = 0; i < SizeFigures; i++)
        for (int j = 0; j < SizeFigures; j++)
            if (i < 2 or i > 5)
                fin >> figures[i][j];
            else
                figures[i][j] = "  ";
    fin.close();
    Game();
    return;
}

void Chess::LoadGame() {
    std::string TempString;
    std::ifstream fin("saves\\saves.txt", std::ios::binary);
    if (!fin) {
        std::cout << "File saves.txt not found. Perhaps you don`t have saved games" << std::endl;
        return;
    }
    std::cout << "Your games" << std::endl;
    while (fin >> TempString)
        std::cout << TempString << std::endl;
    fin.close();
    std::cout << "Choose the game" << std::endl;
    std::cin >> TempString;
    fin.open("saves\\" + TempString + ".txt", std::ios::binary);
    if (!fin) {
        std::cout << "file doesn`t exist" << std::endl;
        return;
    }
    for (int i = 0; i < SizeFigures; i++) {
        for (int j = 0; j < SizeFigures; j++) {
            fin >> TempString;
            if (TempString == "_")
                figures[i][j] = "  ";
            else
                figures[i][j] = TempString;
        }
    }
    int i;
    fin >> i;
    fin >> WhiteKingOrLeftRookMove;
    fin >> WhiteKingOrRightRookMove;
    fin >> BlackKingOrLeftRookMove;
    fin >> BlackKingOrRightRookMove;
    fin >> LastCell1.first >> LastCell1.second;
    fin >> LastCell2.first >> LastCell2.second;
    fin >> WhiteKingPosition.first >> WhiteKingPosition.second;
    fin >> BlackKingPosition.first >> BlackKingPosition.second;
    fin.close();
    Game(i);
    return;
}

void Chess::Help()const {
    char* StringTemp = new char[150];
    std::ifstream fin("help.txt");
    if (!fin) {
        std::cout << "file help.txt not found" <<std:: endl;
        return;
    }
    while (!fin.eof()) {
        fin.getline(StringTemp, 150);
        std::cout << std::endl << StringTemp;
    }
    fin.close();
    delete[]StringTemp;
    return;
}

void Chess::Game(int i) {
    std::string Stringinstruction;
    const std::string restart = "restart";
    const std::string help = "help";
    const std::string save = "save";
    const std::string exit = "exit";
    std::pair<int, int> cell1, cell2;
    bool flag;
    bool Moving;
    int Pmode;
    char NotMoverColor;
    for (;; i++) {
        ShowGame();
        Pmode = (i % 2) ? 1 : -1;
        NotMoverColor = (Pmode == 1) ? 'B' : 'W';
        if (not ExistenceMove(Pmode, NotMoverColor)) {
            if (Shah(Pmode)) {
                std::cout << "Checkmate"<<((Pmode == 1)?"White":"Black")<<" Player" << std::endl;
                break;
            }
            else {
                std::cout << "Stalemate" << std::endl;
                break;
            }
        }

        flag = false;
        Moving = false;
        if (i % 2)
            std::cout << "White Player" << std::endl;
        else
            std::cout << "Black Player" << std::endl;
        std::cout << "Enter  move or instruction(instruction number). Example of move \"A1-A2\"" << std::endl;
        std::cout << "2 - restart" << std::endl;
        std::cout << "3 - save" << std::endl;
        std::cout << "4 - help" << std::endl;
        std::cout << "5 - exit" << std::endl;
        std::cin >> Stringinstruction;
        Stringinstruction = CharToLowerString(Stringinstruction.c_str());
        if (Stringinstruction.length() == 5 and Stringinstruction[2] == '-') {
            if (!parsing(Stringinstruction, cell1, cell2)) {
                std::cout << "Wrong move" << std::endl;
                continue;
            }
            Moving = Move(Pmode, cell1, cell2);
            flag = true;
        }
        if (Stringinstruction == restart or Stringinstruction == "2")
            NewGame(), flag = true;
        if (Stringinstruction == save or Stringinstruction == "3")
            Save(i), flag = true;
        if (Stringinstruction == help or Stringinstruction == "4")
            Help(), flag = true;
        if (Stringinstruction == exit or Stringinstruction == "5") {
            break;
        }
        if (!flag)
            std::cout << "Wrong instruction" << std::endl;
        if (not Moving)
            i--;
    }
    std::cin.ignore();
    return;
}


bool Chess::parsing(const std::string& Move, std::pair<int, int>& cell1, std::pair<int, int>& cell2) const {
    if (Move[0] >= 'a' and Move[0] <= 'h')
        cell1.second = (int)Move[0] - 97;
    else
        return false;
    if (Move[3] >= 'a' and Move[3] <= 'h')
        cell2.second = (int)Move[3] - 97;
    else
        return false;
    if (Move[1] > '0' and Move[1] < '9')
        cell1.first = (int)Move[1] - 49;
    else
        return false;
    if (Move[4] > '0' and Move[4] < '9')
        cell2.first = (int)Move[4] - 49;
    else
        return false;
    return true;
}

void Chess::Save(const int i)const {
    std::string NameOfGame;
    std::cout << "Enter name of game" << std::endl;
    std::cin >> NameOfGame;
    std::ofstream fout("saves\\" + NameOfGame + ".txt");
    for (int i = 0; i < SizeFigures; i++) {
        for (int j = 0; j < SizeFigures; j++)
            if (figures[i][j] == "  ")
                fout << "_ ";
            else
                fout << figures[i][j] + " ";
        fout << std::endl;
    }
    fout << i << std::endl;
    fout << WhiteKingOrLeftRookMove;
    fout << WhiteKingOrRightRookMove;
    fout << BlackKingOrLeftRookMove;
    fout << BlackKingOrRightRookMove;
    fout << LastCell1.first << ' ' << LastCell1.second;
    fout << LastCell2.first << ' ' << LastCell2.second;
    fout << WhiteKingPosition.first << ' ' << WhiteKingPosition.second;
    fout << BlackKingPosition.first << ' ' << BlackKingPosition.second;
    fout.close();
    fout.open("saves\\saves.txt", std::ios::app);
    fout << NameOfGame << std::endl;
    fout.close();
}


bool Chess::Move(const int Pmode, const std::pair<int, int>& cell1, const std::pair<int, int>& cell2, bool message) {
    int Castling;
    std::string FigureCell2 = "  ";
    int move = CheckMove(cell1, cell2, Pmode, message);
    if (move == 0)
        return false;
    if (move == 1 or move == 2) {
        FigureCell2 = figures[cell2.first][cell2.second];
        figures[cell2.first][cell2.second] = figures[cell1.first][cell1.second];
        figures[cell1.first][cell1.second] = "  ";
        if (move == 2)
            figures[cell2.first - Pmode][cell2.second] = "  ";
        if (cell2.first == 3.5 + Pmode * 3.5 and figures[cell2.first][cell2.second][1] == 'P' and message) {
            std::string figure;
            do {
                std::cout << "On which figure do you want replace the pawn ";
                std::cin >> figure;
                if (Pmode == 1 and (figure == "WR" or figure == "WN" or figure == "WB" or figure == "WQ"))
                    break;
                if (Pmode == -1 and (figure == "BR" or figure == "BN" or figure == "BB" or figure == "BQ"))
                    break;
                std::cout << "you can`t choose this figure" << std::endl;
            } while (true);
            figures[cell2.first][cell2.second] = figure;
        }
        if (figures[cell2.first][cell2.second][1] == 'K')
            if (Pmode == 1)
                WhiteKingPosition.first = cell2.first, WhiteKingPosition.second = cell2.second;
            else
                BlackKingPosition.first = cell2.first, BlackKingPosition.second = cell2.second;


    }
    if (not message or (Shah(Pmode) and (move == 1 or move == 2))) {
        if (message) std::cout << "Нou can`t move this figure, shah!" << std::endl;
        figures[cell1.first][cell1.second] = figures[cell2.first][cell2.second];
        if(move)figures[cell2.first][cell2.second] = FigureCell2;
        if (move == 2)
            figures[cell2.first - Pmode][cell2.second] = std::string(((Pmode == 1) ? "W" : "B") + 'P');
        if (Shah(Pmode) and (move == 1 or move == 2))
            return false;
    }
    if (move == 3 ) 
        Castling = 1;
    if (move == 4 )
        Castling = -1;
    if (move > 2) {
        figures[cell1.first][cell1.second - 1 * Castling] = figures[cell2.first][cell2.second];
        figures[cell1.first][cell1.second - 2 * Castling] = figures[cell1.first][cell1.second];
        figures[cell1.first][cell1.second] = "  ";
        figures[cell2.first][cell2.second] = "  ";
        if (Pmode == 1)
            WhiteKingPosition.second = cell1.second - 2 * Castling;
        else
            BlackKingPosition.second = cell1.second - 2 * Castling;
    }
    if (cell2.first == 0 and cell2.second == 0)
        WhiteKingOrLeftRookMove = true;
    if (cell2.first == 0 and cell2.second == 7)
        WhiteKingOrRightRookMove = true;
    if (cell2.first == 7 and cell2.second == 0)
        BlackKingOrLeftRookMove = true;
    if (cell2.first == 7 and cell2.second == 7)
        BlackKingOrRightRookMove = true;
    if (move != 0 and (figures[cell1.first][cell1.second][1] == 'K' or figures[cell2.first][cell2.second][1] == 'K'))
        if (Pmode == 1)
            WhiteKingOrLeftRookMove = true, WhiteKingOrRightRookMove;
        else
            BlackKingOrLeftRookMove = true, BlackKingOrRightRookMove;
    if (figures[cell1.first][cell1.second][1] == 'R') {
        if (cell1.first == 0 and Pmode == 1) {
            if (cell1.second == 0)
                WhiteKingOrLeftRookMove = true;
            if (cell1.second == 7)
                WhiteKingOrRightRookMove = true;
        }
        if (cell1.first == 7 and Pmode == -1) {
            if (cell1.second == 0)
                BlackKingOrLeftRookMove = true;
            if (cell1.second == 7)
                BlackKingOrRightRookMove = true;
        }
    }
    if (figures[cell1.first][cell1.second][1] == 'K') {
        if (Pmode == 1)
            WhiteKingOrLeftRookMove = true, WhiteKingOrRightRookMove = true;
        else
            BlackKingOrLeftRookMove = true, BlackKingOrRightRookMove = true;
    }
    LastCell1 = cell1;
    LastCell2 = cell2;
    return 1;
}



int Chess::CheckMove(const std::pair<int, int>& cell1, const std::pair<int, int>& cell2, const int Pmode, bool message) const {
    if (cell2.first < 0 or cell2.first > 7 or cell2.second < 0 or cell2.second>7)
        return false;
    char NotMoverColor = (Pmode == 1) ? 'B' : 'W';
    if (figures[cell1.first][cell1.second] == "  ") {
        if(message)std::cout << "Empty cell" << std::endl;
        return 0;
    }
    if (figures[cell1.first][cell1.second][0] == NotMoverColor) {
        if (message) std::cout << "Figure of the opponent in selected cell" << std::endl;
        return 0;
    }
    if (cell1.first == cell2.first and cell1.second == cell2.second) {
        if (message)  std::cout << "You don`t move" << std::endl;
        return 0;
    }
    if (figures[cell1.first][cell1.second][1] == 'K' and figures[cell2.first][cell2.second][1] == 'R') {
        return CheckMoveCastling(cell1, cell2, Pmode, NotMoverColor, message);
    }
    if (figures[cell2.first][cell2.second] != "  " and figures[cell2.first][cell2.second][0] != NotMoverColor) {
        if (message)  std::cout << "Your figure in another selcted cell" << std::endl;
        return 0;
    }
    if (figures[cell1.first][cell1.second][1] == 'P') {
        return CheckMovePawn(cell1, cell2, Pmode, NotMoverColor, message);
    }
    if (figures[cell1.first][cell1.second][1] == 'R') {
        return CheckMoveRook(cell1, cell2, message);
    }
    if (figures[cell1.first][cell1.second][1] == 'N') {
        return CheckMoveKnight(cell1, cell2, message);
    }
    if (figures[cell1.first][cell1.second][1] == 'B') {
        return CheckMoveBishop(cell1, cell2, message);
    }
    if (figures[cell1.first][cell1.second][1] == 'Q') {
        return CheckMoveQeen(cell1, cell2, message);
    }
    if (figures[cell1.first][cell1.second][1] == 'K') {
        return 1;
    }
    else
        return 0;
}


int Chess::CheckMovePawn(const std::pair<int, int>& cell1, const std::pair<int, int>& cell2, const int Pmode, const char  NotMoverColor, bool message)const {
    int Step = Pmode * (cell2.first - cell1.first);
    if (Step > 0 and Step < 3) {
        if (cell1.second == cell2.second and CheckVerticalMove(cell1.first, cell2.first, cell1.second, Pmode, message) and figures[cell2.first][cell2.second] == "  ") {
            if (Step == 2)
                if (cell1.first == 3.5 - Pmode * 2.5)
                    return 1;
                else {
                    if (message) std::cout << "Can`t make two-cells move, because it`s not the first move that pawn " << std::endl;
                    return 0;
                }
            return 1;
        }

        else {
            if (Step == 1 and abs(cell1.second - cell2.second) == 1 and figures[cell2.first][cell2.second][0] == NotMoverColor)
                return 1;
            if (cell1.first == 3.5 + 0.5 * Pmode and figures[cell2.first - Pmode][cell2.second] == ((Pmode == 1) ? "BP" : "WP") and cell2.first - Pmode == LastCell2.first and cell2.second == LastCell2.second and -Pmode * (LastCell2.first - LastCell1.first) == 2)
                return 2;
            if (message)  std::cout << "There is no opponent's figure on the diagonal" << std::endl;
            return 0;

        }
    }
    if (message)  std::cout << "The pawn can`t make that pawn " << Step << std::endl;
    return 0;
}

int Chess::CheckMoveRook(const std::pair<int, int>& cell1, const std::pair<int, int>& cell2, bool message)const {//////////////
    if (cell1.first != cell2.first and cell1.second != cell2.second) {
        if (message)   std::cout << "The rook can only move perpendicular" << std::endl;
        return 0;
    }
    if (cell1.first != cell2.first)
        return CheckVerticalMove(cell1.first, cell2.first, cell1.second, (cell2.first - cell1.first) / abs(cell2.first - cell1.first), message);
    else
        return CheckGorizontalMove(cell1.second, cell2.second, cell1.first, message);
}

int Chess::CheckMoveKnight(const std::pair<int, int>& cell1, const std::pair<int, int>& cell2, bool message)const {
    if ((abs(cell1.first - cell2.first) == 2 and abs(cell1.second - cell2.second) == 1) or (abs(cell1.first - cell2.first) == 1 and abs(cell1.second - cell2.second) == 2))
        return 1;
    if (message)  std::cout << "Knight can`t move that way" << std::endl;
    return 0;
}

int Chess::CheckMoveBishop(const std::pair<int, int>& cell1, const std::pair<int, int>& cell2, bool message)const {
    if (abs(cell1.first - cell2.first) != abs(cell1.second - cell2.second)) {
        if (message) std::cout << "Wrong move for bishop" << std::endl;
        return 0;
    }
    return CheckDiagonalMove(cell1, cell2, message);
}

int Chess::CheckMoveQeen(const std::pair<int, int>& cell1, const std::pair<int, int>& cell2, bool message)const {
    if (cell1.first == cell2.first and cell1.second != cell2.second)
        return CheckGorizontalMove(cell1.second, cell2.second, cell1.first, message);
    if (cell1.first != cell2.first and cell1.second == cell2.second)
        return CheckVerticalMove(cell1.first, cell2.first, cell1.second, (cell2.first - cell1.first) / abs(cell2.first - cell1.first), message);
    if (abs(cell1.first - cell2.first) == abs(cell1.second - cell2.second)) {
        return CheckDiagonalMove(cell1, cell2, message);
    }
    if (message)  std::cout << "Wrong move for Qeen" << std::endl;
    return 0;
}

int Chess::CheckMoveCastling(const std::pair<int, int>& cell1, const std::pair<int, int>& cell2, const int Pmode, const char NotMoverColor, bool message)const {
    if (CheckGorizontalMove(cell1.second, cell2.second, cell1.first, false))
        if (Pmode == 1) {
            if (cell2.first == 0 and cell2.second == 0 and not WhiteKingOrLeftRookMove and CheckCellsForCastling(cell1, cell2, Pmode)) {
                return 3;
            }
            if (cell2.first == 0 and cell2.second == 7 and not WhiteKingOrRightRookMove and CheckCellsForCastling(cell1, cell2, Pmode)) {
                return 4;
            }
        }
        else {
            if (cell2.first == 7 and cell2.second == 0 and not BlackKingOrLeftRookMove and CheckCellsForCastling(cell1, cell2, Pmode)) {
                return 3;
            }
            if (cell2.first == 7 and cell2.second == 7 and not BlackKingOrRightRookMove and CheckCellsForCastling(cell1, cell2, Pmode)) {
                return 4;
            }
        }

    if (message)std::cout << "Castling is not possible" << std::endl;
    return 0;
}


bool Chess::CheckVerticalMove(const int v1, const int v2, const int g, const int Pmode, bool message)const {
    for (int i = v1 + Pmode; i != v2; i += Pmode) {
        if (figures[i][g] != "  ") {
            if (message)  std::cout << "This figure can`t jump over" << std::endl;
            return false;
        }
    }
    return true;
}

bool Chess::CheckGorizontalMove(const int g1, const int g2, const int v, bool message)const {
    int Pmode = (g2 - g1) / abs(g2 - g1);
    for (int i = g1 + Pmode; i != g2; i += Pmode) {
        if (figures[v][i] != "  ") {
            if (message) std::cout << "This figure can`t jump over" << std::endl;
            return false;
        }
    }
    return true;
}

bool Chess::CheckDiagonalMove(const std::pair<int, int>& cell1, const std::pair<int, int>& cell2, bool message)const {
    int Pmode1 = (cell2.first - cell1.first) / abs(cell2.first - cell1.first);
    int Pmode2 = (cell2.second - cell1.second) / abs(cell2.second - cell1.second);
    for (int i = cell1.first + Pmode1, j = cell1.second + Pmode2; i != cell2.first; i += Pmode1, j += Pmode2)
        if (figures[i][j] != "  ") {
            if (message) std::cout << "This figure can`t jump over" << std::endl;
            return 0;
        }
    return 1;
}

bool Chess::CheckDiagonalLine(const std::pair<int, int>& cell, const char NotMoverColor)const {
    for (int i = cell.first + 1, j = cell.second + 1; i < SizeFigures and j < SizeFigures; i++, j++) {
        if (figures[i][j][0] == NotMoverColor and (figures[i][j][1] == 'Q' or figures[i][j][1] == 'B'))
            return false;
        if (figures[i][j] != "  ")
            break;
    }
    for (int i = cell.first - 1, j = cell.second + 1; i > -1 and j < SizeFigures; i--, j++) {
        if (figures[i][j][0] == NotMoverColor and (figures[i][j][1] == 'Q' or figures[i][j][1] == 'B'))
            return false;
        if (figures[i][j] != "  ")
            break;
    }
    for (int i = cell.first + 1, j = cell.second - 1; i < SizeFigures and j > -1; i++, j--) {
        if (figures[i][j][0] == NotMoverColor and (figures[i][j][1] == 'Q' or figures[i][j][1] == 'B'))
            return false;
        if (figures[i][j] != "  ")
            break;
    }
    for (int i = cell.first - 1, j = cell.second - 1; i > -1 and j > -1; i--, j--) {
        if (figures[i][j][0] == NotMoverColor and (figures[i][j][1] == 'Q' or figures[i][j][1] == 'B'))
            return false;
        if (figures[i][j] != "  ")
            break;
    }
    return true;

}

bool Chess::CheckVerticalLine(const std::pair<int, int>& cell, const char NotMoverColor)const {
    for (int i = cell.first + 1; i < SizeFigures; i++) {
        if (figures[i][cell.second][0] == NotMoverColor and (figures[i][cell.second][1] == 'Q' or figures[i][cell.second][1] == 'R'))
            return false;
        if (figures[i][cell.second] != "  ")
            break;
    }
    for (int i = cell.first - 1; i > -1; i--) {
        if (figures[i][cell.second][0] == NotMoverColor and (figures[i][cell.second][1] == 'Q' or figures[i][cell.second][1] == 'R'))
            return false;
        if (figures[i][cell.second] != "  ")
            break;
    }
    return true;
}

bool Chess::CheckGorizontalLine(const std::pair<int, int>& cell, const char NotMoverColor)const {
    for (int i = cell.second + 1; i < SizeFigures; i++) {
        if (figures[cell.first][i][0] == NotMoverColor and (figures[cell.first][i][1] == 'Q' or figures[cell.first][i][1] == 'R'))
            return false;
        if (figures[cell.first][i] != "  ")
            break;
    }
    for (int i = cell.second - 1; i > -1; i--) {
        if (figures[cell.first][i][0] == NotMoverColor and (figures[cell.first][i][1] == 'Q' or figures[cell.first][i][1] == 'R'))
            return false;
        if (figures[cell.first][i] != "  ")
            break;
    }
    return true;
}

bool Chess::CheckKnight(const std::pair<int, int>& cell, const char NotMoverColor)const {
    std::string knight = "" + NotMoverColor + 'N';
    for (int i = -2; i <= 2; i++)
        for (int j = -2; j <= 2; j++)
            if (i != j and i != 0 and j != 0 and cell.first + i > -1 and cell.first + i < SizeFigures and cell.second + j > -1 and cell.second + j < SizeFigures and figures[cell.first + i][cell.second + j] == knight)
                return false;
    return true;
}




bool Chess::CheckCellsForCastling(const std::pair<int, int>& cell1, const std::pair<int, int>& cell2, const int Pmode)const {
    int Step = (cell2.second - cell1.second) / abs(cell2.second - cell1.second);
    for (int i = cell1.second; i != cell2.second - Step; i += Step)
        if (!CheckCell(std::pair<int, int>(cell1.first, i),Pmode, (Pmode == 1) ? 'B' : 'W'))
            return false;
    return true;
}




bool Chess::CheckCell(const std::pair<int, int>& cell,const int Pmode, const char NotMoverColor)const {
    std::string NotMoverPawn ;
    std::string NotMoverKing;
    if (Pmode == 1)
        NotMoverPawn = "BP",NotMoverKing = "BK";
    else
        NotMoverPawn = "WP", NotMoverKing = "WK";
    if (cell.first + Pmode < SizeFigures and cell.first + Pmode >= 0 and (figures[cell.first + Pmode][cell.second] == NotMoverKing
        or (cell.second - 1 >=0 and (figures[cell.first + Pmode][cell.second - 1] == NotMoverPawn or figures[cell.first + Pmode][cell.second - 1] == NotMoverKing)) 
        or (cell.second + 1 < SizeFigures and (figures[cell.first + Pmode][cell.second + 1] == NotMoverPawn or figures[cell.first + Pmode][cell.second + 1] == NotMoverKing))))
        return false;
    return CheckDiagonalLine(cell, NotMoverColor) and CheckGorizontalLine(cell, NotMoverColor) and CheckVerticalLine(cell, NotMoverColor) and CheckKnight(cell, NotMoverColor);
}

bool Chess::Shah(const int Pmode)const {
    return not CheckCell(((Pmode == 1) ? WhiteKingPosition : BlackKingPosition),Pmode, (Pmode == 1) ? 'B' : 'W');
}

bool Chess::ExistenceMove(const int Pmode, const char NotMoverColor) {
    for (int i = 0; i < SizeFigures; i++)
        for (int j = 0; j < SizeFigures; j++) {
            if (figures[i][j][0] != NotMoverColor and figures[i][j] != "  ") {
                if (figures[i][j][1] == 'P') {
                    if (Move(Pmode, std::pair<int, int>(i, j), std::pair<int, int>(i + Pmode, j), false))
                        return true;
                    if (Move(Pmode, std::pair<int, int>(i, j), std::pair<int, int>(i + Pmode, j + Pmode), false))
                        return true;
                    if (Move(Pmode, std::pair<int, int>(i, j), std::pair<int, int>(i + Pmode, j - Pmode), false))
                        return true;
                }
                if (figures[i][j][1] == 'R') {
                    for (int k = 0; k < SizeFigures; k++) {
                        if (Move(Pmode, std::pair<int, int>(i, j), std::pair<int, int>(k, j), false))
                            return true;
                        if (Move(Pmode, std::pair<int, int>(i, j), std::pair<int, int>(i, k), false))
                            return true;
                    }
                }
                if (figures[i][j][1] == 'N') {
                    for (int k = -2; k <= 2; k++)
                        for (int z = -2; z <= 2; z++)
                            if (k != z and k != 0 and z != 0 and Move(Pmode, std::pair<int, int>(i, j), std::pair<int, int>(i + k, j + z), false))
                                return true;

                }
                if (figures[i][j][1] == 'B') {
                    for (int k = -SizeFigures + 1; k < SizeFigures; k++) {
                        if (Move(Pmode, std::pair<int, int>(i, j), std::pair<int, int>(i + k, j + k), false))
                            return true;
                        if (Move(Pmode, std::pair<int, int>(i, j), std::pair<int, int>(i + k, j - k), false))
                            return true;
                    }
                }
                if (figures[i][j][1] == 'Q') {
                    for (int k = -SizeFigures + 1; k < SizeFigures; k++) {
                        if (Move(Pmode, std::pair<int, int>(i, j), std::pair<int, int>(i + k, j + k), false))
                            return true;
                        if (Move(Pmode, std::pair<int, int>(i, j), std::pair<int, int>(i + k, j - k), false))
                            return true;
                        if (Move(Pmode, std::pair<int, int>(i, j), std::pair<int, int>(k, j), false))
                            return true;
                        if (Move(Pmode, std::pair<int, int>(i, j), std::pair<int, int>(i, k), false))
                            return true;
                    }
                }
                if (figures[i][j][1] == 'K')
                    for (int k = -1; k <= 1; k++)
                        for (int z = -1; z <= 1; z++) 
                            if (Move(Pmode, std::pair<int, int>(i, j), std::pair<int, int>(i + k, j + z), false))
                                return true;
            }

        }
    return false;

}
   
