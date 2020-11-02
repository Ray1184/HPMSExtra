/*!
 * File LibAnimParser.cpp
 */

#include <tools/dae/LibAnimParser.h>
#include <tools/dae/DaeUtils.h>

void hpms::LibAnimParser::Process(AnimFramesData& anims, pugi::xml_node& node)
{
    for (pugi::xml_node& animNode : node.children("animation"))
    {
        std::string id = animNode.attribute("id").as_string();
        std::string target = animNode.child("channel").attribute("target").as_string();
        std::string token = "/transform";
        std::string boneId = target.replace(target.find(token), token.size(), "");
        for (pugi::xml_node& sourceNode : animNode.children("source"))
        {
            if (sourceNode.attribute("id").as_string() == id + "-transform-output")
            {
                std::string animPlain = sourceNode.child("float_array").child_value();
                std::vector<std::string> matArray = hpms::PlainStringToArray(animPlain);
                std::vector<float> floatArray = hpms::ToFloatArray(matArray);
                std::vector<glm::mat4x4> matrices = hpms::ToMat4x4Array(floatArray);
                anims.transfMatricesByBoneId.insert({boneId, matrices});
                anims.frames = matrices.size();

            }
        }
    }

}

