# Biosphere

## About Biosphere

- Biosphere is a project whose aim is to provide an easy and effective way of developing homebrew.

- Biosphere is splitted into different parts (see below).

## Biosphere's subprojects

### Biosphere library

- These are the main libraries of Biosphere, based on [libnx libraries](https://github.com/switchbrew/libnx), and inspired on Nintendo's official nn libraries and [libtransistor libraries](https://github.com/reswitched/libtransistor).

- Most of the internal and/or low-level code is ported from libnx, whose authors have made a huge work with their libraries. This project wouldn't have been developed without their contributions to homebrew.

- The syntax and structure of the library is almost completely inspired on Nintendo's libraries: same way Nintendo uses `nn` namespace, Biosphere library use `bio` namespace.

- The concept of using object pointers for services and various kernel objects, plus some other ideas, come from libtransistor libraries. ReSwitched team has made huge contributions to homebrew development and knowledge.

### Ecosphere build system

- Ecosphere is the building system for applications, using Biosphere library.

Check the [documentation]() for more info!