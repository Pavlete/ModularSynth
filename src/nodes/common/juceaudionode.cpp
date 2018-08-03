#include "juceaudionode.h"


JuceAudioNode::JuceAudioNode(const SharedNode& model,
                             int inputNumber,
                             int outputNumber)
    : m_model(model)
{
    for(int i = 0; i< inputNumber; i++)
    {
        m_inConnectors.push_back(std::make_unique<Socket>(model, i, Socket::Direction::Input, m_ongoing));
        addComponentListener(m_inConnectors.back().get());
        addAndMakeVisible(m_inConnectors.back().get());
    }

    for(int i = 0; i< outputNumber; i++)
    {
        m_outConnectors.push_back(std::make_unique<Socket>(model, i, Socket::Direction::Output, m_ongoing));
        addComponentListener(m_outConnectors.back().get());
        addAndMakeVisible(m_outConnectors.back().get());
    }

    setBounds(m_model->moduleX, m_model->moduleY, getWidth(),getHeight());
}

std::string JuceAudioNode::getConnectorName(int) {return "";}

int JuceAudioNode::getNodeID() const
{
    return m_model->moduleId;
}

void JuceAudioNode::paint(Graphics &g)
{
    g.setColour(Colours::darkgrey);
    g.fillRoundedRectangle(getLocalBounds().toFloat(), 8);
}

void JuceAudioNode::resized()
{
    auto rect = getLocalBounds().reduced(4);
    auto inputSection = rect.removeFromLeft(20)
            .withTop(10);
    auto outSection = rect.removeFromRight(20)
            .withTop(10);

    for(auto& input : m_inConnectors )
    {
        input->setBounds(inputSection.removeFromTop(20));
    }

    for(auto& output : m_outConnectors )
    {
        output->setBounds(outSection.removeFromTop(20));
    }    
    auto contentRect = getLocalBounds().withRight(getWidth() - 24).withLeft(24);
    setContent(contentRect);
}

void JuceAudioNode::moved()
{
    m_model->moduleX = getBounds().getX();
    m_model->moduleY = getBounds().getY();
}

void JuceAudioNode::parentHierarchyChanged()
{
    getParentComponent()->addChildComponent(m_ongoing);
}

void JuceAudioNode::mouseDown(const MouseEvent &event)
{
    m_dragger.startDraggingComponent(this, event);
}

void JuceAudioNode::mouseDrag(const MouseEvent &event)
{
    m_dragger.dragComponent(this, event, nullptr);
}

void JuceAudioNode::setConnection(std::shared_ptr<JuceConnection> con,
                                  Socket::Direction dir)
{
    auto& socket = dir == Socket::Direction::Input? m_inConnectors[con->getInputPort()]:
                                                    m_outConnectors[con->getOutputPort()];
    socket->setConnection(con);
}

