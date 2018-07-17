#pragma once

#include <JuceHeader.h>

#include "audiographmodel.h"
#include "ongoingconnection.h"
#include "../../synthmodel.h"


class JuceConnection;
class JuceAudioNode;
class AudioBufferWrapper;

using JuceGraphModel = AudioGraphModel<JuceAudioNode, JuceConnection, AudioBufferWrapper>;

class JuceConnection
        : public OngoingConnection
{
public:
    JuceConnection(Connection model);

    void mouseUp(const MouseEvent &event) override;
    void mouseEnter(const MouseEvent &event) override;
    void mouseExit(const MouseEvent &event) override;

    int getInputID() const {return m_connection.connectionInID;}
    int getInputPort() const {return m_connection.connectionInPort;}
    int getOutputID() const {return m_connection.connectionOutID;}
    int getOutputPort() const {return m_connection.connectionOutPort;}
private:
    Connection m_connection;
    bool m_pathHovered;
};
