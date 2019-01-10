
#pragma once
#include <bio/biocoenosis/flora/applet.hpp>

namespace bio::app
{
    enum class RunMode
    {
        Subprocess,
        Application,
        SystemApplication,
        LibraryApplet,
        SystemApplet,
        OverlayApplet,
    };
}