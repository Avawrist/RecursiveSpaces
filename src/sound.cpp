// ======================================================================
// Title: sound.cpp
// Description: The source file for game sounds
// ======================================================================

#include "sound.hpp"

///////////////////////////
// Struct SoundInterface //
///////////////////////////

SoundInterface::SoundInterface()
{
    interface_p    = nullptr;
    master_voice_p = nullptr;
    
    if(soundInterfaceLoadXAudio2())
    {
	// Init COM library
	CoInitializeEx(nullptr, COINIT_MULTITHREADED | COINIT_DISABLE_OLE1DDE);
	
	// Init xaudio interface
        XAudio2Create(&interface_p, 0, XAUDIO2_DEFAULT_PROCESSOR);
	if(!interface_p)
	{
	    OutputDebugStringA("Failed to initialize XAudio2 interface.\n");
	}
	
	// Init sound master
	interface_p->CreateMasteringVoice(&master_voice_p);
	if(!master_voice_p)
	{
	    OutputDebugStringA("Failed to initialize XAudio2 Master Voice\n");
	}

	// Start XAudio2 engine
	interface_p->StartEngine();
    }
    else
    {
	OutputDebugStringA("Failed to load XAudio2 dynamic library.\n");
    }    
}

SoundInterface::~SoundInterface()
{
    // Uninitialize COM library
    CoUninitialize();

    // Stop the XAudio2 engine
    interface_p->StopEngine();
}

int soundInterfaceLoadXAudio2()
{
    // Windows 10 
    HMODULE x_audio_2_libs = LoadLibraryA("xaudio2_9.dll");
    if(x_audio_2_libs)
    {
	OutputDebugStringA("SUCCESS: Loaded xaudio2_9.dll.\n");
	return 1;
    }

    x_audio_2_libs = LoadLibraryA("xaudio2_9redist.dll");
    if(x_audio_2_libs)
    {
	OutputDebugStringA("SUCCESS: Loaded xaudio2_9redist.dll.\n");
	return 1;
    }

    // Windows 8
    x_audio_2_libs = LoadLibraryA("xaudio2_8.dll");
    if(x_audio_2_libs)
    {
	OutputDebugStringA("SUCCESS: Loaded xaudio2_8.dll.\n");
	return 1;
    }

    x_audio_2_libs = LoadLibraryA("xaudio2_8redist.dll");
    if(x_audio_2_libs)
    {
	OutputDebugStringA("SUCCESS: Loaded xaudio2_8redist.dll.\n");
	return 1;
    }

    // DirectX SDK
    x_audio_2_libs = LoadLibraryA("xaudio2_7.dll");
    if(x_audio_2_libs)
    {
	OutputDebugStringA("SUCCESS: Loaded xaudio2_7.dll.\n");
	return 1;
    }

    x_audio_2_libs = LoadLibraryA("xaudio2_7redist.dll");
    if(x_audio_2_libs)
    {
	OutputDebugStringA("SUCCESS: Loaded xaudio2_7redist.dll.\n");
	return 1;
    }
    
    return 0;
}

//////////////////
// Struct Sound //
//////////////////

Sound::Sound(c_char* wav_path, SoundInterface& soundInterface)
{
    // Load audio data to buffer
    if(!soundLoadWav(this, wav_path))
    {
	OutputDebugStringA("\nERROR: Failed to load .wav file.\n");
    }

    // Create source voice
    soundInterface.interface_p->CreateSourceVoice(&source_voice_p,
						  &waveFormat);
    if(!source_voice_p)
    {
	OutputDebugStringA("ERROR: Failed to create source voice.\n");
    }
}

Sound::~Sound()
{
    soundStop(this);
}

int soundLoadWav(Sound* sound, c_char* wav_path)
{
    // Open file
    FILE* file_p = NULL;
    fopen_s(&file_p, wav_path, "rb");
    if(!file_p) {return 0;}
    
    // Verify first ChunkID is "RIFF" format
    char chunk_ID[5];
    chunk_ID[4] = '\0';
    fread(chunk_ID, sizeof(char), 4, file_p);
    if(strcmp(chunk_ID, "RIFF") != 0) {return 0;}

    //////////////////////////////////
    // Populate WAVEFORMATEX struct //
    //////////////////////////////////

    // Skip ChunkSize (4), Format (4), SubchunkID (4),
    // Subchunk1Size (4), AudioFormat (2)
    fseek(file_p, 18, SEEK_CUR);
    // Read NumChannels
    fread(&sound->waveFormat.nChannels, sizeof(shint), 1, file_p);
    // Read SampleRate
    fread(&sound->waveFormat.nSamplesPerSec, sizeof(uint), 1, file_p);
    // Read ByteRate
    fread(&sound->waveFormat.nAvgBytesPerSec, sizeof(uint), 1, file_p);
    // Read BlockAlign
    fread(&sound->waveFormat.nBlockAlign, sizeof(shint), 1, file_p);
    // Read Bits Per Sample
    fread(&sound->waveFormat.wBitsPerSample, sizeof(shint), 1, file_p);
    // FormatTag
    sound->waveFormat.wFormatTag = WAVE_FORMAT_PCM;
    // cbSize
    sound->waveFormat.cbSize = 0;

    ////////////////////////////////////
    // Populate XAUDIO2_BUFFER struct //
    ////////////////////////////////////
    
    // Skip Subchunk2ID (4)
    fseek(file_p, 4, SEEK_CUR);
    // Read Subchunk2Size
    fread(&sound->buffer.AudioBytes, sizeof(uint), 1, file_p);
    // Read Data
    BYTE *buffer = new BYTE[sound->buffer.AudioBytes]; 
    fread((void *)buffer, sizeof(char), sound->buffer.AudioBytes, file_p);
    sound->buffer.pAudioData = buffer;
    sound->buffer.Flags = XAUDIO2_END_OF_STREAM;
    
    // Close file
    fclose(file_p);
    
    return 1;
}

void soundPlay(Sound* sound)
{
    // Check buffer state
    XAUDIO2_VOICE_STATE state_p; 
    sound->source_voice_p->GetState(&state_p);
    if(!&state_p)
    {
	OutputDebugStringA("\nERROR: Failed to get buffer state from XAudio2.\n");
	return;
    }

    if(state_p.BuffersQueued == 0)
    {
	// Submit buffer to the source voice
	HRESULT hr;
	hr = sound->source_voice_p->SubmitSourceBuffer(&(sound->buffer));
	if(FAILED(hr))
	{
	    OutputDebugStringA("\nERROR: Failed to submit audio buffer to source voice.\n");
	    return;
	}
    }
    
    sound->source_voice_p->Start(0);
}

void soundPause(Sound* sound)
{
    sound->source_voice_p->Stop();
}

void soundStop(Sound* sound)
{
    sound->source_voice_p->Stop();
    sound->source_voice_p->FlushSourceBuffers();
}

void soundSetVolume(Sound* sound, int volume)
{
    // Takes a value between 0 and 100, converts it to a float scale of 0 to 1.
    float value = volume * 0.01; 
    sound->source_voice_p->SetVolume(value);
}

////////////////////////
// Struct SoundStream //
////////////////////////

SoundStream::SoundStream(c_char* wav_path, SoundInterface& soundInterface)
{
    // Init member variables
    bytes_read = 0;
    cw_buffer  = 0;
    file_p = nullptr;
    
    //Populate XAudio 2 structs:
    if(!soundStreamReadWavHeader(this, wav_path))
    {
	OutputDebugStringA("ERROR: Unable to populate XAudio2 structs.\n");
    }
    
    // Create source voice
    soundInterface.interface_p->CreateSourceVoice(&source_voice_p,
						  &waveFormat);
    if(!source_voice_p)
    {
	OutputDebugStringA("ERROR: Failed to create source voice.\n");
    }
}

SoundStream::~SoundStream()
{
    fclose(file_p);
    soundStreamStop(this);
}

int soundStreamReadWavHeader(SoundStream* soundStream, c_char* wav_path)
{
    // Open file
    fopen_s(&soundStream->file_p, wav_path, "rb");
    if(!soundStream->file_p) {return 0;}
    
    // Verify first ChunkID is "RIFF" format
    char chunk_ID[5];
    chunk_ID[4] = '\0';
    fread(chunk_ID, sizeof(char), 4, soundStream->file_p);
    if(strcmp(chunk_ID, "RIFF") != 0) {return 0;}

    //////////////////////////////////
    // Populate WAVEFORMATEX struct //
    //////////////////////////////////

    // Skip ChunkSize (4), Format (4), SubchunkID (4),
    // Subchunk1Size (4), AudioFormat (2)
    fseek(soundStream->file_p, 18, SEEK_CUR);
    // Read NumChannels
    fread(&soundStream->waveFormat.nChannels, sizeof(shint), 1, soundStream->file_p);
    // Read SampleRate
    fread(&soundStream->waveFormat.nSamplesPerSec, sizeof(uint), 1, soundStream->file_p);
    // Read ByteRate
    fread(&soundStream->waveFormat.nAvgBytesPerSec, sizeof(uint), 1, soundStream->file_p);
    // Read BlockAlign
    fread(&soundStream->waveFormat.nBlockAlign, sizeof(shint), 1, soundStream->file_p);
    // Read Bits Per Sample
    fread(&soundStream->waveFormat.wBitsPerSample, sizeof(shint), 1, soundStream->file_p);
    // FormatTag
    soundStream->waveFormat.wFormatTag = WAVE_FORMAT_PCM;
    // cbSize
    soundStream->waveFormat.cbSize = 0;

    ////////////////////////////////////
    // Populate XAUDIO2_BUFFER struct //
    ////////////////////////////////////
    
    // Skip Subchunk2ID (4)
    fseek(soundStream->file_p, 4, SEEK_CUR);
    // Read Subchunk2Size into total_bytes
    fread(&soundStream->total_bytes, sizeof(uint), 1, soundStream->file_p);
    // Set AudioBytes to correct buffer size
    soundStream->buffer.AudioBytes = BUFFER_SIZE;

    // Upate bytes_read to account for header
    soundStream->bytes_read = 44;

    return 1;
}

void soundStreamUpdate(SoundStream* soundStream)
{
    // Get sound state from XAudio2
    XAUDIO2_VOICE_STATE state_p; 
    soundStream->source_voice_p->GetState(&state_p);
    if(!&state_p)
    {
	OutputDebugStringA("\nERROR: Failed to get buffer state from XAudio2.\n");
	return;
    }
    
    // Fill and submit buffers if they are unqueued
    if(state_p.BuffersQueued < NUM_BUFFERS && soundStream->buffer.Flags != XAUDIO2_END_OF_STREAM)
    {
	// Offset file_p by amount of data already read
	fseek(soundStream->file_p, soundStream->bytes_read, SEEK_SET);
	
	// Determine amount to read on this pass
	uint bytes_left = soundStream->total_bytes - soundStream->bytes_read;
	uint bytes_to_read = min(bytes_left, BUFFER_SIZE);

	// If final pass, fill buffer with 0s and mark end of stream
	if(bytes_to_read < BUFFER_SIZE)
	{
	    memset(soundStream->buffers[soundStream->cw_buffer], 0, BUFFER_SIZE);
	    soundStream->buffer.Flags = XAUDIO2_END_OF_STREAM;
	}
	
        // Fill unqueued buffer with data from the file
	fread((void *)&(soundStream->buffers[soundStream->cw_buffer]),
	      bytes_to_read, 1, soundStream->file_p);

	// Submit buffer to queue
	soundStream->buffer.pAudioData = soundStream->buffers[soundStream->cw_buffer];
        HRESULT hr;
	hr = soundStream->source_voice_p->SubmitSourceBuffer(&(soundStream->buffer));
	if(FAILED(hr))
	{
	    OutputDebugStringA("\nERROR: Failed to submit audio buffer to source voice.\n");
	    return;
	}

	// Update bytes_read for next pass
	soundStream->bytes_read += bytes_to_read;

	// Update current write buffer for next pass
	soundStream->cw_buffer++;
	soundStream->cw_buffer %= NUM_BUFFERS;
    }
}

void soundStreamPause(SoundStream* soundStream)
{
    // TODO: test
    soundStream->source_voice_p->Stop();
}

void soundStreamStop(SoundStream* soundStream)
{
    // TODO: test
    soundStream->source_voice_p->Stop();
    // Clear XAudio2's queued buffers
    soundStream->source_voice_p->FlushSourceBuffers();
    // Clear our temp buffers
    for(int i = 0; i < NUM_BUFFERS; i++)
    {
	memset(soundStream->buffers[i], 0, BUFFER_SIZE);
    }
    // Reset our tracking values
    soundStream->bytes_read = 0;
    soundStream->cw_buffer  = 0;
}

void soundSetVolume(SoundStream* soundStream, int volume)
{
    // Takes a value between 0 and 100, converts it to a float scale of 0 to 1.
    float value = volume * 0.01; 
    soundStream->source_voice_p->SetVolume(value);
}
