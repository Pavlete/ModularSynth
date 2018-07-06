#include "juceaudionode.h"

#include "../../graph/processgraph.h"


JuceAudioNode::JuceAudioNode(JuceGraphModel& model,
                             int inputNumber,
                             int outputNumber)
{
    for(int i = 0; i< inputNumber; i++)
    {
        m_inConnectors.emplace_back(std::make_unique<Socket>(model, i, Direction::Input));
        addComponentListener(m_inConnectors.back().get());
        addAndMakeVisible(m_inConnectors.back().get());
    }

    for(int i = 0; i< outputNumber; i++)
    {
        m_outConnectors.emplace_back(std::make_unique<Socket>(model, i, Direction::Output));
        addComponentListener(m_outConnectors.back().get());
        addAndMakeVisible(m_outConnectors.back().get());
    }
}

void JuceAudioNode::setNodeID(int id)
{
    auto setIdFunct = [&](std::unique_ptr<Socket>& c){c->setNodeId(id);};
    std::for_each(m_inConnectors.begin(),m_inConnectors.end(), setIdFunct);
    std::for_each(m_outConnectors.begin(),m_outConnectors.end(), setIdFunct);
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

void JuceAudioNode::mouseDown(const MouseEvent &event)
{
    m_dragger.startDraggingComponent(this, event);
}

void JuceAudioNode::mouseDrag(const MouseEvent &event)
{
     m_dragger.dragComponent(this, event, nullptr);
}

