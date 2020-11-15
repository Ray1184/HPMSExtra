/*!
 * File RawDataParser.cpp
 */

#include <tools/RawDataParser.h>
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

    MaterialDefData matData;
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

void hpms::RawDataParser::ProcessRawData(hpms::RawData& data, const hpms::HeaderData& hData, const hpms::FxInstanceData& fxData, const hpms::MaterialDefData& matData, const hpms::GeometryData& geomData, const hpms::SkinningData& sknData, const hpms::VisualData& visualData, const hpms::AnimFramesData& animData)
{
    // 0. Process visual data.
    for (auto& entry : geomData.indicesByMeshId)
    {
        // 1. Process geometry.
        std::string meshId = entry.first;
        std::vector<glm::vec3> vertices;
        std::vector<glm::vec3> normals;
        std::vector<glm::vec2> textCoords;
        std::vector<std::tuple<int, int, int>> rebasedIndices;
        std::map<std::pair<std::string, int>, int> rawByRebasedAndMeshId;
        int count = 0;
        std::unordered_map<std::string, BoneData> boneDataByBoneSid;
        for (auto& index : entry.second)
        {
            vertices.push_back(geomData.positions[std::get<0>(index)]);
            vertices.push_back(geomData.positions[std::get<1>(index)]);
            vertices.push_back(geomData.positions[std::get<2>(index)]);

            normals.push_back(geomData.normals[std::get<0>(index)]);
            normals.push_back(geomData.normals[std::get<1>(index)]);
            normals.push_back(geomData.normals[std::get<2>(index)]);

            if (!geomData.uvCoords.empty())
            {
                textCoords.push_back(geomData.uvCoords[std::get<0>(index)]);
                textCoords.push_back(geomData.uvCoords[std::get<1>(index)]);
                textCoords.push_back(geomData.uvCoords[std::get<2>(index)]);
            }

            int i1 = count++;
            int i2 = count++;
            int i3 = count++;

            rawByRebasedAndMeshId.insert({std::make_pair(meshId, i1), std::get<0>(index)});
            rawByRebasedAndMeshId.insert({std::make_pair(meshId, i2), std::get<1>(index)});
            rawByRebasedAndMeshId.insert({std::make_pair(meshId, i3), std::get<2>(index)});

            rebasedIndices.emplace_back(i1, i2, i3);

            // 2. Process skinning data.
            std::vector<std::tuple<std::string, glm::mat4x4, float>> skin1 = sknData.weightAndTransfMatrixAndBoneSidInfluencesByVertexId.at(std::get<0>(index));
            std::vector<std::tuple<std::string, glm::mat4x4, float>> skin2 = sknData.weightAndTransfMatrixAndBoneSidInfluencesByVertexId.at(std::get<1>(index));
            std::vector<std::tuple<std::string, glm::mat4x4, float>> skin3 = sknData.weightAndTransfMatrixAndBoneSidInfluencesByVertexId.at(std::get<2>(index));


            RebaseSkinning(skin1, boneDataByBoneSid, i1);
            RebaseSkinning(skin2, boneDataByBoneSid, i2);
            RebaseSkinning(skin3, boneDataByBoneSid, i3);


        }
        // 3. Process materials.
        std::string matId = visualData.matIdByMeshId.at(meshId);
        auto matInstAndName = matData.instanceAndNameIdByMatId.at(matId);
        std::string matInst = matInstAndName.first;
        std::string matName = matInstAndName.second;
        glm::vec4 color = fxData.effectByInstanceId.at(matInst);

        MaterialData materialData;
        materialData.name = matName;
        materialData.color = color;
        materialData.textured = !textCoords.empty();


        MeshData meshData;
        meshData.boneDataByBoneSid = boneDataByBoneSid;
        meshData.vertices = vertices;
        meshData.normals = normals;
        meshData.name = meshId;
        meshData.uvMaps = textCoords;
        meshData.indices = rebasedIndices;
        meshData.materialData = materialData;
        data.meshData.push_back(meshData);


    }

    // 4. Process animations.
    std::unordered_map<std::string, std::string> boneSidByBoneId;
    std::string boneId = visualData.boneId;
    std::string rootBoneSid = visualData.boneSid;
    std::string boneName = visualData.boneName;
    boneSidByBoneId.insert({boneId, rootBoneSid});
    SkelData skData;
    skData.boneSid = "root";
    skData.boneName = boneName;
    RecursiveBoneTreeProcess(boneSidByBoneId, visualData.children, skData);
    data.animData.skelData = skData;
    for (auto& entry : animData.transfMatricesByBoneId)
    {
        std::string sBoneSid = boneSidByBoneId[entry.first];
        if (sBoneSid == rootBoneSid) {
            sBoneSid = "root";
        }
        if (data.animData.tranformByBoneSid.find(sBoneSid) == data.animData.tranformByBoneSid.end())
        {
            std::vector<glm::mat4x4> vec;
            data.animData.tranformByBoneSid.insert({sBoneSid, vec});
        }
        for (auto& matrix : entry.second)
        {
            data.animData.tranformByBoneSid[sBoneSid].push_back(matrix);
        }

    }

}



void hpms::RawDataParser::RebaseSkinning(std::vector<std::tuple<std::string, glm::mat4x4, float>>& vertexSkinning, std::unordered_map<std::string, BoneData>& data, int i)
{
    for (auto& skinningInfluence : vertexSkinning)
    {
        std::string boneSid = std::get<0>(skinningInfluence);
        glm::mat4x4 transform = std::get<1>(skinningInfluence);
        float weight = std::get<2>(skinningInfluence);

        if (data.find(boneSid) == data.end())
        {
            BoneData boneData = {};
            data.insert({boneSid, boneData});
        }
        BoneData& boneData = data[boneSid];
        boneData.weightAndTransform.first = transform;
        boneData.weightAndTransform.second.emplace_back(i, weight);
    }
}

void hpms::RawDataParser::RecursiveBoneTreeProcess(std::unordered_map<std::string, std::string>& boneSidByBoneId,
                                                   const std::vector<VisualData>& visualDatas, SkelData& skData)
{
    std::vector<SkelData> skelChildren;
    for (const VisualData& vd : visualDatas)
    {
        if (!vd.jointType) {
            continue;
        }
        std::string boneId = vd.boneId;
        std::string boneSid = vd.boneSid;
        boneSidByBoneId.insert({boneId, boneSid});
        SkelData skDataChild;
        skDataChild.boneSid = boneSid;
        skDataChild.boneName = vd.boneName;
        RecursiveBoneTreeProcess(boneSidByBoneId, vd.children, skDataChild);
        skelChildren.push_back(skDataChild);
    }
    skData.children = skelChildren;

}
