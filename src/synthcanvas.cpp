#include "synthcanvas.h"

#include "nodes/vco.h"
#include "nodes/outvca.h"

SynthCanvas::SynthCanvas(NodeModel& path)
    : NodeView (path)
    , m_synthPath (path)
    , m_menu (path)
{        
    addChildComponent(m_menu);
    m_menu.addElement<VCO_GUI>("VCO", "Oscillator");

    path.addInitNode(std::make_unique<OutVCA_GUI>(m_synthPath), {600,100},0);
    path.addNode(std::make_unique<VCO_GUI>(m_synthPath), {200,100});
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
