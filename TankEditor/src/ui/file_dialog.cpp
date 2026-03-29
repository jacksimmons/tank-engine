#include <log.hpp>
#include "file_dialog.hpp"


static bool handleResult(nfdresult_t result, const std::string &outPath)
{
    if (result = NFD_OKAY)
    {
        TE_INFO( std::format("File dialog outputted path {}", outPath) );
        return true;
    }
    else if (result == NFD_CANCEL)
    {
        TE_INFO("User cancelled dialog");
    }
    else
    {
        TE_ERROR( std::format("Dialog error: {}", NFD_GetError()) );
    }
    return false;
}


namespace Tank
{
    fs::path FileDialog::selectOne(FileDialog::Target target)
    {
        nfdresult_t result;
        std::string outPathStr;
        {
            nfdchar_t *outPath = NULL;

            switch (target)
            {
                case FileDialog::Target::File:
                    result = NFD_OpenDialog(NULL, NULL, &outPath);
                    break;
                case FileDialog::Target::Directory:
                    result = NFD_PickFolder(NULL, &outPath);
            }
            outPathStr = outPath;
            free(outPath);
        }
        
        if (handleResult(result, outPathStr))
        {
            return outPathStr;
        }
        return "";
    }
}