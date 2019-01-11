
#pragma once
#include <bio/biocoenosis/flora/app/Types.hpp>

namespace bio::app
{
    Result Initialize(sm::ServiceManager *SM, RunMode Mode);
    void Finalize();
    bool HasInitialized();
    bool IsApplet();
    bool IsApplication();
    bool IsAppletOrApplication();
    bool IsSubprocess();
}