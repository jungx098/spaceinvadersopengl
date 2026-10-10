/* Helpers shared by the macOS platform files. */
#ifndef MACOS_PORT_H
#define MACOS_PORT_H

#ifdef __cplusplus
extern "C" {
#endif

unsigned macos_asset_bytes(unsigned id, const char* kind);

/* Decode an image file or memory block to tightly packed pixels.
   components 3 = BGR, 4 = BGRA. bottom_up writes the bottom row first. */
int macos_decode_memory(const void* data, unsigned size, int components, int bottom_up,
	unsigned char** out, int* width, int* height);

void* macos_global_lock(void* block, unsigned* size);

void* macos_dc_bits(void* hdc, int* width, int* height);

#ifdef __cplusplus
}
#endif

#endif
