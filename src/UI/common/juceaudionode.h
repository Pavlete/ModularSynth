#pragma once

#include "socket.h"

class JuceAudioNode: public Component
                   , public ButtonListener
{
public:
    JuceAudioNode(const SharedNode& model,int inputNumber, int outputNumber);

    virtual ~JuceAudioNode() = default;

    virtual void setContent(Rectangle<int> &r) = 0;
    virtual std::string getConnectorName(int );

    int getNodeID() const;

    void mouseDown(const MouseEvent &event) override;
    void mouseDrag(const MouseEvent &event) override;

    void setConnection(std::shared_ptr<JuceConnection> con, Socket::Direction dir);

private:
    void paint(Graphics &g) override;
    void resized() override;
    void moved() override;
    void parentHierarchyChanged() override;

    std::vector<std::unique_ptr<Socket>> m_inConnectors;
    std::vector<std::unique_ptr<Socket>> m_outConnectors;

    SharedNode m_model;

    ComponentDragger m_dragger;
    OngoingConnection m_ongoing;

    ShapeButton m_removeButton;
    Path m_buttonPath;

    void buttonClicked(Button *) override;
};

