#include "uiaudionode.h"
#include "../../graph/processgraph.h"

#include <iostream>

Connector::Connector(NodeModel& model, int index)
    : m_parentModel(model)
    , m_index(index)
    , m_nodeId(processGraph::INVALID_NODE_ID)
{}

void Connector::mouseDrag(const MouseEvent &event)
{
    std::cout << "Drag: " << event.position.getX() << ", "
              << event.position.getY() << std::endl;
}

void Connector::paint(Graphics &g)
{
    g.setColour(Colours::white);
    g.fillAll();

    g.setColour(Colours::grey);
    auto rect = getLocalBounds().withHeight(getWidth());
    g.fillEllipse(rect.reduced(rect.getHeight()/3).toFloat());
}

void Connector::resized()
{

}



//--------------------------------------------------------------------------//





UIAudioNode::UIAudioNode(NodeModel& model, int inputNumber, int outputNumber)
{
    for(int i = 0; i< inputNumber; i++)
    {
        m_inConnectors.emplace_back(std::make_unique<Connector>(model, i));
        addAndMakeVisible(m_inConnectors.back().get());
    }

    for(int i = 0; i< outputNumber; i++)
    {
        m_outConnectors.emplace_back(std::make_unique<Connector>(model, i));
        addAndMakeVisible(m_outConnectors.back().get());
    }

    setSize(125, 125);
}

void UIAudioNode::setNodeID(int id)
{
    auto setIdFunct = [&](std::unique_ptr<Connector>& c){c->setNodeId(id);};
    std::for_each(m_inConnectors.begin(),m_inConnectors.end(), setIdFunct);
    std::for_each(m_outConnectors.begin(),m_outConnectors.end(), setIdFunct);
}

void UIAudioNode::paint(Graphics &g)
{
    g.setColour(Colours::darkgrey);
    g.fillRoundedRectangle(getLocalBounds().toFloat(), 8);


    auto rect = getLocalBounds();
    auto inputSection = rect.removeFromLeft(50);
    auto outSection = rect.removeFromRight(50);

//    g.setColour(Colours::darkblue);
//    g.fillRect(inputSection);
//    g.fillRect(outSection);

//    g.setColour(Colours::lightblue);
//    g.fillRect(rect);
}

void UIAudioNode::resized()
{
    auto rect = getLocalBounds().reduced(4);
    auto inputSection = rect.removeFromLeft(20)
            .withTop(20);
    auto outSection = rect.removeFromRight(20)
            .withTop(20);

    for(auto& input : m_inConnectors )
    {
        input->setBounds(inputSection.removeFromTop(40));
    }

    for(auto& output : m_outConnectors )
    {
        output->setBounds(outSection.removeFromTop(40));
    }
}

void UIAudioNode::mouseDrag(const MouseEvent &event)
{}

