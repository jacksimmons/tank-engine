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


        fs::path open(Target target);
        fs::path saveAs();
    }
}