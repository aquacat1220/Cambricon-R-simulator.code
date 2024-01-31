#include "AIBACommon.hpp"

bool operator==(const HashEntry& lhs, const HashEntry& rhs)
{
    return ((lhs.elem0 == rhs.elem0) && (lhs.elem1 == rhs.elem1));
}