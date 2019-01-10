#include <bio/root/Types.hpp>
#include <bio/biocoenosis/flora/err.hpp>

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
        if(this->IsFailure()) err::Throw((u32)*this);
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

    ErrorCode::ErrorCode()
    {
        this->Value = 0;
    }

    ErrorCode::ErrorCode(Result ResultCode)
    {
        this->Value = (((ResultCode & 0x1ffu) + 2000) | (((ResultCode >> 9) & 0x1fff & 0x1fffll) << 32));
    }

    ErrorCode::ErrorCode(u32 Module, u32 Description)
    {
        u32 err = Result(Module, Description);
        this->Value = (((err & 0x1ffu) + 2000) | (((err >> 9) & 0x1fff & 0x1fffll) << 32));
    }

    bool ErrorCode::IsFailure()
    {
        return (this->Value != 0);
    }

    bool ErrorCode::IsSuccess()
    {
        return (this->Value == 0);
    }

    ErrorCode::operator u64()
    {
        return this->Value;
    }

    Result ResultClass::GetResult()
    {
        return this->res;
    }
}