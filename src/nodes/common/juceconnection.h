#pragma once

#include <JuceHeader.h>

#include "ongoingconnection.h"
#include "synthmodel.h"

class JuceConnection
        : public OngoingConnection
{
public:
    JuceConnection(const Connection& model);

    void mouseUp(const MouseEvent &event) override;
    void mouseEnter(const MouseEvent &event) override;
    void mouseExit(const MouseEvent &event) override;

    int getInputID() const;
    int getInputPort() const;
    int getOutputID() const;
    int getOutputPort() const;
private:
    Connection m_connection;
    bool m_pathHovered;
};
