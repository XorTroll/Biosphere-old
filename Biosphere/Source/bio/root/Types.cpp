#include <bio/root/Types.hpp>
#include <bio/biocoenosis/flora/diag.hpp>

namespace bio
{
    ApplicationId::ApplicationId(u64 Id)
    {
        this->Id = Id;
    }

    bool ApplicationId::operator==(const ApplicationId &Other)
    {
        return (this->Id == Other.Id);
    }

    bool ApplicationId::operator!=(const ApplicationId &Other)
    {
        return (this->Id != Other.Id);
    }

    ApplicationId ApplicationId::GetInvalidId()
    {
        return ApplicationId(0);
    }

    Result::Result()
    {
        this->Module = 0;
        this->Description = 0;
    }

    Result::Result(u32 ResultCode)
    {
        this->Module = (ResultCode & 0x1ff);
        this->Description = ((ResultCode >> 9) & 0x1fff);
    }

    Result::Result(u32 Module, u32 Description)
    {
        this->Module = Module;
        this->Description = Description;
    }

    bool Result::IsFailure()
    {
        return (this->Description != 0);
    }

    bool Result::IsSuccess()
    {
        return (this->Description == 0);
    }

    void Result::AssertOk()
    {
        if(this->IsFailure())
        {
            if(!diag::HasInitialized()) exit((u32)*this);
            else diag::AssertResultOk(*this);
        }
    }

    bool Result::operator==(const Result &Other)
    {
        return ((this->Module == Other.Module) && (this->Description == Other.Description));
    }

    bool Result::operator!=(const Result &Other)
    {
        return ((this->Module != Other.Module) || (this->Description != Other.Description));
    }

    Result::operator u32()
    {
        return ((this->Module & 0x1ff) | (this->Description & 0x1fff) << 9);
    }

    Result ResultClass::GetResult()
    {
        return this->res;
    }
}