#pragma once

#define BranchTo(offset,rtype,...) \
	((rtype (__fastcall *)(...))offset)(__VA_ARGS__)

template<typename T>
T clamp(T value, T min, T max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}


struct DLL_STD_MAP_CONST_CHAR_COMPARE
{
    bool operator()(const char* lhs, const char* rhs) const
    {
        return std::strcmp(lhs, rhs) < 0;
    }
};
