#pragma once

#include <JuceHeader.h>

class OngoingConnection
        : public DrawablePath
{
public:
    OngoingConnection();

    void setStart(const Point<int>& start);
    void setEnd(const Point<int>& end);

protected:
    Path m_connectionPath;

private:
    Point<int> m_start, m_end;
    PathStrokeType m_pathProperties;

    void updateGeometry();
    std::pair<Point<float>, Point<float> >
    getPathControl(const Point<float>& pathStart,
                   const Point<float>& pathEnd);
    void updatePath();
};


