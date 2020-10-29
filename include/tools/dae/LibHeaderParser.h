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
    public:
        static void Process(HeaderData& header, pugi::xml_node& node);

    };
}


