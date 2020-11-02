/*!
 * File LightColladaImporter.cpp
 */

#include <tools/LightColladaImporter.h>


hpms::AdvModelItem* hpms::LightColladaImporter::LoadModelItem(const std::string& path, const std::string& textDirs)
{
    ModelRawData data;
    LoadAndProcessModel(path, data);
    return nullptr;
}

void hpms::LightColladaImporter::LoadAndProcessModel(const std::string& path, hpms::ModelRawData& data)
{


}
