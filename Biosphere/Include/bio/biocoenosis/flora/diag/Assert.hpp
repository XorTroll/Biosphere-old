
#pragma once
#include <bio/biotope/svc.hpp>
#include <bio/biocoenosis/flora/fatal.hpp>
#include <functional>

namespace bio::diag
{
    void Initialize(sm::ServiceManager *SM);
    bool HasInitialized();
    Result AssertOk(bool Condition, Result ToThrow);
    Result AssertResultOk(Result Condition);

    static const Result ResultNotInitialized(300, 1);
}