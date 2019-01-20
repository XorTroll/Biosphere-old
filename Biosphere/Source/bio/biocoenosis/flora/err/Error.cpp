#include <bio/biocoenosis/flora/err/Error.hpp>
#include <cstdlib>

namespace bio::err
{
    static ThrowMode tmode = ThrowMode::ProcessExit;

    void SetDefaultThrowMode(ThrowMode Mode)
    {
        tmode = Mode;
    }

    void Throw(Result Res)
    {
        ThrowWithMode(Res, tmode);
    }

    void ThrowWithMode(Result Res, ThrowMode Mode)
    {
        if(Res.IsSuccess()) return;
        fatal::FatalService *fsrv;
        switch(Mode)
        {
            case ThrowMode::ProcessExit:
                exit((u32)Res);
                break;
            case ThrowMode::Fatal:
                fsrv = fatal::Initialize().AssertOk();
                fsrv->ThrowWithPolicy(Res, fatal::ThrowMode::ErrorScreen);
                delete fsrv;
                break;
            case ThrowMode::AppletDialog:
                if(!app::HasInitialized()) return;
                app::ErrorApplet *err = new app::ErrorApplet(app::ErrorAppletMode::Default, Res);
                err->Show().AssertOk();
                delete err;
                break;
        }
    }
}