/*!
 * File RawDataParser.h
 */

#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <glm/glm.hpp>
#include <pugixml.hpp>
#include "LibHeaderParser.h"
#include "LibEffectParser.h"
#include "LibGeometryParser.h"
#include "LibSkinningParser.h"
#include "LibVisualSceneParser.h"
#include "LibAnimParser.h"

namespace hpms
{
    struct AnimData
    {
        std::vector<glm::mat4x4> tranformByFrame;
    };
    struct BoneData
    {
        std::pair<glm::mat4x4, std::vector<std::pair<int, float>>> weightAndTransform;
    };
    struct MeshData
    {
        std::string name;
        std::vector<glm::vec3> vertices;
        std::vector<glm::vec3> normals;
        std::vector<glm::vec2> uvMaps;
        std::vector<std::tuple<int, int, int>> indices;
        std::unordered_map<std::string, BoneData> boneDataByBoneId;

    };
    struct MaterialData
    {
        bool texture;
        std::string name; // Or path if is texture.
        glm::vec4 color;
    };

    struct RawData
    {
        std::vector<MeshData> meshData;
        std::vector<AnimData> animData;
        std::vector<MaterialData> materialData;
    };

    class RawDataParser
    {


    public:
        static void LoadAndProcessModel(const std::string& path, RawData& rawData);

        static void ProcessRawData(RawData& data, const HeaderData& hData, const FxInstanceData& fxData, const MaterialData& matData, const GeometryData& geomData, const SkinningData& sknData, const VisualData& visualData, const hpms::AnimFramesData& animData);

        static void RebaseSkinning(std::vector<std::tuple<std::string, glm::mat4x4, float>>& vertexSkinning, std::unordered_map<std::string, BoneData>& data, int i);
    };
}