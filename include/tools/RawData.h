/*!
 * File RawData.h
 */

#pragma once

#include <vector>
#include <map>

namespace hpms
{
    struct SkelData
    {
        std::string boneSid;
        std::string boneName;
        std::vector<SkelData> children;
    };
    struct AnimData
    {
        struct BoneSidComparator
        {
            bool operator()(const std::string& a, const std::string& b) const
            {
                try {
                size_t lastIndexA = a.find_last_not_of("0123456789");
                size_t lastIndexB = b.find_last_not_of("0123456789");
                int rA = std::stoi(a.substr(lastIndexA + 1));
                int rB = std::stoi(b.substr(lastIndexB + 1));
                return rA - rB;
                } catch (const std::exception& except) {
                    return false;
                }
            }
        };

        std::map<std::string, std::vector<glm::mat4x4>/*, BoneSidComparator*/> tranformByBoneSid;
        SkelData skelData;
    };

    struct BoneData
    {
        std::pair<glm::mat4x4, std::vector<std::pair<int, float>>> weightAndTransform;
    };
    struct MaterialData
    {
        bool textured;
        std::string name; // Or path if is texture.
        glm::vec4 color;
    };
    struct MeshData
    {
        std::string name;
        std::vector<glm::vec3> vertices;
        std::vector<glm::vec3> normals;
        std::vector<glm::vec2> uvMaps;
        std::vector<std::tuple<int, int, int>> indices;
        std::unordered_map<std::string, BoneData> boneDataByBoneSid;
        MaterialData materialData;
    };


    struct RawData
    {
        std::vector<MeshData> meshData;
        AnimData animData;
    };
}