/*!
 * File LibHeaderParser.h
 */

#pragma once

#include <glm/glm.hpp>
#include <pugixml.hpp>



namespace hpms
{
    struct HeaderData {
        glm::vec3 upAxis;
        float unit;
    };

    class LibHeaderParser
    {

        static void Process(const HeaderData& header, pugi::xml_node& node);

    };
}


