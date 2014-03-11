#include "pacman.h"
#include <QPainter>
#include <QPicture>

namespace graphics_item {

    Pacman::Pacman(QGraphicsObject *parent)
        : QGraphicsObject(parent)
    {
    }

    QRectF Pacman::boundingRect() const
    {
        return QRectF(-WIDTH / 2 - MARGIN, - HEIGHT / 2 - MARGIN, WIDTH + MARGIN, HEIGHT + MARGIN);
    }

    void Pacman::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
    {
        Q_UNUSED(option);
        Q_UNUSED(widget);

        painter->drawPixmap(QPoint(0 + MARGIN,0 + MARGIN), QPixmap(":/img/img/pacman.png").scaled(QSize(WIDTH, HEIGHT),
                                                                                Qt::IgnoreAspectRatio,
                                                                                Qt::SmoothTransformation));
    }

}
