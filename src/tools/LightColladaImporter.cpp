/*!
 * File LightColladaImporter.cpp
 */

#include <tools/LightColladaImporter.h>
#include <tools/dae/LibEffectParser.h>
#include <tools/dae/LibMaterialParser.h>
#include <tools/dae/LibGeometryParser.h>
#include <tools/dae/LibVisualSceneParser.h>
#include <tools/dae/LibSkinningParser.h>

hpms::AdvModelItem* hpms::LightColladaImporter::LoadModelItem(const std::string& path, const std::string& textDirs)
{
    ModelRawData data;
    LoadAndProcessModel(path, data);
    return nullptr;
}

void hpms::LightColladaImporter::LoadAndProcessModel(const std::string& path, hpms::ModelRawData& data)
{
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(path.c_str());
    if (!result)
    {
        std::stringstream ss;
        ss << "Impossible to load " << path << ".";
        LOG_ERROR(ss.str().c_str());
    }
    pugi::xml_node colladaNode = doc.child("COLLADA");

    FxInstanceData fxData;
    pugi::xml_node fxNode = colladaNode.child("library_effects");
    hpms::LibEffectsParser::Process(fxData, fxNode);

    MaterialData matData;
    pugi::xml_node matNode = colladaNode.child("library_materials");
    hpms::LibMaterialParser::Process(matData, matNode);

    GeometryData geometryData;
    pugi::xml_node geometryNode = colladaNode.child("library_geometries");
    hpms::LibGeometryParser::Process(geometryData, geometryNode);

    SkinningData skinningData;
    pugi::xml_node skinningDataNode = colladaNode.child("library_controllers");
    hpms::LibSkinningParser::Process(skinningData, skinningDataNode);

    VisualData visualData;
    pugi::xml_node visualNode = colladaNode.child("library_visual_scenes");
    hpms::LibVisualSceneParser::Process(visualData, visualNode);



}
