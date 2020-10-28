/*!
 * File DaeUtils.cpp
 */

#import <tools/dae/DaeUtils.h>


std::vector<std::string> hpms::PlainStringToArray(const std::string& stringVal)
{
    std::string trimmedVal = hpms::Trim(stringVal);

    std::vector<std::string> ret;

    bool recording = false;
    for (const char& c : trimmedVal)
    {
        std::string current;
        if (c == ' ')
        {
            if (recording)
            {
                recording = false;
                ret.push_back(current);
                current = "";
            }
        } else
        {
            recording = true;
            current.push_back(c);
        }

    }
}

std::vector<float> hpms::ToFloatArray(const std::vector<std::string>& stringArr)
{
    std::vector<float> ret;
    ret.reserve(stringArr.size());
    for (const std::string& s : stringArr)
    {
        ret.push_back(std::stof(s));
    }
    return ret;
}

std::vector<int> hpms::ToIntArray(const std::vector<std::string>& stringArr)
{
    std::vector<int> ret;
    ret.reserve(stringArr.size());
    for (const std::string& s : stringArr)
    {
        ret.push_back(std::stoi(s));
    }
    return ret;
}

std::vector<std::pair<int, int>> hpms::ToPairArray(const std::vector<int>& array)
{
    std::vector<std::pair<int, int>> ret;
    ret.reserve(array.size() / 2);
    int count = 1;
    int prev;
    for (int i : array)
    {
        if (count++ % 2 == 0)
        {
            ret.emplace_back(prev, i);
        } else
        {
            prev = i;
        }
    }
    return ret;
}

std::vector<glm::vec3> hpms::ToVec3Array(const std::vector<float>& array)
{
    std::vector<glm::vec3> ret;
    ret.reserve(array.size() / 3);
    int count = 1;
    std::vector<float> prev;
    prev.reserve(3);
    for (int f : array)
    {
        if (count++ % 3 == 0)
        {
            ret.emplace_back(prev[0], prev[1], prev[2]);
        } else
        {
            prev.push_back(f);
        }
    }
    return ret;
}

std::vector<glm::mat4x4> hpms::ToMat4x4Array(const std::vector<float>& array)
{
    std::vector<glm::mat4x4> ret;
    ret.reserve(array.size() / 16);
    int count = 1;
    std::vector<float> prev;
    prev.reserve(16);
    for (int f : array)
    {
        if (count++ % 16 == 0)
        {
            ret.emplace_back(prev[0], prev[1], prev[2], prev[3],
                             prev[4], prev[5], prev[6], prev[7],
                             prev[8], prev[9], prev[10], prev[11],
                             prev[12], prev[13], prev[14], prev[15]);
        } else
        {
            prev.push_back(f);
        }
    }
    return ret;
}
