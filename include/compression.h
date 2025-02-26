#ifndef COMPRESSION_H
#define COMPRESSION_H

#include <stddef.h>
int CLOAK_Compress(const char *input_file, const char *output_file);
int CLOAK_Decompress(const char *input_file, const char *output_file);
int CLOAK_CompressChunk(const unsigned char *input, size_t input_size, unsigned char *output, size_t *output_size);
int CLOAK_DecompressChunk(const unsigned char *input, size_t input_size, unsigned char *output, size_t *output_size);


#ifdef CLOAK_REMOVE_PREFIXES
#define Compress CLOAK_Compress
#define Decompress CLOAK_Decompress
#define CompressChunk CLOAK_CompressChunk
#define DecompressChunk CLOAK_DecompressChunk
#endif // CLOAK_REMOVE_PREFIXES

#endif // COMPRESSION_H
