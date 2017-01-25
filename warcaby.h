#ifndef WARCABY_H
#define WARCABY_H

#include "mainwindow.h"

class Warcaby
{
public:
    Warcaby();
    ~Warcaby();
    void mouseClick();
    void setBoardType(int type);
    void clearBoard();

private:
    MainWindow mainWindow;
    enum pieces{free=0,white=1,black=2,whiteKing=3,blackKing=4,notUse=5};
    int** board;
    int turn;
    int boardType;
    void initBoard();
    void deleteBoard();
    int tempColumn;
    int tempLine;
    bool fromPointChecked;
    int pointFromColumn;
    int pointFromLine;
    int pointToColumn;
    int pointToLine;
    int pointPossibleCaptureColumn;
    int pointPossibleCaptureLine;
    int droughtCaptureColumn;
    int droughtCaptureLine;
    bool mustFromHere;
    bool isPossibleCapture;
    void whichFieldCheked();
    bool isMoved(int pointFromCol,int pointFromLin,int pointToCol,int pointToLin);
    bool isCapture(int pointFromCol, int pointFromLin, int pointToCol, int pointToLin);
    void chengeTurn();
    int oppositeturn();
    bool isPlayerPicesIsHere();
    bool isMovePossible(int column, int line);
    bool isCapturePossible(int column, int line);
    bool itWasPossibleCapture();
    bool isCapturePossibleKing(int directionColumn,int directionLine,int drauhtInLine);
    bool isAnyMovePossible();
    void toKing();
    int winGame();
};

#endif // WARCABY_H
