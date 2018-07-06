#include <memory>

#include "synthcanvas.h"

#include "nodes/vcf.h"
#include "nodes/vco.h"
#include "nodes/outvca.h"

SynthCanvas::SynthCanvas(JuceGraphModel& path)
    : NodeView (path)
    , m_menu (path)
{        
    addChildComponent(m_menu);
    m_menu.addElement<VCO_GUI>("VCO", "Oscillator");
    m_menu.addElement<VCF_GUI>("VCF", "Filter");

    path.addInitNode(std::make_unique<OutVCA_GUI>(path), 600, 100, 0);
}


void SynthCanvas::paint(Graphics &g)
{
    g.fillAll(Colours::beige);
}

void SynthCanvas::mouseDown(const MouseEvent &event)
{
    if(m_menu.isVisible())
    {
        m_menu.setVisible(false);
    }
    else
    {
        auto rect = m_menu.getBounds();
        rect.setPosition(event.getPosition());
        m_menu.setBounds(rect);
        m_menu.setVisible(true);
    }
}

void SynthCanvas::onNodeAdded(JuceAudioNode* node, int x, int y)
{
    addAndMakeVisible(node);
    node->setBounds(x, y, node->getWidth(), node->getHeight());
}

void SynthCanvas::onConnectionAdded(JuceConnection* conn)
{
    addAndMakeVisible(conn);
}
