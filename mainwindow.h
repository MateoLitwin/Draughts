#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QToolButton;
class QPixmap;
class QString;
class Warcaby;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(Warcaby *p_game);
    ~MainWindow();
    void showBoard(int **board,int turn);
    void showTextToPlayer(QString text);
    bool isCheked(int column,int line);
    void setCheked(int column,int line,bool cheked);
    void setChekedAll(bool cheked);

private slots:
    void mouseClickOnBoard();

    void on_gameSize_currentIndexChanged(int index);

    void on_NewGame_clicked();

private:
    Ui::MainWindow *ui;
    enum pieces{free=0,white=1,black=2,whiteKing=3,blackKing=4,notUse=5};
    QToolButton*** boardItem;
    QIcon imgPices[6];
    QPixmap imgTurn[2];
    void initBoardView();
    void initImgPices();
    void initBoardConnect();
    void deleteBoardView();
    void mouseClick();
    void whichFieldCheked();
    void fieldUncheck(int column, int line);
    Warcaby *game;
};

#endif // MAINWINDOW_H
