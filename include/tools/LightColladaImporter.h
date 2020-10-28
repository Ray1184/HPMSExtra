/*!
 * File LightColladaImporter.h
 */

#pragma once

#include <core/AdvModelItem.h>
#include <string>

namespace hpms
{

    class LightColladaImporter
    {
    public:
        static AdvModelItem* LoadModelItem(std::string& path, std::string& textDirs);
    }
};



