#ifndef GAMEPACMAN_H
#define GAMEPACMAN_H

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
        int pointCount() const;
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

        void pacmanStep();
        void fixOutOfGameField(QPointF &p);
        void ghostStep(QGraphicsPixmapItem *ghost);
        QList<QPointF> findPossibleSteps(QGraphicsPixmapItem *ghost);
        QPointF makeDecision(QGraphicsPixmapItem *ghost);
        QPointF findPossibleStep(QGraphicsPixmapItem *ghost);

        void makeDecisionPoints();

        void oneGhostsStep();
        void generatePoints();
        void makeGhosts();
        void genHelper(QPointF p, size_t h, size_t v, qreal xDef, qreal yAdd);

        QPointF movePoint(QGraphicsPixmapItem *item, Direction direction);

        QGraphicsView  *m_View;
        QGraphicsScene *m_Scene;
        QTimer *m_PacmanTimer;
        QTimer *m_GhostTimer;
        graphics_item::Grid *m_Grid;
        QGraphicsPixmapItem *m_Pacman;
        QStringList m_GhostsNames;
        QList<QPointF> m_DecisionPoints;
        QHash<QString, QGraphicsPixmapItem*> m_Ghosts;
        Direction m_PacmanDirection;

        size_t m_PointsCount;
        size_t m_Score;
    };

}

#endif // GAMEPACMAN_H
