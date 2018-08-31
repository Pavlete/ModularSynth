#include "juceconnection.h"

#include "../../data_models/synthmodel.h"

JuceConnection::JuceConnection(const Connection& model)
    : m_connection(model)
{
    setInterceptsMouseClicks(true, false);
}

void JuceConnection::mouseUp(const MouseEvent &)
{
    m_connection.getParentSynth().removeConnection(m_connection);
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

int JuceConnection::getInputID() const {return m_connection.inputNode();}

unsigned int JuceConnection::getInputPort() const {return m_connection.inputPort();}

int JuceConnection::getOutputID() const {return m_connection.outputNode();}

unsigned int JuceConnection::getOutputPort() const {return m_connection.outputPort();}

void JuceConnection::remove()
{
    m_connection.getParentSynth().removeConnection(m_connection);
}

bool JuceConnection::hitTest(int x, int y)
{
    Point<float> pathPoint;
    Point<float> hitPoint(x + getBounds().getX(),
                          y + getBounds().getY());
    m_connectionPath.getNearestPoint(hitPoint, pathPoint);

    auto dist = pathPoint.getDistanceFrom(hitPoint);

    return  dist < 10;
}
