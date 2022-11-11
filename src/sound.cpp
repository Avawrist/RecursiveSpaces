// ======================================================================
// Title: sound.cpp
// Description: The source file for game sounds
// ======================================================================

#include "sound.hpp"

int loadXAudio2()
{
    HMODULE x_audio_2_libs = LoadLibraryA("xaudio2_9.dll");

    IXAudio2* pXAudio2 = nullptr;
    XAudio2Create(&pXAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
    
    return 0;
}
