#include "ExampleDependenciesPlugin.h"

#include <PointData/PointData.h>

#include <algorithm>
#include <cstdint>
#include <iostream>
#include <numeric>
#include <stdio.h>
#include <vector>

#include <hwy/contrib/sort/order.h>
#include <hwy/contrib/sort/vqsort.h>

#if defined __USE_VCPKG__
#include <blake3.h>

#if !defined(__APPLE__)
#include <faiss/IndexFlat.h>
using idx_t = faiss::idx_t;
#endif
#endif

Q_PLUGIN_METADATA(IID "studio.manivault.ExampleDependenciesPlugin")

using namespace mv;
using namespace mv::plugin;

ExampleDependenciesPlugin::ExampleDependenciesPlugin(const PluginFactory* factory) :
    AnalysisPlugin(factory),
    _settingsAction()
{
}

void ExampleDependenciesPlugin::init()
{
    setOutputDataset(mv::data().createDerivedDataset("Output Data", getInputDataset(), getInputDataset()));

    const auto inputPoints  = getInputDataset<Points>();

    auto outputPoints = getOutputDataset<Points>();

    outputPoints->addAction(_settingsAction);

    std::vector<float> initialData;
    const size_t numDims = inputPoints->getNumDimensions();
    const size_t numPoints = inputPoints->getNumPoints();
    initialData.resize(numDims * numPoints);
    outputPoints->setData(initialData.data(), numPoints, numDims);
    events().notifyDatasetDataChanged(outputPoints);

    // Start the analysis when the user clicks the start analysis push button
    connect(&_settingsAction.getStartAnalysisAction(), &mv::gui::TriggerAction::triggered, this, &ExampleDependenciesPlugin::compute);
}

void ExampleDependenciesPlugin::compute()
{
    std::cout << "ExampleDependenciesPlugin: Starting..." << std::endl;

    auto printData = [](const std::vector<float>& vec, size_t dims, size_t points) {
        for (size_t dim = 0; dim < dims; dim++)
        {
            for (size_t point = 0; point < points; point++)
                std::cout << vec[point + points * dim] << " ";
            std::cout << std::endl;
        }
    };

    // Get input data
    const auto inputPoints  = getInputDataset<Points>();
    const size_t numDims    = inputPoints->getNumDimensions();
    const size_t numPoints  = inputPoints->getNumPoints();

    std::vector<float> data;
    std::vector<unsigned int> dimensionIndices(numDims);
    std::iota(dimensionIndices.begin(), dimensionIndices.end(), 0);
    data.resize(numDims * numPoints);
    inputPoints->populateDataForDimensions(data, dimensionIndices);

    // Sort
    std::cout << "ExampleDependenciesPlugin: Data before sorting" << std::endl;
    printData(data, numDims, numPoints);

    for (size_t dim = 0; dim < numDims; dim++)
    {
        size_t offset = numPoints * dim;
        hwy::VQSort(data.data() + offset, numPoints, hwy::SortAscending{});
    }

    std::cout << "ExampleDependenciesPlugin: Data after sorting" << std::endl;
    printData(data, numDims, numPoints);

    // Save to output
    auto outputPoints = getOutputDataset<Points>();
    outputPoints->setData(data.data(), numPoints, numDims);
    events().notifyDatasetDataChanged(outputPoints);

#if defined __USE_VCPKG__
    std::cout << "ExampleDependenciesPlugin: Using blake version: " << blake3_version() << std::endl;

    // Create hash
    blake3_hasher hasher;
    blake3_hasher_init(&hasher);

    blake3_hasher_update(&hasher, data.data(), data.size());

    uint8_t output[BLAKE3_OUT_LEN];
    blake3_hasher_finalize(&hasher, output, BLAKE3_OUT_LEN);

    // Print the hash as hexadecimal
    for (size_t i = 0; i < BLAKE3_OUT_LEN; i++) {
        printf("%02x", output[i]);
    }
    printf("\n");

#if !defined(__APPLE__)
    // find knn
    faiss::IndexFlatL2 index(numDims); // call constructor
    index.add(numPoints, data.data()); // add vectors to the index

    int k = std::max(static_cast<size_t>(5), numPoints);
    std::vector<idx_t> I(k * numPoints);
    std::vector<float> D(k * numPoints);

    index.search(numPoints, data.data(), k, D.data(), I.data());

    printf("knn results\n");
    for (size_t i = 0; i < numPoints; i++) {
        for (int j = 0; j < k; j++)
            printf("%5zd ", I[i * k + j]);
        printf("\n");
    }
#endif // __APPLE__
#endif // __USE_VCPKG__

    std::cout << "ExampleDependenciesPlugin: Finished." << std::endl;
}

AnalysisPlugin* ExampleDependenciesPluginFactory::produce()
{
    // Return a new instance of the example analysis plugin
    return new ExampleDependenciesPlugin(this);
}

mv::DataTypes ExampleDependenciesPluginFactory::supportedDataTypes() const
{
    DataTypes supportedTypes;

    // This example analysis plugin is compatible with points datasets
    supportedTypes.append(PointType);

    return supportedTypes;
}

mv::gui::PluginTriggerActions ExampleDependenciesPluginFactory::getPluginTriggerActions(const mv::Datasets& datasets) const
{
    PluginTriggerActions pluginTriggerActions;

    const auto getPluginInstance = [this](const Dataset<Points>& dataset) -> ExampleDependenciesPlugin* {
        return dynamic_cast<ExampleDependenciesPlugin*>(plugins().requestPlugin(getKind(), { dataset }));
    };

    const auto numberOfDatasets = datasets.count();

    if (numberOfDatasets >= 1 && PluginFactory::areAllDatasetsOfTheSameType(datasets, PointType)) {
        auto pluginTriggerAction = new PluginTriggerAction(const_cast<ExampleDependenciesPluginFactory*>(this), this, "Example dependencies", "Perform an Example Analysis", getIcon(), [this, getPluginInstance, datasets](PluginTriggerAction& pluginTriggerAction) -> void {
            for (auto& dataset : datasets)
                getPluginInstance(dataset);
            });

        pluginTriggerActions << pluginTriggerAction;
    }

    return pluginTriggerActions;
}
