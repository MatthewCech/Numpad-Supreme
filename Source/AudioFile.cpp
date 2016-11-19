/*!***************************************************************************
@file    AudioFile.cpp
@author  Reverie Wisp
@par     Email: contact@rw0.pw
@date    11/19/2016

@brief
Lifted from my ASCIIPlayer project.

Usable under the "Don't be a Jerk" license, included in repo. Please respect
the individual requests of the authors of the externally included libraries.

FMOD Sound System, copyright © Firelight Technologies Pty, Ltd., 1994-2015.
*****************************************************************************/
#include "AudioFile.hpp"


namespace NumpadSupreme
{
  // Static init.
  APUnique AudioFile::uniqueID_ = 0; // ID for lookup in the audio system


  // Constructor and Destructor
  AudioFile::AudioFile(std::string path) 
    : path_(path)
    , fileID_(uniqueID_++)
    , loadedObjects_() // Assigned via lazy init later
  {  }


  // Destructor
  AudioFile::~AudioFile()
  {  }


  // Operator == overload
  bool AudioFile::operator==(const AudioFile &rhs)
  {
    if (fileID_ == rhs.fileID_)
      return true;
    return false;
  }

  // Get the value associated with the Audiosystem with the ID in question
  AudioHandleWrapper *AudioFile::get(APUnique apu)
  {
    return &loadedObjects_[apu];
  }
}
