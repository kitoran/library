#ifndef NEWFILE_H
#define NEWFILE_H

#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <filesystem>

namespace newfile_private {
    bool exists___(const std::string& path) {
        struct stat buffer;
        return (stat (path.c_str(), &buffer) == 0);
    }
}
inline std::string newFile(std::string path, std::string extension) {

    using namespace newfile_private;
    if (!exists___(path + '.' + extension)) {
        return path;
    }
    int i = 1;
    while(exists___(path + std::to_string(i) + '.' + extension)) {
        i++;
    }
    return path + std::to_string(i) + '.' + extension;
}

#endif // NEWFILE_H
