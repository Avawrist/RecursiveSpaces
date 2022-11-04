// ======================================================================
// Title: sound.cpp
// Description: The source file for game sounds
// ======================================================================

#include "sound.hpp"

////////////////////////
// SoundDevice Struct //
////////////////////////

SoundDevice::SoundDevice()
{
    // Get device
    LPDIRECTSOUND8 lpds;
    ds_device = DirectSoundCreate8(NULL, &lpds, NULL);
    // SetCooperativeLevel
}

SoundDevice::~SoundDevice()
{

}
