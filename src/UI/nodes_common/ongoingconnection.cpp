#include "ongoingconnection.h"


OngoingConnection::OngoingConnection():
    m_pathProperties (5, PathStrokeType::curved, PathStrokeType::rounded)
{
    setStrokeType(m_pathProperties);
}

void OngoingConnection::setStart(const Point<int> &start)
{
    m_start = start;
    updateGeometry();
}

void OngoingConnection::setEnd(const Point<int> &end)
{
    m_end = end;
    updateGeometry();
}

void OngoingConnection::updateGeometry()
{
    auto x = std::min(m_start.getX(), m_end.getX());
    auto y = std::min(m_start.getY(), m_end.getY());

    auto width = std::abs(m_start.getX() - m_end.getX());
    auto height = std::abs(m_start.getY() - m_end.getY());

    setBounds(x, y, width,height );

    m_connectionPath.clear();

    auto start = m_start.toFloat();
    auto end = m_end.toFloat();
    auto control = getPathControl(start, end);

    m_connectionPath.startNewSubPath (start);
    m_connectionPath.cubicTo(control.first,
                             control.second,
                             end);

    setPath(m_connectionPath);
}

std::pair<Point<float>, Point<float> > OngoingConnection::getPathControl(const Point<float> &pathStart, const Point<float> &pathEnd)
{
    auto width = std::abs(pathEnd.getX() - pathStart.getX());

    if(pathStart.getX() < pathEnd.getX())
    {
        return {{pathStart + Point<float>(width/4,0)},
            {pathEnd - Point<float>(width/4,0)}};
    }
    else
    {
        return {{pathStart - Point<float>(width/4,0)},
            {pathEnd + Point<float>(width/4,0)}};
    }
}
