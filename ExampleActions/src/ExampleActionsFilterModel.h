// SPDX-License-Identifier: LGPL-3.0-or-later 
// A corresponding LICENSE file is located in the root directory of this source tree 
// Copyright (C) 2023 BioVault (Biomedical Visual Analytics Unit LUMC - TU Delft) 

#pragma once

#include <models/SortFilterProxyModel.h>

/**
 * Example actions filter model class
 *
 * Sorting and filtering model for example actions
 *
 * @author Thomas Kroes
 */
class ExampleActionsFilterModel : public mv::SortFilterProxyModel
{
public:

    /** 
     * Construct the filter model with \p parent
     * @param parent Pointer to parent object
    */
    ExampleActionsFilterModel(QObject* parent = nullptr);
};
