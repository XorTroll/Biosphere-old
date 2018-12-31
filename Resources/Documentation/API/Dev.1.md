# Biosphere documentation → API → Getting into development [1]

## The basics

- To start working with Biosphere, you must include Biosphere's main header:

  ```cpp
  #include <bio/biosphere>
  ```

- The code in Biosphere has a slightly different entrypoint from other libraries. In Biosphere, everything starts on `bio::Main` function:

  ```cpp
  #include <bio/biosphere>

  int bio::Main()
  {
      return 0;
  }
  ```

- To get the arguments `argc` and `argv`, you have `bio::os` functions:

  ```cpp
  #include <bio/biosphere>

  int bio::Main()
  {
      int argc = bio::os::GetHostArgc();
      char **argv = bio::os::GetHostArgv();
      return 0;
  }
  ```

## Services

- To start developing with specific Nintendo Swich support, you will need to create a service manager instance, so then you can access many other services or implementations:

  ```cpp
  #include <bio/biosphere>

  int bio::Main()
  {
      bio::sm::ServiceManager *sm = bio::sm::Initialize();
      // Then, via this ServiceManager instance, we can access
      // many other things, such as input
      bio::input::InputManager *inm = new bio::input::InputManager(sm, 0);
      // Using AppletResourceUserId 0 is ok too
      bio::input::Player *hhd = inm->GetPlayer(bio::input::Controller::HandHeld);
      // Via this Player object we control HandHeld controlling (JoyJons attached on the console)
      return 0;
  }
  ```