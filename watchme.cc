/*
 * watchme - a simple, FAM-based file and directory watcher
 * Author: Derrick Pallas
 *
 * while ./watchme . ; do make clean all ; done
 */
#include <fam.h>

#include <sys/stat.h>

#include <cstdio>
#include <cstdlib>
#include <stdexcept>

#define TRY(f, ...) ({ \
    typeof (f(__VA_ARGS__)) _r = f(__VA_ARGS__); \
    if (_r < 0) throw std::runtime_error(#f); \
    _r; \
})

struct FAMCloser {
    FAMCloser(FAMConnection & fc) : fc(fc) { }
    ~FAMCloser() { TRY(FAMClose, &fc); }
    FAMConnection & fc;
};

int main(int argc, char* argv[]) try {

    FAMConnection fc;
    TRY(FAMOpen2, &fc, argv[0]);
    FAMCloser todo(fc);

    size_t n = 0;
    FAMRequest frs[argc];
    for (int i = 1 ; i < argc ; ++i) try {
        struct stat status;
        if (stat(argv[i], &status) < 0)
            throw std::runtime_error(argv[i]);

        char target[PATH_MAX];
        if (!realpath(argv[i], target))
            throw std::runtime_error(argv[i]);

        if ( ( S_IFDIR == (status.st_mode & S_IFMT)
             ? FAMMonitorDirectory(&fc, target, &frs[n], NULL)
             : FAMMonitorFile(&fc, target, &frs[n], NULL)
             ) < 0 )
            throw std::runtime_error(target);

        ++n;
    } catch(std::exception & e) {
        perror(e.what());
        continue;
    }

    if (n)
    for(;;) {
        FAMEvent fe;
        TRY(FAMNextEvent, &fc, &fe);
        switch(fe.code) {
        case FAMChanged:
        case FAMDeleted:
            return EXIT_SUCCESS;
        default:
            continue;
        }
    }

    return EXIT_SUCCESS;      
} catch(std::exception & e) {
    perror(e.what());
    exit(EXIT_FAILURE);
}
//
