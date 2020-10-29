/*!
 * File DaeUtils.h
 */

#pragma once

#include <string>
#include <vector>
#include <tuple>
#include <common/Utils.h>
#include <glm/glm.hpp>

#define PROFILE "profile_COMMON"
#define LIGHT_METHOD "blinn"
#define LIGHT_TYPE "emission"

namespace hpms {

    std::vector<std::string> PlainStringToArray(const std::string& stringVal);

    std::vector<float> ToFloatArray(const std::vector<std::string>& stringArr);

    std::vector<int> ToIntArray(const std::vector<std::string>& stringArr);

    std::vector<std::pair<int, int>> ToPairArray(const std::vector<int>& array);

    std::vector<std::tuple<int, int, int>> ToTripletArray(const std::vector<int>& array);

    std::vector<glm::vec2> ToVec2Array(const std::vector<float>& array);

    std::vector<glm::vec3> ToVec3Array(const std::vector<float>& array);

    std::vector<glm::vec4> ToVec4Array(const std::vector<float>& array);

    std::vector<glm::mat4x4> ToMat4x4Array(const std::vector<float>& array);
}