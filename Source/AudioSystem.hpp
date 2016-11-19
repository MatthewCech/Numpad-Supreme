/*!***************************************************************************
@file    AudioSystem.hpp
@author  Reverie Wisp
@par     Email: contact@rw0.pw
@date    11/19/2016

@brief
Lifted from my ASCIIPlayer project.

Usable under the "Don't be a Jerk" license, included in repo. Please respect
the individual requests of the authors of the externally included libraries.

FMOD Sound System, copyright © Firelight Technologies Pty, Ltd., 1994-2015.
*****************************************************************************/
// Handles the backend, playing, loading, and unloading files.
#pragma once
#include <unordered_map>
#include "AudioFile.hpp"
#include "Defines.hpp"



namespace NumpadSupreme
{
  class AudioSystem
  {
  public:
    // Constructor
    AudioSystem(int numChannels = 10);
    ~AudioSystem();

    // Member Functions
    // General Upkeep
    bool Update();

    // Loading
    bool PreloadFile(AudioFile &audioFile);
    void UnloadFile(AudioFile &audioFile);

    // Basic Play/Pause
    AudioFile *AudioSystem::PreloadFile(const std::string filepath);
    void PlayFile(AudioFile &audioFile, bool playing = false);
    void SetPaused(AudioFile &audioFile, bool pausedState);
    void StopFile(AudioFile &audioFile);
    void SetMasterVolume(float f);
    float GetMasterVolume() const;

    // Info about song
    bool IsPlaying(AudioFile &audioFile);
    unsigned int GetLength(AudioFile &audioFile) const;
    unsigned int GetCurrentPosition(AudioFile &audioFile);
    std::string GetFilename(const AudioFile &audioFile) const;
    std::string GetFilepath(const AudioFile &audioFile) const;
    void FillWithAudioData(float *arr, int numVals, int channelOffset, FMOD_DSP_FFT_WINDOW window, AudioDataStyle style);

  private:
      //////////////////////////
     // Private member funcs //
    //////////////////////////
    void FCheck(const FMOD_RESULT &res) const;

      ///////////////
     // Variables //
    ///////////////
    // Audio System
    FMOD::System *fmodSystem_;   // Handle to the system we use to initialize and play sounds.
    Channelgroup *masterChannel_; // Master channel to shove things into for volume reasons.

    // Additional FMOD info
    int              numdrivers_;  // Number of drivers in the system (audio cards)
    char             name_[256];   // The name of the driver.
    FMOD_CAPS        caps_;        // The capabilities of the soundcard.
    unsigned int     version_;     // Version number.
    FMOD_SPEAKERMODE speakermode_; // The speaker mode- 5.1, 7.1, etc.

    // Active Handles map:
    std::unordered_map<APUnique, ChannelHandle> channelHandles_;

    // ID Tracking
    APUnique ID_;
    static APUnique AudioSystemIDIncrement;
  };
}