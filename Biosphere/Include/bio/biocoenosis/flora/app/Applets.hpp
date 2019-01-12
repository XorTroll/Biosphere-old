
#pragma once
#include <bio/biocoenosis/flora/app/Types.hpp>
#include <string>

namespace bio::app
{
    class Applet
    {
        public:
            Applet(applet::AppletId Id);
            ~Applet();
            void SetAPIVersion(u32 Version);
            void SetPlayStartupSound(bool PlaySound);
            Result SendCommonArguments();
            Result SendDataViaStorage(void *Data, size_t Size);
            ResultWrap<void*> ReceiveDataFromStorage(size_t Size);
            Result Launch();
            Result WaitFinish();
            Result GetLaunchResult();
            void Finalize();
        private:
            applet::LibraryAppletCreator *lac;
            applet::LibraryAppletAccessor *laa;
            os::Event *elaunchable;
            os::Event *estate;
            os::Event *epod;
            bool scargs;
            u8 *commonargs;
    };

    enum class ErrorAppletMode
    {
        Default,
        SystemError,
        // Add more modes!
    };

    class ErrorApplet : public Applet
    {
        public:
            ErrorApplet(ErrorAppletMode Mode, Result Res);
            void SetErrorText(std::string Text);
            void SetLongErrorText(std::string Text);
            Result Show();
        private:
            u8 *errorargs;
            u64 eargssize;
    };
}