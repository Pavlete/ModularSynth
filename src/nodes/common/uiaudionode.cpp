#include "connection.h"
#include "uiaudionode.h"
#include "../../graph/processgraph.h"

#include <iostream>

Connector::Connector(NodeModel& model, uint32_t index, Direction dir)
    : m_parentModel(model)
    , m_nodeId(processGraph::INVALID_NODE_ID)
    , m_index(index)
    , m_direction(dir)
{}

void Connector::mouseDrag(const MouseEvent &event)
{
    auto con = m_currentConnection.lock();
    con->moveEnd(mapToCanvas(event.getPosition()));
}

void Connector::mouseDown(const MouseEvent &event)
{
    m_currentConnection = m_parentModel.addConnection();
    auto con = m_currentConnection.lock();

    con->setConnectionPoint(getPoint(),
                            getDirection() == Direction::Input);
    con->setStartPoint(mapToCanvas(this->getLocalBounds().getCentre()));
    con->moveEnd(mapToCanvas(this->getLocalBounds().getCentre()));

    m_mouseDownPoint = mapToCanvas(event.getMouseDownPosition());
}

void Connector::mouseUp(const MouseEvent& event)
{
    auto con = m_currentConnection.lock();

    if(!con)
    {
        return;
    }

    auto canvas = getParentComponent()->getParentComponent();
    auto ptr = dynamic_cast<Connector*>(canvas->getComponentAt(mapToCanvas(event.getPosition())));

    if(nullptr != ptr && this != ptr &&
            con->setConnectionPoint(ptr->getPoint(),
                                    ptr->getDirection() == Direction::Input))
    {

        con->setEndPoint(ptr->getCenterInCanvas());
    }
    else
    {
        m_parentModel.removeConnection(con);
    }
}

void Connector::paint(Graphics &g)
{
    g.setColour(Colours::grey);
    auto rect = getLocalBounds();
    g.fillEllipse(rect.toFloat().reduced(3.0f));
}

Point<int> Connector::getCenterInCanvas()
{
    return mapToCanvas(getLocalBounds().getCentre());
}

Point<int> Connector::mapToCanvas(const Point<int>& p)
{
    return p + getParentComponent()->getPosition() + getPosition();
}



//--------------------------------------------------------------------------//



UIAudioNode::UIAudioNode(NodeModel& model, int inputNumber, int outputNumber)
{
    for(int i = 0; i< inputNumber; i++)
    {
        m_inConnectors.emplace_back(std::make_unique<Connector>(model, i, Direction::Input));
        addAndMakeVisible(m_inConnectors.back().get());
    }

    for(int i = 0; i< outputNumber; i++)
    {
        m_outConnectors.emplace_back(std::make_unique<Connector>(model, i, Direction::Output));
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
    paintContent(g);
}

void UIAudioNode::resized()
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
}

void UIAudioNode::mouseDrag(const MouseEvent &event)
{}

