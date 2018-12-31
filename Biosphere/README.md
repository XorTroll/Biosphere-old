# Biosphere project → Biosphere library

## Biosphere library and it's aim

- Biosphere is a high-level, C++ only homebrew library, based on three other libraries:

  - Most of the low and medium-level code is ported from libnx.

  - Some concepts, names and ideas are brought from libtransistor.

  - The syntax and structure of the library are completely based on Nintendo's official libraries, nn libs.  Anyway, this code DOESN'T CONTAIN CONTENT from those libraries, only got inspired by them (`bio` namespace is used instead of `nn`, and most internal namespaces are different, but some of them like `os`, `hipc`, `svc` and `init` are inspired from nn libs)

- Biosphere aims to get a simple, easy-to-use C++ homebrew library, with a simple (un)installation.

## Install / uninstall

- Biosphere's releases are ZIP files, containing a Biosphere folder. That folder must be placed inside `devkitPro` folder, as Biosphere, despite having its own Makefile system, needs devkitPro's compilers and directories.

- Simply delete the folder to delete Biosphere. All Biosphere's assets, makefiles and sources are all inside Biosphere folder.

## API / technical information

You can browse API details [here](API.md).

## Credits

- libnx and it's authors and contributors (although I've contributed with some minimal implementations). Without libnx, this library wouldn't be a thing.