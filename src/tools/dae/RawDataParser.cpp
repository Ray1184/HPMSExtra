/*!
 * File RawDataParser.cpp
 */

#include <tools/dae/RawDataParser.h>
#include <tools/dae/LibHeaderParser.h>
#include <tools/dae/LibEffectParser.h>
#include <tools/dae/LibMaterialParser.h>
#include <tools/dae/LibGeometryParser.h>
#include <tools/dae/LibVisualSceneParser.h>
#include <tools/dae/LibSkinningParser.h>
#include <tools/dae/LibAnimParser.h>
#include <common/Utils.h>

void hpms::RawDataParser::LoadAndProcessModel(const std::string& path, hpms::RawData& rawData)
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

    HeaderData hData;
    pugi::xml_node hNode = colladaNode.child("asset");
    hpms::LibHeaderParser::Process(hData, hNode);

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

    AnimFramesData animData;
    pugi::xml_node animNode = colladaNode.child("library_animations");
    hpms::LibAnimParser::Process(animData, animNode);

    ProcessRawData(rawData, hData, fxData, matData, geometryData, skinningData, visualData, animData);
}

void hpms::RawDataParser::ProcessRawData(hpms::RawData& data, const hpms::HeaderData& hData, const hpms::FxInstanceData& fxData, const hpms::MaterialData& matData, const hpms::GeometryData& geomData, const hpms::SkinningData& sknData, const hpms::VisualData& visualData, const hpms::AnimFramesData& animData)
{
    // 1. Process Inner Mesh data.
    for (auto& entry : geomData.indicesByMeshId)
    {
        std::string meshId = entry.first;
        std::vector<glm::vec3> vertices;
        std::vector<glm::vec3> normals;
        std::vector<glm::vec2> textCoords;
        std::vector<std::tuple<int, int, int>> rebasedIndices;
//        std::map<int, int> rabasedByRawIndices;
        std::map<std::pair<std::string, int>, int> rawByRebasedAndMeshId;
        int count = 0;
        std::unordered_map<std::string, BoneData> boneDataByBoneId;
        for (auto& index : entry.second)
        {
            vertices.push_back(geomData.positions[std::get<0>(index)]);
            vertices.push_back(geomData.positions[std::get<1>(index)]);
            vertices.push_back(geomData.positions[std::get<2>(index)]);

            normals.push_back(geomData.normals[std::get<0>(index)]);
            normals.push_back(geomData.normals[std::get<1>(index)]);
            normals.push_back(geomData.normals[std::get<2>(index)]);

            textCoords.push_back(geomData.uvCoords[std::get<0>(index)]);
            textCoords.push_back(geomData.uvCoords[std::get<1>(index)]);
            textCoords.push_back(geomData.uvCoords[std::get<2>(index)]);

            int i1 = count++;
            int i2 = count++;
            int i3 = count++;

//            rabasedByRawIndices.insert(std::get<0>(index), i1);
//            rabasedByRawIndices.insert(std::get<1>(index), i2);
//            rabasedByRawIndices.insert(std::get<2>(index), i3);

            rawByRebasedAndMeshId.insert({std::make_pair(meshId, i1), std::get<0>(index)});
            rawByRebasedAndMeshId.insert({std::make_pair(meshId, i2), std::get<1>(index)});
            rawByRebasedAndMeshId.insert({std::make_pair(meshId, i3), std::get<2>(index)});

            rebasedIndices.emplace_back(i1, i2, i3);

            std::vector<std::tuple<std::string, glm::mat4x4, float>> skin1 = sknData.weightAndTransfMatrixAndBoneIdInfluencesByVertexId.at(std::get<0>(index));
            std::vector<std::tuple<std::string, glm::mat4x4, float>> skin2 = sknData.weightAndTransfMatrixAndBoneIdInfluencesByVertexId.at(std::get<1>(index));
            std::vector<std::tuple<std::string, glm::mat4x4, float>> skin3 = sknData.weightAndTransfMatrixAndBoneIdInfluencesByVertexId.at(std::get<2>(index));


            RebaseSkinning(skin1, boneDataByBoneId, i1);
            RebaseSkinning(skin2, boneDataByBoneId, i2);
            RebaseSkinning(skin3, boneDataByBoneId, i3);


        }
        MeshData meshData;
        meshData.vertices = vertices;
        meshData.normals = normals;
        meshData.name = meshId;
        meshData.uvMaps = textCoords;
        meshData.indices = rebasedIndices;
        data.meshData.push_back(meshData);

    }
}

void hpms::RawDataParser::RebaseSkinning(std::vector<std::tuple<std::string, glm::mat4x4, float>>& vertexSkinning, std::unordered_map<std::string, BoneData>& data, int i)
{
    for (auto& skinningInfluence : vertexSkinning)
    {
        std::string boneId = std::get<0>(skinningInfluence);
        glm::mat4x4 transform = std::get<1>(skinningInfluence);
        float weight = std::get<2>(skinningInfluence);

        if (data.find(boneId) == data.end())
        {
            BoneData boneData;
            data.insert({boneId, boneData});
        }
        BoneData& boneData = data[boneId];
        boneData.weightAndTransform.first = transform;
        boneData.weightAndTransform.second.emplace_back(i, weight);
    }
}
