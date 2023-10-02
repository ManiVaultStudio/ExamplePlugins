#pragma once

#include <renderers/PointRenderer.h>
#include <graphics/Vector2f.h>
#include <graphics/Vector3f.h>
#include <graphics/Bounds.h>

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>

#include <QColor>

using namespace hdps;
using namespace hdps::gui;

class ExampleGLWidget : public QOpenGLWidget, QOpenGLFunctions_3_3_Core
{
    Q_OBJECT

public:
    ExampleGLWidget();

    /** Returns true when the widget was initialized and is ready to be used. */
    bool isInitialized();

    void setData(const std::vector<float>& points);

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
    std::vector<float>      _sizes;
    std::vector<float>      _opacities;
    Bounds                  _bounds;
};
