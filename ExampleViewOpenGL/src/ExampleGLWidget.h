#pragma once

#include <renderers/PointRenderer.h>
#include <graphics/Vector2f.h>
#include <graphics/Vector3f.h>
#include <graphics/Bounds.h>

#include <QOpenGLWidget>
#include <QOpenGLFunctions>

#include <QColor>

using namespace hdps;
using namespace hdps::gui;

class ExampleGLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    ExampleGLWidget();

    /** Returns true when the widget was initialized and is ready to be used. */
    bool isInitialized();

    void setData(const std::vector<hdps::Vector2f>& points, float pointSize, float pointOpacity);

protected:
    void initializeGL()         Q_DECL_OVERRIDE;
    void resizeGL(int w, int h) Q_DECL_OVERRIDE;
    void paintGL()              Q_DECL_OVERRIDE;
    void cleanup();
    
signals:
    void initialized();

private:
    bool                    _isInitialized;
    QColor                  _backgroundColor;
    PointRenderer           _pointRenderer;
    float                   _pixelRatio;
    std::vector<Vector2f>   _points;
    std::vector<Vector3f>   _colors;
    Bounds                  _bounds;
};
