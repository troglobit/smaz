#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "smaz.h"

#define MAXSIZE 4096

static const char *prognm;

int usage(int rc) {
    FILE *fp = rc ? stderr : stdout;

    fprintf(fp,
            "Usage:\n"
            "  %s [-cdh] infile outfile\n"
            "\n"
            "Options:\n"
            "  -c          compress\n"
            "  -d          decompress\n"
            "  -h, --help  this help text\n", prognm);

    return rc;
}

int main(int argc, char const *argv[]) {
    char in[MAXSIZE];
    char out[MAXSIZE];
    const char *ptr;
    FILE *fp_in;
    FILE *fp_out;
    int file_size;
    int output_size;

    ptr = strrchr(argv[0], '/');
    if (ptr)
        ptr++;
    else
        ptr = argv[0];
    prognm = ptr;

    if (!strncmp(argv[1], "--help", 6) || !strncmp(argv[1], "-h", 2))
        return usage(0);

    if (argc < 4) {
        fprintf(stderr, "[!] Too few arguments\n");
        return usage(1);
    }
    if (strncmp(argv[1], "-c", 2) && strncmp(argv[1], "-d", 2)) {
        fprintf(stderr, "[!] Invalid Option: %s\n", argv[1]);
        return usage(1);
    }
        

    fp_in = fopen(argv[2], "rb");
    if (!fp_in) {
        fprintf(stderr, "Failed opening input file %s: %m\n", argv[2]);
        return 1;
    }
        
    fp_out = fopen(argv[3], "wb");
    if (!fp_out) {
        fclose(fp_in);
        fprintf(stderr, "Failed opening output file %s: %m\n", argv[3]);
        return 1;
    }

    fseek(fp_in, 0, SEEK_END);
    file_size = ftell(fp_in);
    fseek(fp_in, 0, SEEK_SET);

    fread(in, file_size, 1, fp_in);

    if (strncmp(argv[1], "-c", 2) == 0)
        output_size = smaz_compress(in, file_size, out, MAXSIZE);
    else
        output_size = smaz_decompress(in, file_size, out, MAXSIZE);

    fwrite(out, output_size, 1, fp_out);
    fclose(fp_in);
    fclose(fp_out);

    return 0;
}

/**
 * Local Variables:
 *  indent-tabs-mode: nil
 *  c-file-style: "stroustrup"
 * End:
 */
