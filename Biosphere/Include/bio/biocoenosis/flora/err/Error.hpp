
#pragma once
#include <bio/biocoenosis/flora/fatal.hpp>
#include <bio/biocoenosis/flora/app.hpp>

namespace bio::err
{
    enum class ThrowMode
    {
        ProcessExit,
        Fatal,
        AppletDialog,
    };

    void SetDefaultThrowMode(ThrowMode Mode);
    void Throw(Result Res);
    void ThrowWithMode(Result Res, ThrowMode Mode);

    static const u32 Module = 422;
    static const Result ResultNotInitialized(Module, 1);
}