#pragma once

#include <sstream>
#include <vector>

namespace utils
{

void split_topics(std::vector<std::string>* vec,
                  const std::string& str, const char delim);

}

