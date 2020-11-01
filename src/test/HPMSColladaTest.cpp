/*!
 * File HPMSColladaTest.cpp
 */

#include <tools/LightColladaImporter.h>
#include <string>
#include <iostream>

int main()
{
    hpms::LightColladaImporter::LoadModelItem("Man_AnimSet.dae", "");
    return 0;
}