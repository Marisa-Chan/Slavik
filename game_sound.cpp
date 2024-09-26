#include "system.h"
#include "game.h"

namespace Game {

void Engine::PlaySound(uint16_t SoundID, int Volume, int Pan, bool loop)
{
    auto it = Res.Sounds.find(SoundID);

    if (it == Res.Sounds.end())
    {
        it = Res.DynSounds.find(SoundID);

        if (it == Res.DynSounds.end())
        {
            printf("PlaySound NOT FOUND %d\n", SoundID);
            return;
        }
    }

    float speed = 1.0;
    if (SoundID >= 32 || 
        SoundID == 2 ||
        SoundID == 3 ||
        SoundID == 6 ||
        SoundID == 7 ||
        SoundID == 8)
    {
        switch(System::rand() % 3)
        {
            case 1:
                speed = 19050.0 / 22050.0;
                break;
            case 2:
                speed = 25050.0 / 22050.0;
                break;
            default:
                break;
        }
    }
    
    _mixer.PlaySample(it->second, Simix::DCBToVol(Volume), Pan, speed);
}

void Engine::SoundPlaySpeech(int32_t id)
{
    printf("Incomplete %s\n", __PRETTY_FUNCTION__);
}    

bool Engine::FUN_00429c28(int32_t id)
{
    printf("Incomplete %s\n", __PRETTY_FUNCTION__);
    /*int iVar1;
    undefined4 local_1c;
    uint local_18;
    IDirectSoundBuffer *local_14;
    
    local_14 = SoundBuffers[param_1 + -1];
    iVar1 = (*(code *)local_14->vtbl->GetStatus)(local_14,&local_18);
    if ((iVar1 == 0) && ((local_18 & 1) != 0)) {
        local_1c = 1;
    }
    else {
        local_1c = 0;
    }*/
    return false;
}
    
}