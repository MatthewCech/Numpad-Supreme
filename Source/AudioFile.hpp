/*!***************************************************************************
@file    AudioFile.hpp
@author  Reverie Wisp
@par     Email: contact@rw0.pw
@date    11/19/2016

@brief
Lifted from my ASCIIPlayer project.

Usable under the "Don't be a Jerk" license, included in repo. Please respect
the individual requests of the authors of the externally included libraries.

FMOD Sound System, copyright © Firelight Technologies Pty, Ltd., 1994-2015.
*****************************************************************************/
#pragma once
#include <string>
#include "Defines.hpp"
#include <unordered_map>



namespace NumpadSupreme
{
  class AudioSystem;

  struct AudioHandleWrapper
  {
    AudioHandleWrapper() : LoadedObject(nullptr), BoundToChannel(false) { }
    AudioHandle *LoadedObject;
    bool BoundToChannel;
  };

	class AudioFile
	{
    // Mark as friend to manage handles.
    friend class AudioSystem;

	public:
    // Constructor and Destructor
    AudioFile(std::string Path);
    ~AudioFile();

    // Operator overloads
    bool operator==(const AudioFile &rhs);

	private:
    // Private methods
    AudioHandleWrapper *get(APUnique apu);

    // Variables
    std::string path_;                                                 // Filepath of actual song
    APUnique fileID_;                                                  // Unique song ID.
    std::unordered_map<APUnique, AudioHandleWrapper> loadedObjects_;   // Handle to the loaded object.

    // Static Variables
    static APUnique uniqueID_; // ID for lookup in the audio system
	};
}
