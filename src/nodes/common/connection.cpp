#include "connection.h"

Connection::Connection(NodeModel& model)
    : m_model(model)
    , m_input{processGraph::INVALID_NODE_ID,0}
    , m_output{processGraph::INVALID_NODE_ID,0}
{}

Connection::~Connection()
{
    if(m_input.nodeId != processGraph::INVALID_NODE_ID &&
       m_output.nodeId != processGraph::INVALID_NODE_ID)
    {
        m_model.disconnectNodes(m_output.nodeId, m_output.portNumber,
                             m_input.nodeId, m_input.portNumber);
    }
}

void Connection::paint(Graphics &g)
{
    g.setColour(Colours::azure);
    g.fillAll();
}

void Connection::mouseUp(const MouseEvent &event)
{
    m_model.removeConnection(shared_from_this());
}

void Connection::setStartPoint(Point<int> point)
{
    m_startPoint = point;
}

void Connection::setEndPoint(Point<int> point)
{
    m_endPoint = point;
    moveEnd(m_endPoint);
}

void Connection::moveEnd(Point<int> point)
{
    m_endPoint = point;
    auto x = std::min(m_startPoint.getX(), m_endPoint.getX());
    auto y = std::min(m_startPoint.getY(), m_endPoint.getY());

    auto width = std::abs(m_startPoint.getX() - m_endPoint.getX());
    auto height = std::abs(m_startPoint.getY() - m_endPoint.getY());

    setBounds(x,y,width,height);
}

bool Connection::setConnectionPoint(const processGraph::ConnectionPoint &point, bool isInput)
{
    isInput? m_input = point : m_output = point;

    return connectPoints();
}

bool Connection::connectPoints()
{
    if(m_input.nodeId != processGraph::INVALID_NODE_ID &&
       m_output.nodeId != processGraph::INVALID_NODE_ID)
    {
        return m_model.connectNodes(m_output.nodeId, m_output.portNumber,
                                    m_input.nodeId, m_input.portNumber);
    }

    return true;
}


