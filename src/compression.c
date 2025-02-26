#include "compression.h"
#include <stdio.h>

#include <zlib.h>

int CLOAK_Compress(const char *input_file, const char *output_file)
{
    FILE *fin = fopen(input_file, "rb");
    FILE *fout = fopen(output_file, "wb");
    if (!fin || !fout) {
        perror("File open error");
        return -1;
    }

    unsigned char in_buffer[4096];
    unsigned char out_buffer[4096];

    z_stream stream = {0};
    stream.zalloc = Z_NULL;
    stream.zfree = Z_NULL;
    stream.opaque = Z_NULL;

    if (deflateInit(&stream, Z_BEST_COMPRESSION) != Z_OK) {
        fprintf(stderr, "Failed to initialize zlib compression\n");
        fclose(fin);
        fclose(fout);
        return -1;
    }

    int flush;
    do {
        stream.avail_in = fread(in_buffer, 1, sizeof(in_buffer), fin);
        if (ferror(fin)) {
            fprintf(stderr, "File read error\n");
            deflateEnd(&stream);
            fclose(fin);
            fclose(fout);
            return -1;
        }
        flush = feof(fin) ? Z_FINISH : Z_NO_FLUSH;
        stream.next_in = in_buffer;

        do {
            stream.avail_out = sizeof(out_buffer);
            stream.next_out = out_buffer;
            deflate(&stream, flush);
            fwrite(out_buffer, 1, sizeof(out_buffer) - stream.avail_out, fout);
        } while (stream.avail_out == 0);

    } while (flush != Z_FINISH);

    deflateEnd(&stream);
    fclose(fin);
    fclose(fout);
    return 0;
}

int CLOAK_CompressChunk(const unsigned char *input, size_t input_size, unsigned char *output, size_t *output_size)
{
    z_stream stream;
    stream.zalloc = Z_NULL;
    stream.zfree = Z_NULL;
    stream.opaque = Z_NULL;

    if (deflateInit(&stream, Z_BEST_COMPRESSION) != Z_OK) {
        fprintf(stderr, "Failed to initialize compression\n");
        return -1;
    }

    stream.next_in = (Bytef *)input;
    stream.avail_in = input_size;
    stream.next_out = output;
    stream.avail_out = *output_size;

    if (deflate(&stream, Z_FINISH) != Z_STREAM_END) {
        fprintf(stderr, "Compression failed\n");
        deflateEnd(&stream);
        return -1;
    }

    *output_size = stream.total_out;
    deflateEnd(&stream);
    return 0;
}


int CLOAK_Decompress(const char *input_file, const char *output_file)
{
    FILE *fin = fopen(input_file, "rb");
    FILE *fout = fopen(output_file, "wb");
    if (!fin || !fout) {
        perror("File open error");
        return -1;
    }

    unsigned char in_buffer[4096];
    unsigned char out_buffer[4096];

    z_stream stream = {0};
    stream.zalloc = Z_NULL;
    stream.zfree = Z_NULL;
    stream.opaque = Z_NULL;
    stream.avail_in = 0;
    stream.next_in = Z_NULL;

    if (inflateInit(&stream) != Z_OK) {
        fprintf(stderr, "Failed to initialize zlib decompression\n");
        fclose(fin);
        fclose(fout);
        return -1;
    }

    int ret;
    do {
        stream.avail_in = fread(in_buffer, 1, sizeof(in_buffer), fin);
        if (ferror(fin)) {
            fprintf(stderr, "File read error\n");
            inflateEnd(&stream);
            fclose(fin);
            fclose(fout);
            return -1;
        }
        if (stream.avail_in == 0) break;
        stream.next_in = in_buffer;

        do {
            stream.avail_out = sizeof(out_buffer);
            stream.next_out = out_buffer;
            ret = inflate(&stream, Z_NO_FLUSH);
            switch (ret) {
                case Z_NEED_DICT:
                case Z_DATA_ERROR:
                case Z_MEM_ERROR:
                    fprintf(stderr, "Zlib decompression error\n");
                    inflateEnd(&stream);
                    fclose(fin);
                    fclose(fout);
                    return -1;
            }
            fwrite(out_buffer, 1, sizeof(out_buffer) - stream.avail_out, fout);
        } while (stream.avail_out == 0);

    } while (ret != Z_STREAM_END);

    inflateEnd(&stream);
    fclose(fin);
    fclose(fout);
    return 0;
}

int CLOAK_DecompressChunk(const unsigned char *input, size_t input_size, unsigned char *output, size_t *output_size)
{
    z_stream stream;
    stream.zalloc = Z_NULL;
    stream.zfree = Z_NULL;
    stream.opaque = Z_NULL;

    if (inflateInit(&stream) != Z_OK) {
        fprintf(stderr, "Failed to initialize decompression\n");
        return -1;
    }

    stream.next_in = (Bytef *)input;
    stream.avail_in = input_size;
    stream.next_out = output;
    stream.avail_out = *output_size;

    if (inflate(&stream, Z_FINISH) != Z_STREAM_END) {
        fprintf(stderr, "Decompression failed\n");
        inflateEnd(&stream);
        return -1;
    }

    *output_size = stream.total_out;
    inflateEnd(&stream);
    return 0;
}
