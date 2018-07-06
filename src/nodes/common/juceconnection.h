#pragma once

#include <JuceHeader.h>

#include "audiographmodel.h"

class JuceConnection;
class JuceAudioNode;
class AudioBufferWrapper;

using JuceGraphModel = AudioGraphModel<JuceAudioNode, JuceConnection, AudioBufferWrapper>;

class JuceConnection
        : public std::enable_shared_from_this<JuceConnection>
        , public DrawablePath
{
public:
    JuceConnection(JuceGraphModel& model);
    ~JuceConnection();

    void mouseUp(const MouseEvent &event) override;
    void mouseEnter(const MouseEvent &event) override;
    void mouseExit(const MouseEvent &event) override;

    void setStartPoint(const Point<int>& point);
    void setEndPoint(const Point<int>& point);
    void moveEnd(const Point<int>& point);

    bool setConnectionPoint(const processGraph::ConnectionPoint& point,
                            bool isInput);

    bool isEstablished();

protected:
    JuceGraphModel& m_model;

private:    
    processGraph::ConnectionPoint m_input;
    processGraph::ConnectionPoint m_output;
    bool m_isConnecting;

    bool m_pathHovered;    
    Point<int> m_startPoint;
    Point<int> m_endPoint;
    Path m_connectionPath;
    PathStrokeType m_pathProperties;

    void updateGeometry();
    bool connectPoints();

    Point<int> mapLocal(const Point<int>& globalPoint);
    static std::pair<Point<float>, Point<float>> getPathControl(const Point<float>& pathStart,
                                                                const Point<float>& pathEnd);
};
