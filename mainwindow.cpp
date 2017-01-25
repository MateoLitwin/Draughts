#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QString>
#include <QToolButton>
#include <QPixmap>
#include "warcaby.h"

static const int BOARD_WIDTH=10;
static const int BOARD_HIGHT=10;

MainWindow::MainWindow(Warcaby *p_game) :
    ui(new Ui::MainWindow),
    game(p_game)
{
    ui->setupUi(this);
    initBoardView();
    initImgPices();
    initBoardConnect();
}

MainWindow::~MainWindow()
{
    delete ui;
    deleteBoardView();

}

void MainWindow::initImgPices()
{
    imgPices[0].addFile(":/resources/free.png");
    imgPices[1].addFile(":/resources/white.png");
    imgPices[2].addFile(":/resources/black.png");
    imgPices[3].addFile(":/resources/white_d.png");
    imgPices[4].addFile(":/resources/black_d.png");
    imgPices[5].addFile(":/resources/not_used.png");
    imgTurn[0].load(":/resources/white.png");
    imgTurn[1].load(":/resources/black.png");
}

void MainWindow::showBoard(int **board, int turn)
{
    for (int i=0;i<BOARD_HIGHT;i++)
    {
        for (int j=0;j<BOARD_WIDTH;j++)
        {
            if (board[i][j]==free)
            {
                boardItem[i][j]->setIcon(imgPices[free]);
                boardItem[i][j]->setVisible(true);
                boardItem[i][j]->setEnabled(true);
            }
            else if (board[i][j]==white)
            {
                boardItem[i][j]->setIcon(imgPices[white]);
                boardItem[i][j]->setVisible(true);
                boardItem[i][j]->setEnabled(true);
            }
            else if (board[i][j]==black)
            {
                boardItem[i][j]->setIcon(imgPices[black]);
                boardItem[i][j]->setVisible(true);
                boardItem[i][j]->setEnabled(true);
            }
            else if (board[i][j]==whiteKing)
            {
                boardItem[i][j]->setIcon(imgPices[whiteKing]);
                boardItem[i][j]->setVisible(true);
                boardItem[i][j]->setEnabled(true);
            }
            else if (board[i][j]==blackKing)
            {
                boardItem[i][j]->setIcon(imgPices[blackKing]);
                boardItem[i][j]->setVisible(true);
                boardItem[i][j]->setEnabled(true);
            }
            else
            {
                boardItem[i][j]->setEnabled(false);
                boardItem[i][j]->setIcon(imgPices[notUse]);
                boardItem[i][j]->setVisible(false);
            }
        }
    }
    if (turn==1)
        ui->PlayerTurn->setPixmap(imgTurn[0]);
    else
        ui->PlayerTurn->setPixmap(imgTurn[1]);
}

void MainWindow::initBoardView()
{
    boardItem=new QToolButton**[BOARD_HIGHT];
    for (int i=0;i<BOARD_HIGHT;i++)
    {
        boardItem[i]=new QToolButton*[BOARD_WIDTH];
    }
    for (int i=0;i<BOARD_HIGHT;i++)
    {
        for (int j=0;j<BOARD_WIDTH;j++)
        {
            QString name;
            switch (j)
            {
            case 0:
                name='A';
                break;
            case 1:
                name='B';
                break;
            case 2:
                name='C';
                break;
            case 3:
                name='D';
                break;
            case 4:
                name='E';
                break;
            case 5:
                name='F';
                break;
            case 6:
                name='G';
                break;
            case 7:
                name='H';
                break;
            case 8:
                name='I';
                break;
            case 9:
                name='J';
                break;
            }
            name+=QString::number(i+1);
            boardItem[i][j]=findChild<QToolButton*>(name);
        }
    }
}

void MainWindow::initBoardConnect()
{
    for (int i=0;i<BOARD_HIGHT;i++)
    {
        for (int j=0;j<BOARD_WIDTH;j++)
        {
            QObject::connect(boardItem[i][j],&QToolButton::clicked,this,&MainWindow::mouseClickOnBoard);
        }
    }
}

void MainWindow::deleteBoardView()
{
    for (int i=0;i<BOARD_HIGHT;i++)
    {
        delete boardItem[i];
    }
    delete boardItem;
}

void MainWindow::showTextToPlayer(QString text)
{
    ui->Info->setPlainText(text);
}

bool MainWindow::isCheked(int column, int line)
{
    return boardItem[column][line]->isChecked();
}

void MainWindow::setCheked(int column, int line, bool cheked)
{
    boardItem[column][line]->setChecked(cheked);
}

void MainWindow::mouseClickOnBoard()
{

    game->mouseClick();
}

void MainWindow::setChekedAll(bool cheked)
{
    for (int i=0;i<BOARD_HIGHT;i++)
    {
        for (int j=0;j<BOARD_WIDTH;j++)
        {
             boardItem[i][j]->setChecked(cheked);
        }
    }
}


void MainWindow::fieldUncheck(int column,int line)
{
    boardItem[column][line]->setChecked(false);
}

void MainWindow::on_gameSize_currentIndexChanged(int index)
{
    game->setBoardType(index);
    ui->Info->setPlainText("");
    if (index==0)
    {
        ui->I->setVisible(true);
        ui->J->setVisible(true);
        ui->_9->setVisible(true);
        ui->_10->setVisible(true);
    }
    else
    {
        ui->I->setVisible(false);
        ui->J->setVisible(false);
        ui->_9->setVisible(false);
        ui->_10->setVisible(false);
    }
    game->clearBoard();
}

void MainWindow::on_NewGame_clicked()
{
    game->clearBoard();
    ui->Info->setPlainText("");
}
