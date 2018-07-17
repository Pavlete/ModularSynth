#include "juceconnection.h"

#include "audiobufferwrapper.h"
#include "../../graph/processgraph.h"

JuceConnection::JuceConnection(Connection model)
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
