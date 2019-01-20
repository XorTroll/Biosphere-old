
#pragma once
#include <bio/biocoenosis/flora/app/Types.hpp>
#include <bio/biocoenosis/flora/account.hpp>
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
            ~ErrorApplet();
            void SetErrorText(std::string Text);
            void SetLongErrorText(std::string Text);
            Result Show();
        private:
            u8 *errorargs;
            u64 eargssize;
    };

    class PlayerSelectApplet : public Applet
    {
        public:
            PlayerSelectApplet();
            ~PlayerSelectApplet();
            ResultWrap<account::Uid> Show();
        private:
            u8 *pselargs;
    };

    class WebApplet : public Applet
    {
        public:
            WebApplet(std::string RequestUrl);
            ~WebApplet();
            void SetRequestUrl(std::string RequestUrl);
            Result Show();
        private:
            u8 *webargs;
    };
}