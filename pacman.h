#ifndef PACMAN_H
#define PACMAN_H

#include <QGraphicsObject>

namespace graphics_item {

    class Pacman : public QGraphicsObject
    {
        Q_OBJECT
    public:
        explicit Pacman(QGraphicsObject *parent = 0);

        QRectF boundingRect() const;
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    private:
        static constexpr float WIDTH  = 20;
        static constexpr float HEIGHT = 20;
        static constexpr float MARGIN = 2;
    };

}

#endif // PACMAN_H
