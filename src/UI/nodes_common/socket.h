#pragma once

#include "../../data_models/synthmodel.h"
#include "juceconnection.h"

class Socket: public Component
            , public ComponentListener
{
    using ConnectorMovingFunction = void (JuceConnection::*)(const Point<int>&);

public:
    enum class Direction
    {
        Input,
        Output
    };

    Socket(SharedNode model, uint32_t index, Direction dir, OngoingConnection& ongoing);

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

    void setConnection(std::shared_ptr<JuceConnection> con);

    Direction getDirection() const {return m_direction;}

private:
    SynthModel m_synthModel;

    // Node interaction properties
    std::weak_ptr<JuceConnection> m_currentConnection;
    const int m_nodeId;
    const uint32_t m_index;
    const Direction m_direction;

    // Geometry and connection moving functions
    ConnectorMovingFunction m_movingFunction;
    Point<int> getCenterInCanvas();
    Point<int> mapToCanvas(const Point<int>& p);
    Socket* getSocketUnderCursor(const Point<int>& p);
    Socket* m_opositeSocket;
    void resetOngoingConnection();


    // Animating/interactive stuff
    ComponentAnimator m_animator;
    Colour m_color;
    Rectangle<int> m_noMouseBounds;
    Rectangle<int> m_mouseHoveredBounds;
    void startMouseEnterAnimation();
    void startMouseExitAnimation();

    OngoingConnection& m_ongoing;
};
