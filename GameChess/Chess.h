#pragma once
#ifndef CHESS_H
#define CHESS_H
#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include <assert.h>

class TestChess;


class Chess {
    std::vector<std::vector<std::string>> figures;
    std::vector<std::string> board;
    std::pair<int, int> LastCell1;
    std::pair<int, int> LastCell2;
    const int SizeFigures = 8;
    const int SizeBoard = 21;
    bool WhiteKingOrLeftRookMove;
    bool WhiteKingOrRightRookMove;
    bool BlackKingOrLeftRookMove;
    bool BlackKingOrRightRookMove;
    std::pair<int, int> WhiteKingPosition;
    std::pair<int, int> BlackKingPosition;
    void ShowGame()const;
    std::string CharToLowerString(const char*)const;
    void NewGame();
    void LoadGame();
    void Save(const int)const;
    void Help()const;///////////write help
    void Game(int i = 1);
    bool Move(const int, const std::pair<int, int>&, const std::pair<int, int>&, bool message = true);
    int CheckMove(const std::pair<int, int>& cell1, const std::pair<int, int>& cell2, const int Pmode, bool Message = true)const;
    int CheckMovePawn(const std::pair<int, int>& cell1, const std::pair<int, int>& cell2, const int Pmode, const char NotMoverColor, bool Message = true)const;
    int CheckMoveRook(const std::pair<int, int>& cell1, const std::pair<int, int>& cell2, bool Message = true)const;
    int CheckMoveKnight(const std::pair<int, int>& cell1, const std::pair<int, int>& cell2, bool Message = true)const;
    int CheckMoveBishop(const std::pair<int, int>& cell1, const std::pair<int, int>& cell2, bool Message = true)const;
    int CheckMoveQeen(const std::pair<int, int>& cell1, const std::pair<int, int>& cell2, bool Message = true)const;
    int CheckMoveCastling(const std::pair<int, int>& cell1, const std::pair<int, int>& cell2, const int Pmode, const char NotMoverColor, bool Message = true)const;
    bool CheckVerticalMove(const int, const int, const int, const int, bool Message = true)const;
    bool CheckGorizontalMove(const int, const int, const int, bool pov = true)const;
    bool CheckDiagonalMove(const std::pair<int, int>& cell1, const std::pair<int, int>& cell2, bool Message = true)const;
    bool CheckDiagonalLine(const std::pair<int, int>& cell, const char NotMoverColor)const;
    bool CheckVerticalLine(const std::pair<int, int>& cell, const char NotMoverColor)const;
    bool CheckGorizontalLine(const std::pair<int, int>& cell, const char NotMoverColor)const;
    bool CheckKnight(const std::pair<int, int>& cell, const char NotMoverColor)const;
    bool CheckCell(const std::pair<int, int>& cell,const int Pmode, const char NotMoverColor)const;
    bool CheckCellsForCastling(const std::pair<int, int>& cell1, const std::pair<int, int>& cell2, const int Pmode)const;
    bool Shah(const int Pmode)const;
    bool ExistenceMove(const int Pmode, const char NoMoverColor);
    bool parsing(const std::string& StringCommand, std::pair<int, int>& cell1, std::pair<int, int>& cell2)const;
    friend class TestChess;
public:
    Chess();
    ~Chess();
    void menu();
};


class TestChess {
public:
    void Testing();
    void TestExistenceMove();
    void TestShah();
    void TestMoveCastling();
    void TestCheckMoveQeen();
    void TestCheckMoveKnight();
    void TestCheckMovePawn();
    void TestCheckMove();
};









#endif