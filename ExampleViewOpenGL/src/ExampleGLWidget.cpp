#include "ExampleGLWidget.h"

#include <util/Exception.h>

#include <QPainter>

ExampleGLWidget::ExampleGLWidget() : 
    _isInitialized(false),
    _backgroundColor(125, 125, 125, 255),
    _pointRenderer(),
    _pixelRatio(1.0f),
    _points(),
    _colors(),
    _bounds(),
    _sizes(),
    _opacities()
{
    setAcceptDrops(true);

    QSurfaceFormat surfaceFormat;

    surfaceFormat.setRenderableType(QSurfaceFormat::OpenGL);

#if defined(__APPLE__) || defined(__linux__ )
    // Ask for an OpenGL 3.3 Core Context as the default
    surfaceFormat.setVersion(3, 3);
    surfaceFormat.setProfile(QSurfaceFormat::CoreProfile);
    surfaceFormat.setSwapBehavior(QSurfaceFormat::DoubleBuffer);
    //QSurfaceFormat::setDefaultFormat(defaultFormat);
#else
    // Ask for an OpenGL 4.3 Core Context as the default
    surfaceFormat.setVersion(4, 3);
    surfaceFormat.setProfile(QSurfaceFormat::CoreProfile);
    surfaceFormat.setSwapBehavior(QSurfaceFormat::DoubleBuffer);
#endif

#ifdef _DEBUG
    surfaceFormat.setOption(QSurfaceFormat::DebugContext);
#endif

    surfaceFormat.setSamples(16);

    setFormat(surfaceFormat);

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

    _sizes.clear();
    _sizes.reserve(numPoints);

    _opacities.clear();
    _opacities.reserve(numPoints);

    for(unsigned long i = 0; i < numPoints; i++)
    {
        _points.emplace_back(Vector2f{points[2*i], points[2*i+1]});
        _colors.emplace_back(Vector3f{0.f, 0.f, 0.f});
        _sizes.emplace_back(10);
        _opacities.emplace_back(0.5f);
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

    _pointRenderer.setBounds(_bounds);
    _pointRenderer.setData(_points);
    _pointRenderer.setColors(_colors);
    _pointRenderer.setSizeChannelScalars(_sizes);
    _pointRenderer.setPointSize(*std::max_element(_sizes.begin(), _sizes.end()));
    _pointRenderer.setOpacityChannelScalars(_opacities);

    update();
}


void ExampleGLWidget::initializeGL()
{
    initializeOpenGLFunctions();

    connect(context(), &QOpenGLContext::aboutToBeDestroyed, this, &ExampleGLWidget::cleanup);

    // Initialize renderers
    _pointRenderer.init();

    // Set a default color map for both renderers
    _pointRenderer.setScalarEffect(PointEffect::None);
    _pointRenderer.setPointScaling(Absolute);
    _pointRenderer.setPointSize(1.f);
    _pointRenderer.setAlpha(0.5f);
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