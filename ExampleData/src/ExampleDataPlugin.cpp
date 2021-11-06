#include "ExampleDataPlugin.h"
#include "Application.h"

#include <QtCore>

Q_PLUGIN_METADATA(IID "nl.BioVault.ExampleDataPlugin")

// =============================================================================
// Data
// =============================================================================

ExampleDataPlugin::~ExampleDataPlugin(void)
{
    
}

void ExampleDataPlugin::init()
{
}

/**
 * Create a new dataset linking back to the original raw data
 */
hdps::DataSet* ExampleDataPlugin::createDataSet() const
{
    return new PixelSet(_core, getName());
}

/**
 * Convenience function for setting data in the plugin from an image
 */
void ExampleDataPlugin::setData(QImage image) {
    for (int y = 0; y < image.height(); y++) {
        for (int x = 0; x < image.width(); x++) {
            QRgb rgb = image.pixel(x, y);
            QColor color = QColor::fromRgb(rgb);
            data.push_back(color);
        }
    }
}

PixelSet::PixelSet(hdps::CoreInterface* core, QString dataName) :
    DataSet(core, dataName)
{
}

PixelSet::~PixelSet()
{
}

hdps::DataSet* PixelSet::copy() const
{
    PixelSet* set = new PixelSet(_core, getDataName());
    set->setName(getName());
    set->_indices = _indices;
    return set;
}

QIcon PixelSet::getIcon() const
{
    return QIcon();
}

QIcon ExampleDataPluginFactory::getIcon() const
{
    return hdps::Application::getIconFont("FontAwesome").getIcon("database");
}

hdps::plugin::RawData* ExampleDataPluginFactory::produce()
{
    return new ExampleDataPlugin(this);
}
