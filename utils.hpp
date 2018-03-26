#include <string>
#include <dirent.h>

inline std::string pathSeparator()
{
#ifdef _WIN32
    return "\\";
#else
    return "/";
#endif
}

inline int isDir(std::string folder)
{
    if (auto dir = opendir(folder.c_str()))
    {
        closedir(dir);
        return 0;
    }
    else
    {
        return -1;
    }
}

inline int mkdir(std::string folder)
{
    mode_t nMode = 0733; // UNIX style permissions
    int nError = 0;
    
    //create dir
    nError = mkdir(folder.c_str(),nMode); // can be used on non-Windows
    if (nError != 0) 
    {
        return -1;
    }
    return 0;
}