#include "gamepacman.h"
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsRectItem>
#include <QGraphicsPixmapItem>
#include <QTimer>
#include "grid.h"
#include "constants.cpp"

namespace game {

    GamePacman::GamePacman(QGraphicsView *view, QGraphicsScene *scene, QObject *parent)
        : QObject(parent)
        , m_View(view)
        , m_Scene(scene)
        , m_PacmanTimer(new QTimer(this))
        , m_GhostTimer(new QTimer(this))
        , m_PacmanDirection(Top)
        , m_PointsCount(0)
        , m_Score(0)
    {
        m_GhostsNames << "blinky" << "pinky" << "inky" << "clyde";
        prepareToStart();
        makeDecisionPoints();
        connect(m_PacmanTimer, &QTimer::timeout, [this](){ pacmanStep(); });
        connect(m_GhostTimer, &QTimer::timeout, [this](){ oneGhostsStep(); });
    }

    void GamePacman::prepareToStart()
    {
        m_Scene->clear();

        m_Grid = new graphics_item::Grid(H_CELL_COUNT, V_CELL_COUNT, QSizeF(CELL_WIDTH, CELL_HEIGHT));
        m_Scene->addItem(m_Grid);

        QPointF p(m_Grid->boundingRect().topLeft());
        p.rx() += m_Grid->margin()-1;
        p.ry() += m_Grid->margin()-1;
        auto border = m_Scene->addPixmap(QPixmap(":/img/img/border.png"));
        border->setPos(p);
        border->setData(0, "border");

        m_Pacman = new QGraphicsPixmapItem(QPixmap(":/img/img/pacman.png")
                                           .scaled(QSize(CELL_WIDTH - m_Grid->margin(),
                                                         CELL_HEIGHT - m_Grid->margin()),
                                                   Qt::IgnoreAspectRatio,
                                                   Qt::SmoothTransformation));
        m_Scene->addItem(m_Pacman);

        p = m_Grid->boundingRect().bottomLeft();
        p.rx() += 10 * CELL_WIDTH + m_Grid->margin();
        p.ry() -= 6  * CELL_HEIGHT - m_Grid->margin() / 2;
        m_Pacman->setPos(p);
        pacmanToTop();

        makeGhosts();
        generatePoints();
    }

    void GamePacman::start()
    {
        m_PacmanTimer->start(PACMAN_UPTIME);
        m_GhostTimer->start(GHOSTS_UPTIME);
    }

    void GamePacman::stop()
    {
        m_PacmanTimer->stop();
        m_GhostTimer->stop();
        emit gameEnded();
    }

    int GamePacman::score() const
    {
        return m_Score;
    }

    int GamePacman::pointCount() const
    {
        return m_PointsCount;
    }

    bool GamePacman::inGame() const
    {
        return m_PacmanTimer->isActive() && m_GhostTimer->isActive();
    }

    void GamePacman::pacmanToTop()
    {
        m_PacmanDirection = Top;
        m_Pacman->setRotation(0);
        m_Pacman->setTransform(QTransform().translate(0,0));
    }

    void GamePacman::pacmanToBottom()
    {
        m_PacmanDirection = Bottom;
        m_Pacman->setRotation(180);
        m_Pacman->setTransform(QTransform().translate(CELL_WIDTH - m_Grid->margin(),CELL_HEIGHT - m_Grid->margin()));
    }

    void GamePacman::pacmanToLeft()
    {
        m_PacmanDirection = Left;
        m_Pacman->setRotation(270);
        m_Pacman->setTransform(QTransform().translate(0,CELL_HEIGHT - m_Grid->margin()));
    }

    void GamePacman::pacmanToRight()
    {
        m_PacmanDirection = Right;
        m_Pacman->setRotation(90);
        m_Pacman->setTransform(QTransform().translate(CELL_WIDTH - m_Grid->margin(),0));
    }

    void GamePacman::oneGhostsStep()
    {
        ghostStep(m_Ghosts["blinky"]);
        ghostStep(m_Ghosts["pinky"]);

        if (m_Score >= 30) ghostStep(m_Ghosts["inky"]);
        if (m_Score >= 59) ghostStep(m_Ghosts["clyde"]);

        if (m_Ghosts["blinky"]->collidesWithItem(m_Pacman) ||
            m_Ghosts["pinky"]->collidesWithItem(m_Pacman)  ||
            m_Ghosts["inky"]->collidesWithItem(m_Pacman)   ||
            m_Ghosts["clyde"]->collidesWithItem(m_Pacman)  ||
            !m_PacmanTimer->isActive()) stop();
    }

    void GamePacman::generatePoints()
    {
        m_PointsCount = 0;
        m_Score = 0;

        // yes, I really lazy for normal grid( mb in future
        // top rect
        QPointF p(m_Grid->boundingRect().bottomLeft());
        p.ry() -= 2 * CELL_HEIGHT;
        p.rx() += 2 * CELL_WIDTH + 2;
        genHelper(p, 19, 9, m_Grid->boundingRect().bottomLeft().x() + 2 * CELL_WIDTH + m_Grid->margin(), -(qreal)CELL_HEIGHT);

        // middle rect
        p = m_Grid->boundingRect().topLeft();
        p.ry() += CELL_HEIGHT + 2;
        p.rx() += 2 * CELL_WIDTH + 2;
        genHelper(p, 19, 7, m_Grid->boundingRect().topLeft().x() + 2 * CELL_WIDTH + 2, CELL_HEIGHT);

        // bottom rect
        p = m_Grid->boundingRect().topLeft();
        p.ry() += CELL_HEIGHT * 6 + 2;
        p.rx() += 5 * CELL_WIDTH + 2;
        genHelper(p, 13, 9, m_Grid->boundingRect().topLeft().x() + 5 * CELL_WIDTH + 2, CELL_HEIGHT);

        emit scoreChanged(m_Score);
        emit pointsCountChanged(m_PointsCount);
    }

    void GamePacman::makeGhosts()
    {
        QGraphicsPixmapItem *item(nullptr);
        for (auto name : m_GhostsNames) {
            item = new QGraphicsPixmapItem(QPixmap(QString(":/img/img/%1.png").arg(name))
                                           .scaled(QSize(CELL_WIDTH - m_Grid->margin(),
                                                         CELL_HEIGHT - m_Grid->margin()),
                                                   Qt::IgnoreAspectRatio,
                                                   Qt::SmoothTransformation));
            item->setData(Type, "ghost");
            item->setData(Name, name);
            m_Ghosts[name] = item;
        }

        QPointF p(m_Grid->boundingRect().topLeft());
        p.ry() += m_Grid->margin() + 1 + 7 * CELL_HEIGHT;
        p.rx() += m_Grid->margin() + 1 + 10 * CELL_WIDTH;
        m_Ghosts["blinky"]->setPos(p);
        m_Scene->addItem(m_Ghosts["blinky"]);
        m_Ghosts["blinky"]->setData(LastPos, movePoint(m_Ghosts["blinky"], Bottom));

        p = m_Ghosts["blinky"]->pos();
        p.ry() += 2 * CELL_HEIGHT;
        m_Ghosts["pinky"]->setPos(p);
        m_Scene->addItem(m_Ghosts["pinky"]);

        m_Ghosts["inky"]->setPos(movePoint(m_Ghosts["pinky"], Left));
        m_Scene->addItem(m_Ghosts["inky"]);

        m_Ghosts["clyde"]->setPos(movePoint(m_Ghosts["pinky"], Right));
        m_Scene->addItem(m_Ghosts["clyde"]);
    }

    void GamePacman::genHelper(QPointF p, size_t h, size_t v, qreal xDef, qreal yAdd)
    {
        for (size_t j = 3; j <= v; ++j) {
            for (size_t i = 3; i <= h; ++i) {
                if (m_Scene->itemAt(p.x() + CELL_WIDTH / 2, p.y() + CELL_HEIGHT / 2, m_View->transform()) == m_Grid) {
                    auto point = m_Scene->addPixmap(QPixmap(":/img/img/point.png"));
                    point->setPos(p);
                    point->setData(0, "point");
                    ++m_PointsCount;
                }
                p.rx() += CELL_WIDTH;
            }
            p.ry() = p.y() + yAdd;
            p.rx() = xDef;
        }
    }

    void GamePacman::pacmanStep()
    {
        if (m_PointsCount == 0) stop();
        QPointF p(m_Pacman->pos());
        switch (m_PacmanDirection) {
        case Top:
            p = movePoint(m_Pacman, Top);
            break;
        case Bottom:
            p = movePoint(m_Pacman, Bottom);
            break;
        case Left:
            p = movePoint(m_Pacman, Left);
            break;
        case Right:
            p = movePoint(m_Pacman, Right);
            break;
        default:
            break;
        }

        fixOutOfGameField(p);
        if (m_Scene->itemAt(p.x() + m_Grid->margin(), p.y() + m_Grid->margin(), m_View->transform())->data(0).toString() != "border") {
            QPointF cellCenter(p.x() + m_Grid->margin() + CELL_WIDTH / 2, p.y() + m_Grid->margin() + CELL_HEIGHT / 2);
            if (m_Scene->itemAt(cellCenter, m_View->transform())->data(0).toString() == "point") {
                auto item = m_Scene->itemAt(cellCenter, m_View->transform());
                m_Scene->removeItem(item);
                delete item;
                --m_PointsCount;
                ++m_Score;
                emit scoreChanged(m_Score);
                emit pointsCountChanged(m_PointsCount);
            }
            m_Pacman->setPos(p);
        }
    }

    void GamePacman::fixOutOfGameField(QPointF &p)
    {
        if (p.x() >= m_Grid->boundingRect().right()) p.rx() -= H_CELL_COUNT * CELL_WIDTH;
        if (p.x() <= m_Grid->boundingRect().left())  p.rx() += H_CELL_COUNT * CELL_WIDTH;
    }

    void GamePacman::ghostStep(QGraphicsPixmapItem *ghost)
    {
        auto index = m_DecisionPoints.indexOf(ghost->pos());
        QPointF p;

        if (index != -1) {
            // near to the Strategy. not refactor
            p = makeDecision(ghost);
        } else {
            p = findPossibleStep(ghost);
        }

        ghost->setData(LastPos, ghost->pos());
        ghost->setPos(p);
    }

    QList<QPointF> GamePacman::findPossibleSteps(QGraphicsPixmapItem *ghost)
    {
        QList<QPointF> result;
        QPointF p;
        for(int i = Left; i <= Bottom; ++i) {
             p = movePoint(ghost, static_cast<Direction>(i));
             if (m_Scene->itemAt(p.x() /*+ m_Grid->margin()*/,
                                 p.y() /*+ m_Grid->margin()*/,
                                 m_View->transform())->data(0).toString() != "border" &&
                 p != ghost->data(LastPos).toPointF()) {
                 fixOutOfGameField(p);
                 result << p;
             }
        }

        return result;
    }

    QPointF GamePacman::makeDecision(QGraphicsPixmapItem *ghost)
    {
        QList<QPointF> posP = std::move(findPossibleSteps(ghost));
        QPointF p = posP[0];
        QPointF resultGhostPos;

        if (ghost->data(Name).toString() == "blinky") {
            resultGhostPos = m_Pacman->pos();
        } else if (ghost->data(Name).toString() == "pinky") {
            resultGhostPos = m_Pacman->pos();

            switch (m_PacmanDirection) {
            case Top:
                resultGhostPos.rx() -= 4 * CELL_WIDTH;
                resultGhostPos.ry() -= 4 * CELL_HEIGHT;
                break;
            case Bottom:
                resultGhostPos.ry() += 4 * CELL_HEIGHT;
                break;
            case Left:
                resultGhostPos.rx() -= 4 * CELL_WIDTH;
                break;
            case Right:
                resultGhostPos.rx() += 4 * CELL_WIDTH;
            }
        } else if (ghost->data(Name).toString() == "inky") {
            resultGhostPos = m_Pacman->pos();

            switch (m_PacmanDirection) {
            case Top:
                resultGhostPos.ry() -= 2 * CELL_HEIGHT;
                break;
            case Bottom:
                resultGhostPos.ry() += 2 * CELL_HEIGHT;
                break;
            case Left:
                resultGhostPos.rx() -= 2 * CELL_WIDTH;
                break;
            case Right:
                resultGhostPos.rx() += 2 * CELL_WIDTH;
            }

            QLineF l(m_Ghosts["blinky"]->pos(), resultGhostPos);
            l.setLength(l.length() * 2);
            resultGhostPos = l.p2();
        } else if (ghost->data(Name).toString() == "clyde") {
            if (QLineF(m_Pacman->pos(), ghost->pos()).length() <= 8 * CELL_HEIGHT) {
                resultGhostPos = m_Pacman->pos();
            } else {
                resultGhostPos = m_Grid->boundingRect().bottomRight();
                resultGhostPos.ry() += CELL_HEIGHT;
            }
        }

        for (auto point : posP)
            if (QLineF(resultGhostPos, p).length() > QLineF(resultGhostPos, point).length()) p = point;
        return p;
    }

    QPointF GamePacman::findPossibleStep(QGraphicsPixmapItem *ghost)
    {
        QPointF p;
        for(int i = Left; i <= Bottom; ++i) {
             p = movePoint(ghost, static_cast<Direction>(i));
             if (m_Scene->itemAt(p.x() + m_Grid->margin(),
                                 p.y() + m_Grid->margin(),
                                 m_View->transform())->data(0).toString() != "border" &&
                 p != ghost->data(LastPos).toPointF()) {
                 fixOutOfGameField(p);
                 return p;
             }
        }
        return ghost->data(LastPos).toPointF();
    }


    void GamePacman::makeDecisionPoints()
    {
        // NOTE: mb refactor this code...
        //       QPointF p = m_Grid->m_Grid->boundingRect().topLeft();
        //       p.rx() += N * CELL_WIDTH;
        //       p.ry() += M * CELL_HEIGHT;
        m_DecisionPoints << QPointF(-109,-189) << QPointF(-169,-149) << QPointF(-109,-149)
                         << QPointF(-69,-149)  << QPointF(-29,-149)  << QPointF(11,-149)
                         << QPointF(51,-149)   << QPointF(91,-149)   << QPointF(151,-149)
                         << QPointF(91,-189)   << QPointF(91,-109)   << QPointF(-109,-109)
                         << QPointF(-109,-29)  << QPointF(-69,-29)   << QPointF(51,-29)
                         << QPointF(91,-29)    << QPointF(51,11)     << QPointF(-69,11)
                         << QPointF(-109,51)   << QPointF(-69,51)    << QPointF(51,51)
                         << QPointF(91,51)     << QPointF(91,91)     << QPointF(51,91)
                         << QPointF(-69,91)    << QPointF(-109,91)   << QPointF(-149,131)
                         << QPointF(131,131)   << QPointF(11,171)    << QPointF(-29,171)
                         << QPointF(-29,91) << QPointF(31,91)
                         << QPointF(-29,-69) << QPointF(11,-69) << QPointF(-9,-69) << QPointF(-9,-29);
    }

    QPointF GamePacman::movePoint(QGraphicsPixmapItem *item, GamePacman::Direction direction)
    {
        QPointF p(item->pos());

        switch (direction) {
        case Top:
            p.ry() -= CELL_HEIGHT;
            break;
        case Bottom:
            p.ry() += CELL_HEIGHT;
            break;
        case Left:
            p.rx() -= CELL_WIDTH;
            break;
        case Right:
            p.rx() += CELL_WIDTH;
        default:
            break;
        }

        return p;
    }
}
