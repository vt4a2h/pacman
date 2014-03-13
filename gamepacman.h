#pragma once

#include <QHash>
#include <QStringList>
#include <QPointF>
#include <QObject>

class QGraphicsView;
class QGraphicsScene;
class QGraphicsPixmapItem;
class QTimer;

namespace graphics_item {
    class Grid;
}

namespace game {

    class GamePacman : public QObject
    {
        Q_OBJECT
    public:
        GamePacman(QGraphicsView *view, QGraphicsScene *scene, QObject *parent = 0);

        void prepareToStart();
        void start();
        void stop();

        int score() const;
        int pointsCount() const;
        bool inGame() const;

    public slots:
        void pacmanToTop();
        void pacmanToBottom();
        void pacmanToLeft();
        void pacmanToRight();

    signals:
        void scoreChanged(int score);
        void pointsCountChanged(int pointsCount);
        void gameEnded();

    private:        
        enum Direction {Left, Right, Top, Bottom};
        enum GhostParameters {Type, Name, LastPos};

        void generatePoints();
        void makeGhosts();
        void genHelper(QPointF p, size_t h, size_t v, qreal xDef, qreal yAdd);

        void pacmanStep();
        void ghostsStep();
        void ghostStep(QGraphicsPixmapItem *ghost);

        void fixOutOfGameField(QPointF &p);     
        QList<QPointF> findPossibleSteps(QGraphicsPixmapItem *ghost);       
        QPointF findPossibleStep(QGraphicsPixmapItem *ghost);

        void makeDecisionPoints();
        QPointF makeDecision(QGraphicsPixmapItem *ghost);

        QPointF makeMovePoint(QGraphicsPixmapItem *item, Direction direction);

        QGraphicsView  *m_View;
        QGraphicsScene *m_Scene;

        QTimer *m_PacmanTimer;
        QTimer *m_GhostTimer;

        graphics_item::Grid *m_Grid;
        QGraphicsPixmapItem *m_Pacman;
        QHash<QString, QGraphicsPixmapItem*> m_Ghosts;

        QStringList m_GhostsNames;
        QList<QPointF> m_DecisionPoints;

        Direction m_PacmanDirection;
        size_t m_PointsCount;
        size_t m_Score;
    };

}
