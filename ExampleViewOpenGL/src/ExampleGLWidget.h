#pragma once

#include <renderers/PointRenderer.h>

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

    /** Get/set background color */
    QColor getBackgroundColor();
    void setBackgroundColor(QColor color);

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
};
