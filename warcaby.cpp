#include "warcaby.h"
#include "mainwindow.h"

static const int BOARD_WIDTH=10;
static const int BOARD_HIGHT=10;

Warcaby::Warcaby()
    :mainWindow(this)
{
    initBoard();
    boardType=0;
    clearBoard();
    mainWindow.show();
}

Warcaby::~Warcaby()
{
    deleteBoard();
}


void Warcaby::initBoard()
{
    board=new int*[BOARD_HIGHT];
    for (int i=0;i<BOARD_HIGHT;i++)
    {
        board[i]=new int[BOARD_WIDTH];
    }
}

void Warcaby::setBoardType(int type)
{
    boardType=type;
}

void Warcaby::clearBoard()
{
    fromPointChecked=false;
    turn=white;
    mustFromHere=false;
    isPossibleCapture=false;
    if (boardType==0)
    {
        for(int i=0;i<BOARD_HIGHT;i++)
        {
            for(int j=0;j<BOARD_WIDTH;j++)
            {
                if ((i==0||i==2)&&(j==1||j==3||j==5||j==7||j==9))
                    board[i][j]=black;
                else if ((i==1||i==3)&&(j==0||j==2||j==4||j==6||j==8))
                    board[i][j]=black;
                else if ((i==6||i==8)&&(j==1||j==3||j==5||j==7||j==9))
                    board[i][j]=white;
                else if ((i==7||i==9)&&(j==0||j==2||j==4||j==6||j==8))
                    board[i][j]=white;
                else if ((i==4)&&(j==1||j==3||j==5||j==7||j==9))
                    board[i][j]=free;
                else if ((i==5)&&(j==0||j==2||j==4||j==6||j==8))
                    board[i][j]=free;
                else
                    board[i][j]=notUse;
            }
        }
    }
    else
    {
        for(int i=0;i<BOARD_HIGHT;i++)
        {
            for(int j=0;j<BOARD_WIDTH;j++)
            {
                if ((i==0||i==2)&&(j==1||j==3||j==5||j==7))
                    board[i][j]=black;
                else if ((i==1)&&(j==0||j==2||j==4||j==6))
                    board[i][j]=black;
                else if ((i==6)&&(j==1||j==3||j==5||j==7))
                    board[i][j]=white;
                else if ((i==5||i==7)&&(j==0||j==2||j==4||j==6))
                    board[i][j]=white;
                else if ((i==4)&&(j==1||j==3||j==5||j==7))
                    board[i][j]=free;
                else if ((i==3)&&(j==0||j==2||j==4||j==6))
                    board[i][j]=free;
                else
                    board[i][j]=notUse;
            }
        }
    }
    mainWindow.showBoard(board,turn);
}

void Warcaby::deleteBoard()
{
    for (int i=0;i<BOARD_HIGHT;i++)
    {
        delete board[i];
    }
    delete board;
}

void Warcaby::mouseClick()
{
    if (!fromPointChecked)
    {
        whichFieldCheked();
        if (!isPlayerPicesIsHere())
        {
            mainWindow.setCheked(tempColumn,tempLine,false);
            mainWindow.showTextToPlayer("Tu nie ma twojego pionka!");
        }
        else if (!isMovePossible(tempColumn,tempLine)&&!(isCapturePossible(tempColumn,tempLine)))
        {
            mainWindow.setCheked(tempColumn,tempLine,false);
            mainWindow.showTextToPlayer("Tym pionkiem nie masz gdzie się poruszyć.");
        }
        else
        {
            mainWindow.showTextToPlayer("Wybrałeś pionka, gdzie się chcesz teraz poruszyć?");
            pointFromColumn=tempColumn;
            pointFromLine=tempLine;
            fromPointChecked=true;
        }
    }
    else if(!mainWindow.isCheked(pointFromColumn,pointFromLine))
    {
        if(mustFromHere)
        {
            mainWindow.showTextToPlayer("Nie możesz teraz ruszyć innym pionkiem!");
            mainWindow.setChekedAll(false);
            mainWindow.setCheked(pointFromColumn,pointFromLine,true);
        }
        else
        {
            fromPointChecked=false;
            mainWindow.setChekedAll(false);
        }
    }
    else
    {
        whichFieldCheked();
        pointToColumn=tempColumn;
        pointToLine=tempLine;
        if (!board[tempColumn][tempLine]==free)
        {
            mainWindow.showTextToPlayer("Pole zajęte, wybierz inne pole.");
            mainWindow.setCheked(tempColumn,tempLine,false);
        }
        else if (isCapture(pointFromColumn,pointFromLine,pointToColumn,pointToLine))
        {
            tempColumn=pointToColumn;
            tempLine=pointToLine;
            board[pointToColumn][pointToLine]=board[pointFromColumn][pointFromLine];
            board[pointFromColumn][pointFromLine]=free;
            board[droughtCaptureColumn][droughtCaptureLine]=free;
            if (isCapturePossible(tempColumn,tempLine))
            {
                mainWindow.showTextToPlayer("Pionek zbity, możliwe kolejne bicie");
                fromPointChecked=true;
                mainWindow.setChekedAll(false);
                mainWindow.setCheked(pointToColumn,pointToLine,true);
                mustFromHere=true;
                pointFromColumn=tempColumn;
                pointFromLine=tempLine;
            }
            else
            {
                mainWindow.showTextToPlayer("Pionek zbity.");
                fromPointChecked=false;
                mainWindow.setChekedAll(false);
                chengeTurn();
                mustFromHere=false;
            }
        }

        else if (itWasPossibleCapture()&&isMoved(pointFromColumn,pointFromLine,pointToColumn,pointToLine))
        {
            if (pointPossibleCaptureColumn==pointFromColumn&&pointPossibleCaptureLine==pointFromLine)
                board[pointToColumn][pointToLine]=free;
            mainWindow.showTextToPlayer("Było możliwe bicie, pionek zdjęty");
            board[pointPossibleCaptureColumn][pointPossibleCaptureLine]=free;
            fromPointChecked=false;
            mainWindow.setChekedAll(false);
            isPossibleCapture=false;
            chengeTurn();
        }
        else if (!itWasPossibleCapture()&&isMoved(pointFromColumn,pointFromLine,pointToColumn,pointToLine))
        {
            mainWindow.showTextToPlayer("Wykonano ruch.");
            fromPointChecked=false;
            mainWindow.setChekedAll(false);
            chengeTurn();
        }
        else
        {
            mainWindow.showTextToPlayer("Niepoprawny ruch.");
            mainWindow.setCheked(tempColumn,tempLine,false);
        }

    }
    toKing();
    if (winGame()==1)
    {
        mainWindow.showTextToPlayer("Wygrał gracz z czarnymi pionkami. Gratulacje!");
        chengeTurn();

    }
    else if (winGame()==2)
    {
        mainWindow.showTextToPlayer("Wygrał gracz z białymi pionkami. Gratulacje!");
        chengeTurn();
    }
    else if (winGame()==3)
    {
        mainWindow.showTextToPlayer("Koniec gry, brak możliwości ruchu gracza.");
    }

    mainWindow.showBoard(board,turn);
}

void Warcaby::whichFieldCheked()
{
    for (int i=0;i<BOARD_HIGHT;i++)
    {
        for (int j=0;j<BOARD_WIDTH;j++)
        {
            if (mainWindow.isCheked(i,j)&&!(pointFromColumn==i&&pointFromLine==j&&fromPointChecked))
            {
                tempColumn=i;
                tempLine=j;
                break;
            }
        }
    }
}

bool Warcaby::isCapture(int pointFromCol,int pointFromLin,int pointToCol,int pointToLin)
{
    if (board[pointFromCol][pointFromLin]==white||board[pointFromCol][pointFromLin]==black)
    {
        if (((abs(pointFromCol-pointToCol))==2)&&((abs(pointFromLin-pointToLin))==2)
                &&(board[(pointFromCol+pointToCol)/2][(pointFromLin+pointToLin)/2]==oppositeturn()
                ||board[(pointFromCol+pointToCol)/2][(pointFromLin+pointToLin)/2]==oppositeturn()+2))
        {
            droughtCaptureColumn=(pointFromCol+pointToCol)/2;
            droughtCaptureLine=(pointFromLin+pointToLin)/2;
            return true;
        }
        else
            return false;
    }
    else
    {
        if (abs(pointFromCol-pointToCol)==(abs(pointFromLin-pointToLin)))
        {
            int i=0;
            int j=0;
            int draughtInLine=0;
            while (i!=(pointToCol-pointFromCol)&&j!=(pointToLin-pointFromLin))
            {

                if ((pointToCol-pointFromCol)>0)
                    i++;
                else
                    i--;
                if ((pointToLin-pointFromLin)>0)
                    j++;
                else
                    j--;
                if (board[pointFromCol+i][pointFromLin+j]==turn
                        ||board[pointFromCol+i][pointFromLin+j]==turn+2)
                    return false;
                else if (draughtInLine==0&&(board[pointFromCol+i][pointFromLin+j]==oppositeturn()
                                            ||board[pointFromCol+i][pointFromLin+j]==oppositeturn()+2))
                {
                    droughtCaptureColumn=pointFromCol+i;
                    droughtCaptureLine=pointFromLin+j;
                    draughtInLine++;
                }
                else if (draughtInLine==1&&board[pointFromCol+i][pointFromLin+j]!=free)
                    return false;
            }
            if (draughtInLine==1)
                return true;
        }
        else
            return false;
    }
    return false;
}


bool Warcaby::isMoved(int pointFromCol,int pointFromLin,int pointToCol,int pointToLin)
{
    if (board[pointFromCol][pointFromLin]==white||board[pointFromCol][pointFromLin]==black)
    {
        if ((((pointFromCol-pointToCol)==1&&turn==white)||((pointFromCol-pointToCol)==-1&&turn==black))
                &&((abs(pointFromLin-pointToLin))==1))
        {
            board[pointToCol][pointToLin]=board[pointFromCol][pointFromLin];
            board[pointFromCol][pointFromLin]=free;
            return true;
        }
        else
            return false;
    }
    else
    {
        if (abs(pointFromCol-pointToCol)==(abs(pointFromLin-pointToLin)))
        {
            int i=0;
            int j=0;
            while (i!=(pointToCol-pointFromCol)&&j!=(pointToLin-pointFromLin))
            {

                if ((pointToCol-pointFromCol)>0)
                    i++;
                else
                    i--;
                if ((pointToLin-pointFromLin)>0)
                    j++;
                else
                    j--;
                if (board[pointFromCol+i][pointFromLin+j]!=free)
                    return false;
            }
            board[pointToCol][pointToLin]=board[pointFromCol][pointFromLin];
            board[pointFromCol][pointFromLin]=free;
            return true;
        }
        else
            return false;
    }
    return false;
}


void Warcaby::chengeTurn()
{
    if (turn==white)
        turn=black;
    else
        turn=white;
}

int Warcaby::oppositeturn()
{
    if (turn==white)
        return black;
    else
        return white;
}

bool Warcaby::isPlayerPicesIsHere()
{
    if ((turn==white)&&(board[tempColumn][tempLine]==white||board[tempColumn][tempLine]==whiteKing))
        return true;
    if ((turn==black)&&(board[tempColumn][tempLine]==black||board[tempColumn][tempLine]==blackKing))
        return true;
    else return false;
}


bool Warcaby::isMovePossible(int column,int line)
{
    int i,j;
    if ((column-1)>=0)
        i=column-1;
    else i=column+1;
    while((i<=column+1)&&(i<BOARD_HIGHT)&&(i>=0))
    {
        if ((line-1)>=0)
            j=line-1;
        else j=line+1;
        while((j<=line+1)&&(j<BOARD_WIDTH)&&(j>=0))
        {
            if ((((column>i)&&(turn==white))||((column<i)&&(turn==black)))
                    &&(board[i][j]==free)&&(board[column][line]==turn))
                return true;
            else if ((board[i][j]==free)&&(board[column][line]==turn+2))
                return true;
            j=j+2;
        }
        i=i+2;
    }
    return false;
}


bool Warcaby::isCapturePossible(int column,int line)
{
    for (int i=0;i<BOARD_HIGHT;i++)
    {
        for (int j=0;j<BOARD_WIDTH;j++)
        {
            if (board[i][j]==free&&(abs(column-i)==abs(line-j))&&isCapture(column,line,i,j))
            {
                pointPossibleCaptureColumn=column;
                pointPossibleCaptureLine=line;
                return true;
            }
        }
    }
    return false;
}

bool Warcaby::itWasPossibleCapture()
{
    for (int i=0;i<BOARD_HIGHT;i++)
    {
        for (int j=0;j<BOARD_WIDTH;j++)
        {
            if (board[i][j]==turn||board[i][j]==turn+2)
            {
                if (isCapturePossible(i,j))
                    return true;
            }
        }
    }
    return false;
}

bool Warcaby::isAnyMovePossible()
{
    for (int i=0;i<BOARD_HIGHT;i++)
    {
        for (int j=0;j<BOARD_WIDTH;j++)
        {
            if (board[i][j]==turn||board[i][j]==turn+2)
            {
                if (isMovePossible(i,j)||(isCapturePossible(i,j)))
                    return true;
            }
        }
    }
    return false;
}

void Warcaby::toKing()
{
    for (int i=0;i<BOARD_WIDTH;i++)
    {
        if (board[9][i]==black&&boardType==0)
            board[9][i]=blackKing;
        else if (board[7][i]==black&&boardType==1)
            board[7][i]=blackKing;
        else if (board[0][i]==white)
            board[0][i]=whiteKing;
    }
}


int Warcaby::winGame()
{
    bool isHereWhiteDraught=false;
    bool isHereBlackDraught=false;
    for (int i=0;i<BOARD_HIGHT;i++)
    {
        for (int j=0;j<BOARD_WIDTH;j++)
        {
            if (board[i][j]==white||board[i][j]==whiteKing)
                isHereWhiteDraught=true;
            else if (board[i][j]==black||board[i][j]==blackKing)
                isHereBlackDraught=true;
        }
    }

    if (!isHereBlackDraught)
        return 2;
    else if (!isHereWhiteDraught)
        return 1;
    else if (!isAnyMovePossible())
        return 3;
    else
        return 0;
}

