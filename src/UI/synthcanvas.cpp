#include <memory>

#include "synthcanvas.h"

#include "../data_models/nodefactory.h"

SynthCanvas::SynthCanvas(SynthModel& synth)
    : m_menu (synth)
{
    synth.addListener(this);
    addChildComponent(m_menu);

    auto a = NodeCatalog::getCategories();

    for(auto element : NodeCatalog::getCategories())
    {
        if(element.first.isEmpty())
        {
            continue;
        }
        m_menu.addElement(element.second, element.first);
    }
}

void SynthCanvas::paint(Graphics &g)
{
    g.fillAll(Colours::aliceblue);
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

void SynthCanvas::nodeAdded(const SharedNode& mod)
{
    m_nodes.push_back(mod->getUIFactory()());
    auto newNode = m_nodes.back().get();
    addAndMakeVisible(newNode);
}

void SynthCanvas::connectionAdded(const Connection& con)
{    
    auto newConnection = std::make_shared<JuceConnection>(con);
    m_connections.push_back(newConnection);

    addAndMakeVisible(newConnection.get());
    newConnection->setAlwaysOnTop(true);

    auto outNode = findNodeByID(con.outputNode());
    auto inNode = findNodeByID(con.inputNode());

    if(outNode != m_nodes.end() && inNode != m_nodes.end())
    {
        (*outNode)->setConnection(newConnection, Socket::Direction::Output);
        (*inNode)->setConnection(newConnection, Socket::Direction::Input);
    }
}

void SynthCanvas::connectionRemoved(const Connection& connection)
{
    auto conn = findConnection(connection.outputNode(),
                               connection.outputPort());
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
