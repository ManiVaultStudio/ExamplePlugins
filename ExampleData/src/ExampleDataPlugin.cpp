#include "ExampleDataPlugin.h"
#include "Application.h"

#include <QtCore>

#include <set>

Q_PLUGIN_METADATA(IID "nl.BioVault.ExampleDataPlugin")

using namespace mv;

ExampleDataPlugin::~ExampleDataPlugin(void)
{
    
}

void ExampleDataPlugin::init()
{
}

/**
 * Create a new dataset linking back to the original raw data
 */
Dataset<DatasetImpl> ExampleDataPlugin::createDataSet(const QString& guid /*= ""*/) const
{
    return new PixelSet(_core, getName(), guid);
}

std::vector<QColor>& ExampleDataPlugin::getData()
{
    return _data;
}

/**
 * Convenience function for setting data in the plugin from an image
 */
void ExampleDataPlugin::setData(QImage image) {
    for (int y = 0; y < image.height(); y++) {
        for (int x = 0; x < image.width(); x++) {
            QRgb rgb = image.pixel(x, y);
            QColor color = QColor::fromRgb(rgb);
            _data.push_back(color);
        }
    }
}

PixelSet::PixelSet(CoreInterface* core, QString dataName, const QString& guid /*= ""*/) :
    DatasetImpl(core, dataName, guid)
{
}

PixelSet::~PixelSet()
{
}

Dataset<DatasetImpl> PixelSet::createSubsetFromSelection(const QString& guiName, const Dataset<DatasetImpl>& parentDataSet /*= Dataset<DatasetImpl>()*/, const bool& visible /*= true*/) const
{
    return _core->createSubsetFromSelection(getSelection<PixelSet>(), const_cast<PixelSet*>(this), guiName, parentDataSet, visible);
}

Dataset<DatasetImpl> PixelSet::copy() const
{
    auto copySet = new PixelSet(_core, getRawDataName());

    copySet->_indices = _indices;

    return copySet;
}

QIcon PixelSet::getIcon(const QColor& color /*= Qt::black*/) const
{
    return QIcon();
}

std::vector<std::uint32_t>& PixelSet::getSelectionIndices()
{
    return _indices;
}

void PixelSet::setSelectionIndices(const std::vector<std::uint32_t>& indices)
{
    _indices = indices;
}

bool PixelSet::canSelect() const
{
    return getRawData<ExampleDataPlugin>().getData().size() >= 1;
}

bool PixelSet::canSelectAll() const
{
    return canSelect() && (getSelectionSize() < getRawData<ExampleDataPlugin>().getData().size());
}

bool PixelSet::canSelectNone() const
{
    return canSelect() && (getSelectionSize() >= 1);
}

bool PixelSet::canSelectInvert() const
{
    return canSelect();
}

void PixelSet::selectAll()
{
    // Get reference to selection indices
    auto& selectionIndices = getSelectionIndices();

    // Clear and resize
    selectionIndices.clear();
    selectionIndices.resize(getRawData<ExampleDataPlugin>().getData().size());

    // Generate cluster selection indices
    std::iota(selectionIndices.begin(), selectionIndices.end(), 0);

    // Notify others that the selection changed
    events().notifyDatasetDataSelectionChanged(this);
}

void PixelSet::selectNone()
{
    // Clear selection indices
    getSelectionIndices().clear();

    // Notify others that the selection changed
    events().notifyDatasetDataSelectionChanged(this);
}

void PixelSet::selectInvert()
{
    // Get reference to selection indices
    auto& selectionIndices = getSelectionIndices();

    // Create set of selected indices
    std::set<std::uint32_t> selectionSet(selectionIndices.begin(), selectionIndices.end());

    // Get number of items
    const auto numberOfItems = getRawData<ExampleDataPlugin>().getData().size();

    // Clear and resize
    selectionIndices.clear();
    selectionIndices.reserve(numberOfItems - selectionSet.size());

    // Do the inversion
    for (std::uint32_t i = 0; i < numberOfItems; i++) {
        if (selectionSet.find(i) == selectionSet.end())
            selectionIndices.push_back(i);
    }

    // Notify others that the selection changed
    events().notifyDatasetDataSelectionChanged(this);
}

QIcon ExampleDataPluginFactory::getIcon(const QColor& color /*= Qt::black*/) const
{
    return Application::getIconFont("FontAwesome").getIcon("database", color);
}

plugin::RawData* ExampleDataPluginFactory::produce()
{
    return new ExampleDataPlugin(this);
}
