#pragma once

#include "uiaudionode.h"

class Connection : public Component
                 , public std::enable_shared_from_this<Connection>
{
public:
    Connection(NodeModel& model);
    ~Connection();

    void paint(Graphics &g) override;
    void mouseUp(const MouseEvent &event) override;

    void setStartPoint(Point<int> point);
    void setEndPoint(Point<int> point);
    void moveEnd(Point<int> point);

    bool setConnectionPoint(const processGraph::ConnectionPoint& point, bool isInput);
private:
    NodeModel& m_model;

    processGraph::ConnectionPoint m_input;
    processGraph::ConnectionPoint m_output;

    Point<int> m_startPoint;
    Point<int> m_endPoint;

    bool connectPoints();

};
