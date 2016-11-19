#include "FileIO\FileIO.hpp" // Easy reading and writing to files
#include "AudioSystem.hpp"   // System for managing sounds
#include "AudioFile.hpp"     // Sounds
#include <ostream>           // cout
#include <conio.h>           // _kbhit()
#include <istream>           // cin
#include <vector>            // vector and pair


// Typedefs
typedef std::vector<std::pair<char, NumpadSupreme::AudioFile *>> SoundVec;


// Parses input provided by two strings, adding a new char and sound pair
// to the provided vector initialized with the provided audiosystem
static void ParseInput(
  std::string strKey
  , std::string filepath
  , SoundVec *sounds
  , NumpadSupreme::AudioSystem *audioSystem)
{
  // Create file and associate it with a key, add to vector of possibilities
  NumpadSupreme::AudioFile *newAF = new NumpadSupreme::AudioFile(filepath);
  audioSystem->PreloadFile(*newAF);
  std::pair<char, NumpadSupreme::AudioFile *> keyPair(strKey[0], newAF);
  sounds->push_back(keyPair);
}


// Takes a vector of sounds to populate and an audiosystem to initialize
// the sounds with.
static void PromptInput(SoundVec *sounds, NumpadSupreme::AudioSystem *audioSystem)
{// While we are still accepting inputs
  while (true)
  {
    // Parse input pairs!
    try
    {
      // Ask user for a key, get character input
      //!TODO: Handle spaces in filepath!
      std::string inputKey;
      std::string inputFilepath;
      std::cout << "Please bind a sound to a key! Format: <key> <filepath>\n";
      std::cout << "When you are done binding keys, type 'done'.\n";

      // When done, type 'done'
      std::cin >> inputKey;
      if (inputKey == "done")
        break;
      std::cin >> inputFilepath;
      if (inputFilepath == "done")
        break;

      // Parse strings
      ParseInput(inputKey, inputFilepath, sounds, audioSystem);
    }
    catch (std::exception e)
    {
      std::cout << "Invalid input!" << std::endl;
      std::cout << e.what() << std::endl;
      throw e;
    }
  }
}


// entry point
int main(int arc, char** argv)
{
  // Audio system setup
  NumpadSupreme::AudioSystem audioSystem(10);
  SoundVec sounds;

  // Config file handling:
  // ...

  // Handle input
  PromptInput(&sounds, &audioSystem);

  // Announce we are ready to jam out
  std::cout << "== [ " << sounds.size() << " sound(s) loaded and ready! ] ==\n";

  // Primary loop
  while (true)
  {
    // Check if keyboard is hit
    if (_kbhit())
    {
      // Get input
      char input = _getch();
      
      // For all possible sounds
      for (unsigned int i = 0; i < sounds.size(); ++i)
      {
        // Check to see if we have corrseponding key
        if (input == sounds[i].first)
        {
          // Play sound
          audioSystem.PlayFile(*sounds[i].second);
          audioSystem.SetPaused(*sounds[i].second, false);
        }
      }
    }

    // Update our audio system
    audioSystem.Update();
  }


  //!TODO: Cleanup allocated memory
  return 0;
}


