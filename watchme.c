/*
 * watchme - a simple, FAM-based file and directory watcher
 * Author: Derrick Pallas
 *
 * while ./watchme . ; do make clean all ; done
 */
#include <fam.h>

#include <sys/stat.h>

#include <stdio.h>
#include <stdlib.h>

#define EXIT(e) ({ \
    fprintf(stderr, "%s\n", e); \
    exit(EXIT_FAILURE); \
})

#define TRY(f, e, ...) ({ \
    typeof (f(__VA_ARGS__)) _r = f(__VA_ARGS__); \
    if (_r < 0) { EXIT(e); } \
    _r; \
})

FAMConnection fc;
void close_fc() { TRY(FAMClose, "FAMClose", &fc); }

int main(int argc, char* argv[]) {
    TRY(FAMOpen2, argv[0], &fc, argv[0]);
    atexit(close_fc);

    size_t n = 0;
    FAMRequest frs[argc];
    for (int i = 1 ; i < argc ; ++i) {
        struct stat status;
        if (stat(argv[i], &status) < 0) {
            perror(argv[i]);
            continue;
        }

        char target[PATH_MAX];
        if (!realpath(argv[i], target)) {
            perror(argv[i]);
            continue;
        }

        if ( ( S_IFDIR == (status.st_mode & S_IFMT)
             ? FAMMonitorDirectory(&fc, target, &frs[n], NULL)
             : FAMMonitorFile(&fc, target, &frs[n], NULL)
             ) < 0 )
        {
            perror(target);
        } else {
            ++n;
        }
    }

    if (!n)
        return EXIT_FAILURE;

    for(;;) {
        FAMEvent fe;
        TRY(FAMNextEvent, "FAMNextEvent", &fc, &fe);
        switch(fe.code) {
        case FAMChanged:
        case FAMDeleted:
            return EXIT_SUCCESS;
        default:
            continue;
        }
    }

    return EXIT_SUCCESS;
}
//
