
#pragma once
#include <bio/biocoenosis/fauna/sm/Types.hpp>

namespace bio::sm
{
    Result Initialize();
    bool HasInitialized();
    ResultWrap<hipc::Object*> GetService(const char *Name);
    void Finalize();
}