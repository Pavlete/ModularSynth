#include "juceaudionode.h"
#include "style.h"


namespace
{
    static String font = "sans-serif";
}


JuceAudioNode::JuceAudioNode(const SharedNode& model,
                             int inputNumber,
                             int outputNumber)
    : m_model(model)
    , m_removeButton("",
                     colours::remove_module,
                     colours::remove_module.darker(0.3f),
                     colours::remove_module.darker(0.3f))
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

    setBounds(m_model->x(), m_model->y(), getWidth(),getHeight());

    addAndMakeVisible(m_removeButton);
    m_buttonPath.addEllipse(0,0,100,100);
    m_removeButton.addListener(this);
    m_removeButton.setShape(m_buttonPath,false, false, false);
}

std::string JuceAudioNode::getConnectorName(int) {return "";}

int JuceAudioNode::getNodeID() const
{
    return m_model->ID();
}

void JuceAudioNode::paint(Graphics &g)
{
    auto width = getLocalBounds().getWidth();

    g.setColour(colours::module_background);
    g.fillRoundedRectangle(getLocalBounds().toFloat(), measures::module_radius);

    auto topBand = getLocalBounds().removeFromTop(measures::top_band_size);

    g.setColour(colours::font);
    g.setFont(Font(20, 1));
    g.drawText(m_model->name(), topBand + Point<int>(measures::side_margin,0)
               , Justification::verticallyCentred);

    g.fillRect(measures::side_margin,
               measures::top_band_size - measures::separation_bar_width,
               width - 2 * measures::side_margin,
               measures::separation_bar_width);
}

void JuceAudioNode::resized()
{
    auto topBand = getLocalBounds().removeFromTop(measures::top_band_size);

    m_removeButton.setBounds(topBand.removeFromRight(measures::top_band_size)
                                    .reduced((measures::top_band_size - measures::remove_size )/2));

    auto rect = getLocalBounds().withTop(measures::top_band_size);
    rect = rect.reduced(measures::side_margin);

    auto inputSection = rect.removeFromLeft(measures::connectors_width);
    auto outSection = rect.removeFromRight(measures::connectors_width);

    for(auto& input : m_inConnectors )
    {
        input->setBounds(inputSection.removeFromTop(20));
    }

    for(auto& output : m_outConnectors )
    {
        output->setBounds(outSection.removeFromBottom(20));
    }    
    rect.reduce(measures::controls_margin, 0);
    setContent(rect);
}

void JuceAudioNode::moved()
{
    m_model->setX(getBounds().getX());
    m_model->setY(getBounds().getY());
}

void JuceAudioNode::parentHierarchyChanged()
{
    auto parent = getParentComponent();
    if(parent)
    {
        getParentComponent()->addChildComponent(m_ongoing);
    }
}

void JuceAudioNode::buttonClicked(Button *)
{
    for(auto& socket : m_inConnectors)
    {
        socket->removeConnection();
    }
    for(auto& socket : m_outConnectors)
    {
        socket->removeConnection();
    }

    auto& model = m_model->getParentSynth();
    model.removeNode(m_model);
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

