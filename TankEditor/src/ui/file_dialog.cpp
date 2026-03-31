#include <log.hpp>
#include "file_dialog.hpp"


static std::string handleResult(nfdresult_t result, nfdchar_t **outPath)
{
    switch (result)
    {
        case NFD_OKAY:
            TE_INFO( std::format("File dialog outputted path {}", *outPath) );
            return *outPath;
        case NFD_CANCEL:
            TE_INFO( "User cancelled dialog" );
            return "";
        default:
            TE_INFO( std::format("NFD error: {}", NFD_GetError()) );
            return "";
    }
}


namespace Tank
{
    fs::path FileDialog::open(FileDialog::Target target)
    {
        nfdresult_t result;
        std::string outPathStr;

        nfdchar_t *outPath = NULL;

        switch (target)
        {
            case FileDialog::Target::File:
                result = NFD_OpenDialog(NULL, NULL, &outPath);
                break;
            case FileDialog::Target::Directory:
                result = NFD_PickFolder(NULL, &outPath);
                break;
        }

        return handleResult(result, &outPath);
    }


    fs::path FileDialog::saveAs()
    {
        nfdresult_t result;
        std::string outPathStr;
        nfdchar_t *outPath = NULL;

        result = NFD_SaveDialog(NULL, NULL, &outPath);
        return handleResult(result, &outPath);
    }
}