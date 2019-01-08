#include <bio/biocoenosis/flora/err/Error.hpp>
#include <cstdlib>

namespace bio::err
{
    static ThrowMode tmode = ThrowMode::ProcessExit;
    static fatal::FatalService *fatalref = NULL;
    static applet::LibraryAppletCreator *lacref = NULL;

    void InitializeFatalThrowMode(fatal::FatalService *Fatal)
    {
        fatalref = Fatal;
    }

    void InitializeAppletThrowMode(applet::LibraryAppletCreator *AppletCreator)
    {
        // Add this as soon as applets work!
    }

    Result SetDefaultThrowMode(ThrowMode Mode)
    {
        switch(Mode)
        {
            case ThrowMode::Fatal:
                if(fatalref == NULL) return ResultNotInitialized;
                break;
            case ThrowMode::AppletDialog:
                if(lacref == NULL) return ResultNotInitialized;
                break;
        }
        tmode = Mode;
        return ResultSuccess;
    }

    void Throw(Result Res)
    {
        ThrowWithMode(Res, tmode);
    }

    void ThrowWithMode(Result Res, ThrowMode Mode)
    {
        if(Res.IsSuccess()) return;
        switch(Mode)
        {
            case ThrowMode::ProcessExit:
                exit((u32)Res);
                break;
            case ThrowMode::Fatal:
                if(fatalref == NULL) return;
                fatalref->ThrowWithPolicy(Res, fatal::ThrowMode::ErrorScreen);
                break;
            case ThrowMode::AppletDialog:
                if(lacref == NULL) return;
                // Add this as soon as applets work!
                break;
        }
    }
}