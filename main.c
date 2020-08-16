#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "smaz.h"

static char *prognm;

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

int main(int argc, char *argv[]) {
    char *ibuf, *obuf;
    int rc = 0, len;
    size_t isz, osz;
    FILE *in, *out;

    prognm = strrchr(argv[0], '/');
    if (prognm) prognm++;
    else        prognm = argv[0];

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
        
    in = fopen(argv[2], "rb");
    if (!in) {
        fprintf(stderr, "Failed opening input file %s: %s\n", argv[2], strerror(errno));
        return 1;
    }
        
    out = fopen(argv[3], "wb");
    if (!out) {
        fclose(in);
        fprintf(stderr, "Failed opening output file %s: %s\n", argv[3], strerror(errno));
        return 1;
    }

    fseek(in, 0, SEEK_END);
    isz = ftell(in);
    fseek(in, 0, SEEK_SET);
    ibuf = malloc(isz);
    if (!ibuf) {
    memerr:
        fprintf(stderr, "Failed allocating memory: %s", strerror(errno));
        rc = 1;
        goto end;
    }

    osz = isz * 10;
    obuf = malloc(osz);
    if (!obuf) {
        free(ibuf);
        goto memerr;
    }

    if (fread(ibuf, isz, 1, in) != isz) {
        fprintf(stderr, "Failed reading %s: %s", argv[2], strerror(errno));
        rc = 1;
        goto readerr;
    }

    if (strncmp(argv[1], "-c", 2) == 0)
        len = smaz_compress(ibuf, isz, obuf, osz);
    else
        len = smaz_decompress(ibuf, isz, obuf, osz);

    if (fwrite(obuf, len, 1, out) != (size_t)len) {
        fprintf(stderr, "Failed writing data to %s: %s", argv[3], strerror(errno));
        rc = 1;
    }

readerr:
    free(ibuf);
    free(obuf);
end:
    fclose(in);
    fclose(out);

    return rc;
}

/**
 * Local Variables:
 *  indent-tabs-mode: nil
 *  c-file-style: "stroustrup"
 * End:
 */
