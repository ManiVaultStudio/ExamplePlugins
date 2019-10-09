#include "ExampleDataPlugin.h"

#include <QtCore>

Q_PLUGIN_METADATA(IID "nl.tudelft.ExampleDataPlugin")

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
hdps::Set* ExampleDataPlugin::createSet() const
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

/**
 * Mandatory override for copying of data sets
 */
hdps::Set* PixelSet::copy() const
{
    PixelSet* set = new PixelSet(_core, getDataName());
    set->setName(getName());
    set->indices = indices;
    return set;
}

// =============================================================================
// Factory DOES NOT NEED TO BE ALTERED
// Merely responsible for generating new plugins when requested
// =============================================================================

RawData* ExampleDataPluginFactory::produce()
{
    return new ExampleDataPlugin();
}
