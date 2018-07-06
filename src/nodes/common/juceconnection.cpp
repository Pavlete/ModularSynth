#include "juceconnection.h"

#include "audiobufferwrapper.h"
#include "../../graph/processgraph.h"

JuceConnection::JuceConnection(JuceGraphModel& model)
    : m_model(model)
    , m_input {processGraph::INVALID_NODE_ID, 0}
    , m_output {processGraph::INVALID_NODE_ID, 0}
    , m_isConnecting(false)
    , m_pathHovered(false)
    , m_pathProperties (5, PathStrokeType::curved, PathStrokeType::rounded)
{
    setStrokeType(m_pathProperties);
}

JuceConnection::~JuceConnection()
{
    m_model.disconnectNodes(m_output, m_input);
}

void JuceConnection::mouseUp(const MouseEvent &)
{
    m_model.removeConnection(shared_from_this());
}

void JuceConnection::mouseEnter(const MouseEvent &)
{
    replaceColour(Colours::black, Colours::red);
    repaint();
}

void JuceConnection::mouseExit(const MouseEvent &)
{
    replaceColour(Colours::red, Colours::black);
    repaint();
}

void JuceConnection::setStartPoint(const Point<int>& point)
{
    m_startPoint = point;
    updateGeometry();
}

void JuceConnection::setEndPoint(const Point<int>& point)
{    
    m_endPoint = point;
    updateGeometry();
}

void JuceConnection::updateGeometry()
{    
    auto x = std::min(m_startPoint.getX(), m_endPoint.getX());
    auto y = std::min(m_startPoint.getY(), m_endPoint.getY());

    auto width = std::abs(m_startPoint.getX() - m_endPoint.getX());
    auto height = std::abs(m_startPoint.getY() - m_endPoint.getY());

    setBounds(x,y - 10,width,height + 20);

    m_connectionPath.clear();

    auto start = m_startPoint.toFloat();
    auto end = m_endPoint.toFloat();
    auto control = getPathControl(start, end);

    m_connectionPath.startNewSubPath (start);
    m_connectionPath.cubicTo(control.first,
                             control.second,
                             end);

   setPath(m_connectionPath);
}

bool JuceConnection::setConnectionPoint(const processGraph::ConnectionPoint &point, bool isInput)
{
    auto& p = isInput? m_input : m_output;

    if(p.nodeId == processGraph::INVALID_NODE_ID)
    {
        p = point;
        return connectPoints();
    }
    return false;
}

bool JuceConnection::isEstablished()
{
    return m_isConnecting;
}

bool JuceConnection::connectPoints()
{
    if(m_input.nodeId != processGraph::INVALID_NODE_ID &&
            m_output.nodeId != processGraph::INVALID_NODE_ID)
    {
        setInterceptsMouseClicks(true, true);
        m_isConnecting = m_model.connectNodes(m_output, m_input);
        return m_isConnecting;
    }
    return true;
}

Point<int> JuceConnection::mapLocal(const Point<int>& globalPoint)
{
    return globalPoint - getBounds().getTopLeft();
}

std::pair<Point<float>, Point<float> >
JuceConnection::getPathControl(const Point<float>& pathStart,
                               const Point<float>& pathEnd)
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
