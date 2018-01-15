#pragma once
#include <Windows.h>
#include <cstdint>
#include <vector>

/// <summary>
/// Returns an address pointed recursively by its base and offsets.
/// </summary>
uintptr_t GetAddressByPointer(uintptr_t base, std::vector<uintptr_t> offsets);

bool Compare(const BYTE* pData, const BYTE* bMask, const char* szMask);

uintptr_t Pattern(uintptr_t dwAddress, uintptr_t dwLen, const BYTE *bMask, const char * szMask);