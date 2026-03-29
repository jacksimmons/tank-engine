#pragma once
#include <nfd.h>


namespace Tank
{
    namespace FileDialog
    {
        enum class Target
        {
            File,
            Directory
        };


        fs::path selectOne(Target target);
        static fs::path selectFile() { return selectOne(Target::File); }
        static fs::path selectDirectory() { return selectOne(Target::Directory); }
    }
}