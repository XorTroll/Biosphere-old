#include <bio/biocoenosis/flora/diag/Assert.hpp>

namespace bio::diag
{
    sm::ServiceManager *smptr;
    bool sinit = false;

    void Initialize(sm::ServiceManager *SM)
    {
        smptr = SM;
        smptr->Initialize();
        sinit = true;
    }

    bool HasInitialized()
    {
        return sinit;
    }

    Result AssertOk(bool Condition, Result ToThrow)
    {
        Result rc = 0;
        if(!sinit) return ResultNotInitialized;
        if(!Condition) rc = AssertResultOk(ToThrow);
        return rc;
    }

    Result AssertResultOk(Result Condition)
    {
        Result rc = 0;
        if(!sinit) return ResultNotInitialized;
        if(Condition.IsFailure())
        {
            fatal::FatalService *fatal = fatal::Initialize(smptr).AssertOk();
            rc = fatal->ThrowWithPolicy(Condition, fatal::ThrowMode::ErrorScreen);
        }
        return rc;
    }
}