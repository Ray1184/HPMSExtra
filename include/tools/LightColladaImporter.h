/*!
 * File LightColladaImporter.h
 */

#pragma once

#include <core/AdvModelItem.h>
#include <string>
#include <pugixml.hpp>

namespace hpms
{

    struct ModelRawData
    {

    };

    class LightColladaImporter
    {
    public:
        static AdvModelItem* LoadModelItem(const std::string& path, const std::string& textDirs);

    private:

        static void LoadAndProcessModel(const std::string& path, ModelRawData& data);

    };


}


