
#pragma once
#include <bio/biocoenosis/fauna/hipc.hpp>
#include <bio/biocoenosis/fauna/os.hpp>

namespace bio::sm
{
    class ServiceManager : hipc::Object
    {
        public:
            using Object::Object;
            Result Initialize();
            ResultWrap<hipc::Object*> GetService(const char *Name);
    };

    ResultWrap<ServiceManager*> Initialize();
}