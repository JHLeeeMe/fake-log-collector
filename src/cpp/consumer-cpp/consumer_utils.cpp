#include "consumer_utils.hpp"

namespace utils
{

void split_topics(std::vector<std::string>* vec,
                  const std::string& str, const char delim)
{
    std::string token;
    std::istringstream token_stream{str};
    while (std::getline(token_stream, token, delim))
    {
        vec->push_back(token);
    }
}

}

