#include "strutils.h"

#include <string.h>



namespace Common
{

int StriCmp(const std::string &a, const std::string &b)
{
        return strcasecmp(a.c_str(), b.c_str());
}


}