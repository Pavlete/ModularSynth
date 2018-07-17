#include <memory>

#include "synthcanvas.h"

#include "nodes/vcf.h"
#include "nodes/vco.h"
#include "nodes/outvca.h"

SynthCanvas::SynthCanvas(SynthModel synth)
    : m_synth(synth)
    , m_menu (synth)
{
    m_synth.addListener(this);
    addChildComponent(m_menu);
    m_menu.addElement<VCO_GUI>("VCO", "Oscillator");
    m_menu.addElement<VCF_GUI>("VCF", "Filter");
    //path.addInitNode(std::make_unique<OutVCA_GUI>(path), 600, 100, 0);
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

void SynthCanvas::nodeAdded(Node mod)
{
    m_nodes.push_back(std::make_unique<VCF_GUI>(mod));
    auto newNode = m_nodes.back().get();
    addAndMakeVisible(newNode);
}

void SynthCanvas::connectionAdded(Connection con)
{
    m_connections.push_back(std::make_shared<JuceConnection>(con));
    auto newConnection = m_connections.back();
    addAndMakeVisible(newConnection.get());
    newConnection->setAlwaysOnTop(true);

    auto outNode = findNodeByID(con.connectionOutID);
    auto inNode = findNodeByID(con.connectionInID);

    (*outNode)->setConnection(newConnection, Socket::Direction::Output);
    (*inNode)->setConnection(newConnection, Socket::Direction::Input);
}

void SynthCanvas::connectionRemoved(Connection connection)
{
    auto conn = findConnection(connection.connectionOutID,
                               connection.connectionOutPort);
    removeChildComponent(conn->get());
    m_connections.erase(conn);
}

std::vector<std::unique_ptr<JuceAudioNode>>::iterator SynthCanvas::findNodeByID(int id)
{
    auto node = std::find_if(m_nodes.begin(), m_nodes.end(), [id](const auto& node)
    {
        return node->getNodeID() == id;
    });

    return node;
}

std::vector<std::shared_ptr<JuceConnection>>::iterator SynthCanvas::findConnection(int outID, int outPort)
{
    auto conn = std::find_if(m_connections.begin(), m_connections.end(), [&](const auto& conn)
    {
        return conn->getOutputID() == outID &&
               conn->getOutputPort() == outPort;
    });

    return conn;
}
