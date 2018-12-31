#include <bio/biocoenosis/flora/input/Input.hpp>

namespace bio::input
{
    Player::Player(InputMemory *Memory, Controller PlayerNo)
    {
        this->shmem = Memory;
        this->pno = PlayerNo;
    }

    bool Player::IsConnected()
    {
        ControllerData cd = this->shmem->Controllers[static_cast<u32>(this->pno)];
        return (cd.Main.Entries[cd.Main.LatestIndex].ConnectionState & BIO_BITMASK(0));
    }

    bool Player::AreJoyConsJoined()
    {
        return !(bool)(this->shmem->Controllers[static_cast<u32>(this->pno)].IsJoyConHalf);
    }

    u64 Player::GetInput()
    {
        ControllerData cd = this->shmem->Controllers[static_cast<u32>(this->pno)];
        return cd.Main.Entries[cd.Main.LatestIndex].ButtonState;
    }

    InputManager::InputManager(sm::ServiceManager *SM, u64 AppletResourceUserId)
    {
        hid::HidService *hid = hid::Initialize(SM).AssertOk();
        hid::AppletResource *har = hid->CreateAppletResource(AppletResourceUserId).AssertOk();
        u32 memh = har->GetSharedMemoryHandle().AssertOk();
        os::SharedMemory *shm = new os::SharedMemory(memh, 0x40000, Permission::Read);
        shm->Map().AssertOk();
        hid->ActivateNpad(AppletResourceUserId).AssertOk();
        hid->SetSupportedNpadStyleSet((static_cast<u32>(hid::NpadStyleTag::ProController) | static_cast<u32>(hid::NpadStyleTag::Handheld) | static_cast<u32>(hid::NpadStyleTag::JoyconPair) | static_cast<u32>(hid::NpadStyleTag::JoyconLeft) | static_cast<u32>(hid::NpadStyleTag::JoyconRight)), AppletResourceUserId).AssertOk();
        u32 ctbuf[9] = { 0, 1, 2, 3, 4, 5, 6, 7, 0x20 };
        hid->SetSupportedNpadIdType(AppletResourceUserId, ctbuf, 9).AssertOk();
        for(u32 i = 0; i < 8; i++)
        {
            Result rc = hid->SetNpadJoyAssignmentModeDual(i, AppletResourceUserId);
            if(rc.IsFailure()) break;
        }
        this->shmem = (InputMemory*)shm->GetAddress();
    }

    InputMemory *InputManager::GetInputMemory()
    {
        return this->shmem;
    }

    Player *InputManager::GetPlayer(Controller PlayerNo)
    {
        return new Player(this->shmem, PlayerNo);
    }
}