#include "socket.h"

namespace  {
    static const float non_paired_alpha = 0.3f;
    static const float paired_alpha = 0.7f;
}


Socket::Socket(SharedNode model, uint32_t index, Direction dir, OngoingConnection& ongoing)
    : m_synthModel(model->getParentSynth())
    , m_nodeId(model->ID())
    , m_index(index)
    , m_direction(dir)
    , m_opositeSocket(nullptr)
    , m_color(Colours::grey)
    , m_ongoing(ongoing)
{
    dir == Direction::Output? m_movingFunction = &JuceConnection::setStart:
                              m_movingFunction = &JuceConnection::setEnd;

}

void Socket::mouseDrag(const MouseEvent &event)
{
    auto con = m_currentConnection.lock();

    if(con)
    {
        return;
    }

    m_ongoing.setEnd(mapToCanvas(event.getPosition()));

    auto connector = getSocketUnderCursor(event.getPosition());

    if(connector &&
       connector->m_nodeId != m_nodeId &&
       connector->getDirection() != getDirection() &&
       connector->m_currentConnection.expired() &&
       !m_opositeSocket)
    {
        m_opositeSocket = connector;
        m_ongoing.setAlpha(paired_alpha);
    }
    else if(!connector &&
            m_opositeSocket)
    {
        m_ongoing.setAlpha(non_paired_alpha);
        m_opositeSocket = nullptr;
    }
}

void Socket::mouseDown(const MouseEvent& event)
{
    auto connection = m_currentConnection.lock();
    if(connection)
    {
        return;
    }
    resetOngoingConnection();
    m_ongoing.setEnd(mapToCanvas(event.getPosition()));
}

void Socket::mouseUp(const MouseEvent&)
{
    m_ongoing.setVisible(false);

    if(!m_opositeSocket)
    {
        return;
    }

    getDirection() == Direction::Output?
                m_synthModel.addConnection(m_nodeId, m_index,
                                            m_opositeSocket->m_nodeId, m_opositeSocket->m_index):
                m_synthModel.addConnection(m_opositeSocket->m_nodeId, m_opositeSocket->m_index,
                                            m_nodeId, m_index);
}

void Socket::mouseEnter(const MouseEvent&)
{
    if(m_currentConnection.expired())
    {
       // startMouseEnterAnimation();
    }
}

void Socket::mouseExit(const MouseEvent&)
{
    //startMouseExitAnimation();
}

void Socket::paint(Graphics &g)
{    
    g.setColour(m_color);
    auto rect = getLocalBounds();
    g.fillEllipse(rect.toFloat().reduced(2.0f));
}



void Socket::componentMovedOrResized(Component&, bool wasMoved, bool)
{
    auto con = m_currentConnection.lock();
    if(wasMoved && con)
    {
        (con.get()->*m_movingFunction)(getCenterInCanvas());
    }
}

void Socket::setConnection(std::shared_ptr<JuceConnection> con)
{
    m_currentConnection = con;
    getDirection() == Direction::Output? con->setStart(getCenterInCanvas()):
                                         con->setEnd(getCenterInCanvas());
}

void Socket::removeConnection()
{
    if(auto con = m_currentConnection.lock())
    {
        con->remove();
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
    auto canvas = m_ongoing.getParentComponent();
    auto ptr = dynamic_cast<Socket*>(canvas->getComponentAt(mapToCanvas(p)));
    if(!ptr)
    {
        ptr = dynamic_cast<Socket*>(canvas->getComponentAt(mapToCanvas(p + Point<int>(-1,0))));
    }
    return ptr == this? nullptr : ptr;
}

void Socket::resetOngoingConnection()
{
    m_ongoing.setVisible(true);
    m_ongoing.setAlpha(non_paired_alpha);
    m_ongoing.setAlwaysOnTop(true);
    m_ongoing.setStart(getCenterInCanvas());
}
