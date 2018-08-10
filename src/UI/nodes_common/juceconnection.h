#pragma once

#include "ongoingconnection.h"

#include "../../data_models/connection.h"

class JuceConnection
        : public OngoingConnection
{
public:
    JuceConnection(const Connection& model);

    void mouseUp(const MouseEvent &event) override;
    void mouseEnter(const MouseEvent &event) override;
    void mouseExit(const MouseEvent &event) override;

    int getInputID() const;
    unsigned int getInputPort() const;
    int getOutputID() const;
    unsigned int getOutputPort() const;
private:
    Connection m_connection;
    bool m_pathHovered;
};
