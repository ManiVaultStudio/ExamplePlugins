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
    ~ExampleGLWidget();
    
    /** Returns true when the widget was initialized and is ready to be used. */
    bool isInitialized() const { return _isInitialized;};

    /** Sets 2D point positions and visual properties in the renderer */
    void setData(const std::vector<hdps::Vector2f>& points, float pointSize, float pointOpacity);

protected:
    // We have to override some QOpenGLWidget functions that handle the actual drawing
    void initializeGL()         override;
    void resizeGL(int w, int h) override;
    void paintGL()              override;
    void cleanup();

signals:
    void initialized();

private:
    PointRenderer           _pointRenderer;     /* ManiVault OpenGL point renderer implementation */
    float                   _pixelRatio;        /* device pixel ratio */
    std::vector<Vector2f>   _points;            /* 2D coordinates of points */
    std::vector<Vector3f>   _colors;            /* Color of points - here we use a constant color for simplicity */
    Bounds                  _bounds;            /* Min and max point coordinates for camera placement */
    QColor                  _backgroundColor;   /* Background color */
    bool                    _isInitialized;     /* Whether OpenGL is initialized */
};
