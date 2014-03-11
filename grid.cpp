#include "grid.h"
#include "constants.cpp"

namespace graphics_item {

    Grid::Grid(size_t hCellCount, size_t vCellCount, QSizeF cellSize, QGraphicsItem *parent)
        : QGraphicsObject(parent)
        , m_HCellCount(hCellCount)
        , m_VCellCount(vCellCount)
        , m_CellSize(cellSize)
        , m_Height(m_HCellCount * m_CellSize.width())
        , m_Width(m_VCellCount * m_CellSize.height())
        , m_DrowLines(false)
    {
    }

    QRectF Grid::boundingRect() const
    {
        QPointF leftTop(-m_Width / 2 - MARGIN, -m_Height / 2 - MARGIN);
        QSizeF  size(m_Width + MARGIN, m_Height + MARGIN);
        return QRectF(leftTop, size);
    }

    void Grid::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
    {
        Q_UNUSED(option)
        Q_UNUSED(widget)

        drawBorder(painter);
        if (m_DrowLines) drawLines(painter);
    }

    void Grid::drawBorder(QPainter *p)
    {
        p->save();
        p->setBrush(Qt::black);
        p->drawRect(QRectF(-m_Width / 2, -m_Height / 2, m_Width, m_Height));
        p->restore();
    }

    void Grid::drawLines(QPainter *p)
    {
        p->save();
        p->setPen(Qt::blue);

        // horizontal
        QLineF l_h(-m_Width / 2, -m_Height / 2, m_Width / 2, -m_Height / 2);
        float h = m_CellSize.height();
        for(size_t i = 0; i < m_HCellCount-1; ++i, h += m_CellSize.height()) {
            p->drawLine(l_h.x1(), l_h.y1() + h, l_h.x2(), l_h.y2() + h);
        }

        // vertical
        QLineF l_v(-m_Width / 2, -m_Height / 2, -m_Width / 2, m_Height / 2);
        float v = m_CellSize.width();
        for(size_t i = 0; i < m_VCellCount-1; ++i, v += m_CellSize.width()) {
            p->drawLine(l_v.x1() + v, l_v.y1(), l_v.x2() + v, l_v.y2());
        }

        p->restore();
    }

    float Grid::width() const
    {
        return m_Width;
    }

    float Grid::margin() const
    {
        return MARGIN;
    }

    float Grid::height() const
    {
        return m_Height;
    }

} // namespace graphics_item
