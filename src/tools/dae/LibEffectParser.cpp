/*!
 * File LibMaterialParser.cpp
 */

#include <tools/dae/LibEffectParser.h>
#include <tools/dae/DaeUtils.h>

void hpms::LibEffectsParser::Process(hpms::FxInstanceData& fxData, pugi::xml_node& node)
{
    for (pugi::xml_node effect : node.children("effect"))
    {
        std::string instId = effect.attribute("id").as_string();
        std::string colorString = effect.child(PROFILE).child("technique").child(LIGHT_METHOD).child(LIGHT_TYPE).child("color").child_value();
        std::vector<std::string> colorStringVector = hpms::PlainStringToArray(colorString);
        std::vector<float> colorFloatVector = hpms::ToFloatArray(colorStringVector);
        glm::vec4 color = glm::vec4(colorFloatVector[0], colorFloatVector[1], colorFloatVector[2], colorFloatVector[3]);
        fxData.effectByInstanceId.insert({instId, color});
    }
}
