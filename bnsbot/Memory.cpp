#include "Memory.h"
#include <stdio.h>

#define MEMORY_SHOW_DEBUG_MESSAGES 0

uintptr_t GetAddressByPointer(uintptr_t base, std::vector<uintptr_t> offsets) {
	base += offsets[0];
	for (int i = 1; i < offsets.size(); i++) {
		if (!base) {
			printf("ERROR: GetAddressByPointer at base=%p, offsets[%i]=%i\n", base, i, offsets[i]);
			return NULL;
		}
#if defined MEMORY_SHOW_DEBUG_MESSAGES
		printf("base = %p\n", (void *)base);
#endif

		// Read this address and then check if it's a NULL pointer
		base = *(uintptr_t *)base;
#if defined MEMORY_SHOW_DEBUG_MESSAGES
		printf("*base = %p, offset = %p\n", (void *)base, offsets[i]);
#endif
		if (!base) {
			printf("ERROR: GetAddressByPointer at base=%p, offsets[%i]=%i\n", base, i, offsets[i]);
			return NULL;
		}

		// Add the offset afterwards because NULL+offset != NULL (usual case)
		base += offsets[i];
#if defined MEMORY_SHOW_DEBUG_MESSAGES
		printf("new base = %p\n", (void *)base);
#endif

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