#include "Memory.h"
#include <stdio.h>

uintptr_t GetAddressByPointer(uintptr_t base, std::vector<uintptr_t> offsets) {
	base += offsets[0];
	for (int i = 1; i < offsets.size(); i++) {
		if (!base) {
			printf("ERROR: GetAddressByPointer at base=%p, offsets[%i]=%i\n", base, i, offsets[i]);
			return NULL;
		}
		printf("base = %p, offset = %p\n", base, offsets[i]);
		base = *(uintptr_t *)base + offsets[i];
		printf("new base = %p\n", base);
	}

	return base;
}

bool Compare(const BYTE* pData, const BYTE* bMask, const char* szMask) {
	for (; *szMask; ++szMask, ++pData, ++bMask)
		if (*szMask == 'x' && *pData != *bMask)   return 0;
	return (*szMask) == NULL;
}

uintptr_t Pattern(uintptr_t dwAddress, uintptr_t dwLen, const BYTE *bMask, const char * szMask) {
	for (uintptr_t i = 0; i < dwLen; i++) {
		if (Compare((BYTE*)(dwAddress + i), bMask, szMask))  return (uintptr_t)(dwAddress + i);
	}

	return 0;
}