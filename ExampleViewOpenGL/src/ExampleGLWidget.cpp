#include "ExampleGLWidget.h"

#include <util/Exception.h>

#include <QPainter>

ExampleGLWidget::ExampleGLWidget() : 
    _isInitialized(false),
    _backgroundColor(0.5f, 0.5f, 0.5f),
    _pointRenderer(),
    _pixelRatio(1.0f),
    _points(),
    _bounds(),
    _colors()
{
    setAcceptDrops(true);
}

bool ExampleGLWidget::isInitialized()
{
    return _isInitialized;
}

void ExampleGLWidget::setData(const std::vector<float>& points)
{
    assert(points.size() % 2 == 0);
    auto numPoints = points.size() / 2;

    _points.clear();
    _points.reserve(numPoints);

    _colors.clear();
    _colors.reserve(numPoints);

    for(unsigned long i = 0; i < numPoints; i++)
    {
        _points.emplace_back(Vector2f{points[2*i], points[2*i+1]});
        _colors.emplace_back(Vector3f{0.f, 0.f, 1.f});
    }

    _bounds = Bounds::Max;

    for (const Vector2f& point : _points)
    {
        _bounds.setLeft(std::min(point.x, _bounds.getLeft()));
        _bounds.setRight(std::max(point.x, _bounds.getRight()));
        _bounds.setBottom(std::min(point.y, _bounds.getBottom()));
        _bounds.setTop(std::max(point.y, _bounds.getTop()));
    }

    _bounds.makeSquare();
    _bounds.expand(0.1f);

    _pointRenderer.setData(_points);
    _pointRenderer.setColors(_colors);
    _pointRenderer.setBounds(_bounds);

    update();
}


void ExampleGLWidget::initializeGL()
{
    initializeOpenGLFunctions();

    connect(context(), &QOpenGLContext::aboutToBeDestroyed, this, &ExampleGLWidget::cleanup);

    // Initialize renderers
    _pointRenderer.init();

    // Set a default color map for both renderers
    _pointRenderer.setScalarEffect(None);
    _pointRenderer.setPointSize(1.f);
    _pointRenderer.setAlpha(0.f);
    _pointRenderer.setSelectionOutlineColor(Vector3f(1, 0, 0));

    // OpenGL is initialized
    _isInitialized = true;

    emit initialized();
}

void ExampleGLWidget::resizeGL(int w, int h)
{
    // we need this here as we do not have the screen yet to get the actual devicePixelRatio when the view is created
    _pixelRatio = devicePixelRatio();
    
    // Pixel ration tells us how many pixels map to a point
    // That is needed as macOS calculates in points and we do in pixels
    // On macOS high dpi displays pixel ration is 2
    w *= _pixelRatio;
    h *= _pixelRatio;

    _pointRenderer.resize(QSize(w, h));
}

void ExampleGLWidget::paintGL()
{
    try {
        QPainter painter;

        // Begin mixed OpenGL/native painting
        if (!painter.begin(this))
            throw std::runtime_error("Unable to begin painting");

        // Draw layers with OpenGL
        painter.beginNativePainting();
        {
            // Bind the framebuffer belonging to the widget
            // glBindFramebuffer(GL_FRAMEBUFFER, defaultFramebufferObject());

            // Clear the widget to the background color
            glClearColor(_backgroundColor.redF(), _backgroundColor.greenF(), _backgroundColor.blueF(), _backgroundColor.alphaF());
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // Reset the blending function
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
               
            _pointRenderer.render();                
        }
        painter.endNativePainting();
        
        painter.end();
    }
    catch (std::exception& e)
    {
        hdps::util::exceptionMessageBox("Rendering failed", e);
    }
    catch (...) {
        hdps::util::exceptionMessageBox("Rendering failed");
    }
}

void ExampleGLWidget::cleanup()
{
    qDebug() << "Deleting widget, performing clean up...";
    _isInitialized = false;

    makeCurrent();
    _pointRenderer.destroy();
}
