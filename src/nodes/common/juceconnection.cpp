#include "juceconnection.h"

#include "audiobufferwrapper.h"
#include "../../graph/processgraph.h"

JuceConnection::JuceConnection(const Connection& model)
    : m_connection(model)
{
    setInterceptsMouseClicks(true, true);
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

int JuceConnection::getInputID() const {return m_connection.connectionInID();}

int JuceConnection::getInputPort() const {return m_connection.connectionInPort();}

int JuceConnection::getOutputID() const {return m_connection.connectionOutID();}

int JuceConnection::getOutputPort() const {return m_connection.connectionOutPort();}
