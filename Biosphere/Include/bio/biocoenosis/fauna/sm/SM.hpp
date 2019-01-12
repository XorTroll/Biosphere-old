
#pragma once
#include <bio/biocoenosis/fauna/hipc.hpp>
#include <bio/biocoenosis/fauna/os.hpp>

namespace bio::sm
{
    Result Initialize();
    bool HasInitialized();
    ResultWrap<hipc::Object*> GetService(const char *Name);
    void Finalize();
}