#pragma once

#include "juceconnection.h"

enum class Direction
{
    Input,
    Output
};

class Socket: public Component
            , public ComponentListener
{
    using ConnectorMovingFunction = void (JuceConnection::*)(const Point<int>&);

public:
    Socket(JuceGraphModel& model, uint32_t index, Direction dir);

    void mouseDrag(const MouseEvent &event) override;
    void mouseDown(const MouseEvent &event) override;
    void mouseUp(const MouseEvent &event) override;
    void mouseEnter(const MouseEvent &event) override;
    void mouseExit(const MouseEvent &event) override;

    void paint(Graphics &g) override;
    void resized() override;

    void componentMovedOrResized(Component &component,
                                 bool wasMoved,
                                 bool wasResized) override;

    void setNodeId(int id) { m_nodeId = id; }

    processGraph::ConnectionPoint getPoint() {return {m_nodeId, m_index};}
    Direction getDirection() {return m_direction;}

private:
    JuceGraphModel& m_parentModel;

    // Node interaction properties
    std::weak_ptr<JuceConnection> m_currentConnection;
    int m_nodeId;
    const uint32_t m_index;
    const Direction m_direction;

    // Geometry and connection moving functions
    ConnectorMovingFunction m_movingFunction;
    Point<int> getCenterInCanvas();
    Point<int> mapToCanvas(const Point<int>& p);

    Socket* getSocketUnderCursor(const Point<int>& p);
    Socket* m_opositeSocket;

    // Animating/interactive stuff
    ComponentAnimator m_animator;
    Colour m_color;
    Rectangle<int> m_noMouseBounds;
    Rectangle<int> m_mouseHoveredBounds;
    void startMouseEnterAnimation();
    void startMouseExitAnimation();


    static void setConnection(Socket& socket,
                              std::shared_ptr<JuceConnection> con,
                              bool isStart);
};
