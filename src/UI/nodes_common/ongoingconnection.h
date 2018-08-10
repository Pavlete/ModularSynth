#pragma once

#include <JuceHeader.h>

class OngoingConnection
        : public DrawablePath
{
public:
    OngoingConnection();

    void setStart(const Point<int>& start);
    void setEnd(const Point<int>& end);

private:
    Point<int> m_start, m_end;
    Path m_connectionPath;
    PathStrokeType m_pathProperties;

    void updateGeometry();
    std::pair<Point<float>, Point<float> >
    getPathControl(const Point<float>& pathStart,
                                   const Point<float>& pathEnd);
};


