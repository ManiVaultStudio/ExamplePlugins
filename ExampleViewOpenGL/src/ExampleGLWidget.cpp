#include "ExampleGLWidget.h"

#include <util/Exception.h>

#include <QPainter>

ExampleGLWidget::ExampleGLWidget() : 
    QOpenGLWidget(),
    _isInitialized(false),
    _backgroundColor(235, 235, 235, 255),
    _pointRenderer(),
    _pixelRatio(1.0f),
    _points(),
    _colors(),
    _bounds()
{
    setAcceptDrops(true);

    QSurfaceFormat surfaceFormat;
    surfaceFormat.setRenderableType(QSurfaceFormat::OpenGL);

    // Ask for an different OpenGL versions depending on OS
#if defined(__APPLE__) 
    surfaceFormat.setVersion(4, 1); // https://support.apple.com/en-us/101525
    surfaceFormat.setProfile(QSurfaceFormat::CoreProfile);
#elif defined(__linux__ )
    surfaceFormat.setVersion(4, 2); // glxinfo | grep "OpenGL version"
    surfaceFormat.setProfile(QSurfaceFormat::CompatibilityProfile);
#else
    surfaceFormat.setVersion(4, 3);
    surfaceFormat.setProfile(QSurfaceFormat::CoreProfile);
#endif

#ifdef _DEBUG
    surfaceFormat.setOption(QSurfaceFormat::DebugContext);
#endif

    surfaceFormat.setSwapBehavior(QSurfaceFormat::DoubleBuffer);
    surfaceFormat.setSamples(16);

    setFormat(surfaceFormat);

}

ExampleGLWidget::~ExampleGLWidget()
{
    cleanup();
}

void ExampleGLWidget::setData(const std::vector<hdps::Vector2f>& points, float pointSize, float pointOpacity)
{
    const auto numPoints = points.size();

    _points = points;

    _colors.clear();
    _colors.reserve(numPoints);

    constexpr hdps::Vector3f pointColor = {0.f, 0.f, 0.f};

    for(unsigned long i = 0; i < numPoints; i++)
        _colors.emplace_back(pointColor);

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

    // Send the data to the renderer
    _pointRenderer.setBounds(_bounds);
    _pointRenderer.setData(_points);
    _pointRenderer.setColors(_colors);

    _pointRenderer.setPointSize(pointSize);
    _pointRenderer.setAlpha(pointOpacity);

    // Calls paintGL()
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
    initializeOpenGLFunctions();
    // Bind the framebuffer belonging to the widget
    glBindFramebuffer(GL_FRAMEBUFFER, defaultFramebufferObject());

    // Clear the widget to the background color
    glClearColor(_backgroundColor.redF(), _backgroundColor.greenF(), _backgroundColor.blueF(), _backgroundColor.alphaF());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Reset the blending function
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    _pointRenderer.render();                
}

void ExampleGLWidget::cleanup()
{
    qDebug() << "Deleting widget, performing clean up...";
    _isInitialized = false;

    makeCurrent();
    _pointRenderer.destroy();
}
