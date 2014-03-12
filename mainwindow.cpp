#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGraphicsScene>
#include <QDebug>
#include <QKeyEvent>
#include <QFile>
#include <QGraphicsPixmapItem>
#include "gamepacman.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_Scene(new QGraphicsScene(this))
{
    ui->setupUi(this);
    ui->graphicsView->setScene(m_Scene);
    ui->graphicsView->installEventFilter(this);

    m_Game = new game::GamePacman(ui->graphicsView, m_Scene, this);

    ui->lbl_Result->hide();

    connect(m_Game, &game::GamePacman::scoreChanged, [this](int score){
        ui->lbl_Score->setText(QString::number(score));
    });
    connect(m_Game, &game::GamePacman::pointsCountChanged, [this](int count){
        ui->lbl_Points->setText(QString::number(count));
    });

    connect(m_Game, &game::GamePacman::gameEnded, [this](){
        ui->lbl_Result->show();
        if (m_Game->pointsCount() == 0) {
            ui->lbl_Result->setText(tr("Победа!\nНажмите \"R\" для начала новой игры."));
        } else {
            ui->lbl_Result->setText(tr("Вы проиграли... \nНажмите \"R\" для начала новой игры."));
        }
    });

    m_Game->start();
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::eventFilter(QObject *obj, QEvent *ev)
{
    if (obj == ui->graphicsView) {
        if (ev->type() == QEvent::KeyPress) {
            QKeyEvent *key_ev = static_cast<QKeyEvent*>(ev);

            switch (key_ev->key()) {
            case Qt::Key_Up:
                m_Game->pacmanToTop();
                break;
            case Qt::Key_Down:
                m_Game->pacmanToBottom();
                break;
            case Qt::Key_Left:
                m_Game->pacmanToLeft();
                break;
            case Qt::Key_Right:
                m_Game->pacmanToRight();
                break;
            case Qt::Key_R:
                if (!m_Game->inGame()) {
                    ui->lbl_Result->clear();
                    ui->lbl_Result->hide();
                    ui->lbl_Points->setText("0");
                    ui->lbl_Points->setText("0");

                    m_Game->prepareToStart();
                    m_Game->start();
                }
                break;
            }
        }

    }

    return QMainWindow::eventFilter(obj, ev);
}
