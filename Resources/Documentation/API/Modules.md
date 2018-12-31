# Biosphere documentation → API → Modular library

## A modular library

- Biosphere is a "modular" library. It's splitted into **groups** and **modules** (and subgroups and submodules too).

## Module information

### **Group:** *Biotope*

- Biotope is the more low-level part of the library. It contains basic initialization functions, with the most lower part written in ASM, access to SVC calls, written in ASM and then extended in C++, and homebrew environment / ARM-related stuff.

  - **Module:** *init* (`bio::init`)

    - This module contains the different functions to be called on initialization or in exit. The entrypoint is also defined there.

    - IMPORTANT! Biosphere's entrypoint is NOT `int main(int argc, char **argv)`. Biosphere provides a different entrypoint: `int bio::Main()`. Arguments can be accessed via `os` module of fauna: `bio::os::GetHostArgc / GetHostArgv`

  - **Module:** *arm* (`bio::arm`)

  - **Module:** *env* (`bio::env`)

  - **Module:** *svc* (`bio::svc`)

### **Group:** *Biocoenosis*

- Biocoenosis is the part of the library containing high-level implementations. It is internally divided into two subgroups:

  - **Subgroup:** *Fauna*

    - Fauna contains medium-level implementations of two main areas: Horizon OS's elements (like Mutex, SharedMemory...) and IPC support: marshalling, sessions, handle objects...

    - **Module:** *os* (`bio::os`)

    - **Module:** *hipc* (`bio::hipc`)

  - **Subgroup:** *Flora*

    - Flora contains the most high-level and useful elements, both using most of the other groups or modules mentioned above, mostly containing two different module types: IPC service implementations, and modules simplifying and/or extending services' usage.

    - Best example of this concept happens with `hid` and `input`: `hid` just implements the homonymous service, while `input` has various classes and types which simplify input access and management, using `hid`.

    - **Module:** *applet* (`bio::applet`)

      - Applet itself doesn't provide any service implementation, but contains all the proxy IPC objects which will be used by the other submodules.

      - **Submodule:** *ae* (`bio::applet::ae`)

        - Implements `appletAE` service.

      - **Submodule:** *oe* (`bio::applet::oe`)

        - Implements `appletOE` service.

    - **Module:** *diag* (`bio::diag`)

      - Extends `fatal` module for simple Result assets and aborts.

      - Must be initialized with a SM object before asserting!

    - **Module:** *fatal* (`bio::fatal`)

      - Implements `fatal:u` service.

    - **Module:** *gpio* (`bio::gpio`)

      - Implements the homonymous service.

    - **Module:** *hid* (`bio::hid`)

      - Implements the homonymous service.

      - **Submodule:** *dbg* (`bio::hid::dbg`)

        - Implements `hid:dbg` service.

    - **Module:** *input* (`bio::input`)

      - Extends `hid` module providing an easy way to manage controller input via InputManager and Player classes, plus all the other enums/structs.

    - **Module:** *psm* (`bio::psm`)

      - Implements the homonymous service.

    - **Module:** *sm* (`bio::sm`)

      - Implements the homonymous port, the base to access any other service.

      - A ServiceManager object should be always initialized, because most of the modules do require it.

For a more detailed documentation about the library (which I'm sure you'll need), check the rest of the documentation.