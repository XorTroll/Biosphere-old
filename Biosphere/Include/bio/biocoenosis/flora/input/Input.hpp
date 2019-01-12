
#pragma once
#include <bio/biocoenosis/flora/input/Memory.hpp>
#include <bio/biocoenosis/fauna/os.hpp>

namespace bio::input
{
    class Player
    {
        public:
            Player(InputMemory *Memory, Controller PlayerNo);
            bool IsConnected();
            bool AreJoyConsJoined();
            u64 GetInput();
        private:
            InputMemory *shmem;
            Controller pno;
    };

    class InputManager
    {
        public:
            InputManager(u64 AppletResourceUserId);
            InputMemory *GetInputMemory();
            Player *GetPlayer(Controller PlayerNo);
        private:
            InputMemory *shmem;
    };
}