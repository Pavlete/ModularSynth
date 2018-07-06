#include "socket.h"


Socket::Socket(JuceGraphModel& model, uint32_t index, Direction dir)
    : m_parentModel(model)
    , m_nodeId(processGraph::INVALID_NODE_ID)
    , m_index(index)
    , m_direction(dir)
    , m_opositeSocket(nullptr)
    , m_color(Colours::grey)
{}

void Socket::mouseDrag(const MouseEvent &event)
{
    auto con = m_currentConnection.lock();

    if(!con || con->isEstablished())
    {
        return;
    }

    con->setEndPoint(mapToCanvas(event.getPosition()));
    auto connector = getSocketUnderCursor(event.getPosition());

    if(connector &&
       !m_opositeSocket)
    {
        m_opositeSocket = connector;
        m_opositeSocket->startMouseEnterAnimation();
    }
    else if(!connector &&
            m_opositeSocket)
    {
        m_opositeSocket->startMouseExitAnimation();
        m_opositeSocket = nullptr;
    }

}

void Socket::mouseDown(const MouseEvent &event)
{
    if(!m_currentConnection.expired())
    {
        return;
    }

    m_currentConnection =
            m_parentModel.addConnection(std::make_shared<JuceConnection>(m_parentModel));
    auto con = m_currentConnection.lock();

    con->setConnectionPoint(getPoint(),
                            getDirection() == Direction::Input);
    setConnection(*this, con, true);
}

void Socket::mouseUp(const MouseEvent& event)
{
    auto connection = m_currentConnection.lock();

    if(!connection || connection->isEstablished())
    {
        return;
    }

    auto connector = getSocketUnderCursor(event.getPosition());

    if(connector && connector == m_opositeSocket &&
       connection->setConnectionPoint(m_opositeSocket->getPoint(),
                                      m_opositeSocket->getDirection() == Direction::Input))
    {
        setConnection(*m_opositeSocket, connection, false);
        m_opositeSocket->startMouseExitAnimation();
    }
    else
    {
        m_parentModel.removeConnection(connection);
    }
}

void Socket::mouseEnter(const MouseEvent&)
{
    if(m_currentConnection.expired())
    {
        startMouseEnterAnimation();
    }
}

void Socket::mouseExit(const MouseEvent&)
{
    startMouseExitAnimation();
}

void Socket::paint(Graphics &g)
{
    g.setColour(m_color);
    auto rect = getLocalBounds();
    g.fillEllipse(rect.toFloat().reduced(3.0f));
}

void Socket::resized()
{
    if(!m_animator.isAnimating(this))
    {
        m_noMouseBounds = getBounds();
        m_mouseHoveredBounds = getBounds().reduced(-2);
    }
}

void Socket::componentMovedOrResized(Component&, bool wasMoved, bool)
{
    auto con = m_currentConnection.lock();
    if(wasMoved && con)
    {
        (con.get()->*m_movingFunction)(getCenterInCanvas());
    }
}

Point<int> Socket::getCenterInCanvas()
{
    return mapToCanvas(getLocalBounds().getCentre());
}

Point<int> Socket::mapToCanvas(const Point<int>& p)
{
    return p + getParentComponent()->getPosition() + getPosition();
}

Socket* Socket::getSocketUnderCursor(const Point<int> &p)
{
    auto canvas = getParentComponent()->getParentComponent();
    auto ptr = dynamic_cast<Socket*>(canvas->getComponentAt(mapToCanvas(p)));
    if(!ptr)
    {
        ptr = dynamic_cast<Socket*>(canvas->getComponentAt(mapToCanvas(p + Point<int>(-1,0))));
    }
    return ptr == this? nullptr : ptr;
}

void Socket::startMouseEnterAnimation()
{
    m_animator.animateComponent(this,
                                m_mouseHoveredBounds,
                                1, 200, false, 0.0, 1.0);
}

void Socket::startMouseExitAnimation()
{
    m_animator.animateComponent(this,
                                m_noMouseBounds,
                                1, 200, false, 1.0, 0.0);
}

void Socket::setConnection(Socket &socket,
                           std::shared_ptr<JuceConnection> con,
                           bool isStart)
{
    socket.m_currentConnection = con;
    con->setEndPoint(socket.getCenterInCanvas());

    if(isStart)
    {
        con->setStartPoint(socket.getCenterInCanvas());
        socket.m_movingFunction = &JuceConnection::setStartPoint;
    }
    else
    {
        socket.m_movingFunction = &JuceConnection::setEndPoint;
    }}
