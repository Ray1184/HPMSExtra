/*!
 * File LightColladaImporter.h
 */

#pragma once

#include <core/AdvModelItem.h>
#include <string>
#include <pugixml.hpp>

namespace hpms
{



    class LightColladaImporter
    {
    public:
        static AdvModelItem* LoadModelItem(const std::string& path, const std::string& textDirs);

    private:


    };


}


