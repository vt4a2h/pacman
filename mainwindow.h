#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QGraphicsScene;

namespace game {
    class GamePacman;
}

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    bool eventFilter(QObject *obj, QEvent *ev);

private:
    Ui::MainWindow *ui;
    QGraphicsScene   *m_Scene;
    game::GamePacman *m_Game;
};

#endif // MAINWINDOW_H
