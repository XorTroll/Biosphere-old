#include <bio/biotope/env/Launch.hpp>
#include <cstdlib>

extern char *nextpath;
extern char *nextargv;

namespace bio::env
{
    void Launch(const char *NRO, std::vector<const char*> Args)
    {
        char *argv = (char*)Args.data();
        strcpy(nextpath, NRO);
        if(nextargv != NULL)
        {
            if(argv == NULL) nextargv[0] = '\0';
            else strcpy(nextargv, argv);
        }
        exit(0);
    }
}