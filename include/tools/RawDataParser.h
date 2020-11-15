/*!
 * File RawDataParser.h
 */

#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <glm/glm.hpp>
#include <pugixml.hpp>
#include <tools/dae/LibHeaderParser.h>
#include <tools/dae/LibEffectParser.h>
#include <tools/dae/LibGeometryParser.h>
#include <tools/dae/LibSkinningParser.h>
#include <tools/dae/LibVisualSceneParser.h>
#include <tools/dae/LibAnimParser.h>
#include <tools/dae/LibMaterialParser.h>
#include <tools/RawData.h>

namespace hpms
{


    class RawDataParser
    {


    public:
        static void LoadAndProcessModel(const std::string& path, RawData& rawData);

    private:

        static void ProcessRawData(RawData& data, const HeaderData& hData, const FxInstanceData& fxData, const MaterialDefData& matData, const GeometryData& geomData, const SkinningData& sknData, const VisualData& visualData, const hpms::AnimFramesData& animData);

        static void RebaseSkinning(std::vector<std::tuple<std::string, glm::mat4x4, float>>& vertexSkinning, std::unordered_map<std::string, BoneData>& data, int i);

        static void
        RecursiveBoneTreeProcess(std::unordered_map<std::string, std::string>& boneSidByBoneId,
                                 const std::vector<VisualData>& visualDatas, SkelData& skData);
    };
}