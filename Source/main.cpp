#include "FileIO\FileIO.hpp" // Easy reading and writing to files
#include "AudioSystem.hpp"   // System for managing sounds
#include "AudioFile.hpp"     // Sounds
#include <ostream>           // cout
#include <conio.h>           // _kbhit()
#include <istream>           // cin
#include <vector>            // vector and pair


// Typedefs and Defines
typedef std::vector<std::pair<char, NumpadSupreme::AudioFile *>> SoundVec;
#define SPACE_CHAR ' '
#define NEWLINE_CHAR '\n'
#define RETURN_CHAR '\r'
#define TAB_CHAR '\t'


// Checks to see if a character is considered whitespace.
// If so, returns true, otherwise false.
static bool IsWhitespace(char toCheck)
{
  if (toCheck == SPACE_CHAR
    || toCheck == NEWLINE_CHAR
    || toCheck == RETURN_CHAR
    || toCheck == TAB_CHAR)
    return true;

  return false;
}


// takes a string reference as an argument and returns said refernce when done.
// The string will have all whitespace removed from it.
static std::string &TrimStr(std::string &str)
{
  // Trim leading and trailing spaces from filepath;
  // For each leading index. If the index is *still* a space or whitespace, trim.
  while (IsWhitespace(str[0]))
    str = str.substr(1);

  // Trailing trimming mimics leading
  while (IsWhitespace(str[str.size() - 1]))
    str = str.substr(0, str.size() - 1);

  return str;
}


// Parses input provided by two strings, adding a new char and sound pair
// to the provided vector initialized with the provided audiosystem
static void ParseInput(
  std::string strKey
  , std::string filepath
  , SoundVec *sounds
  , NumpadSupreme::AudioSystem *audioSystem)
{
  // Create file and associate it with a key, add to vector of possibilities
  // only if we managed to load it.
  TrimStr(filepath);
  NumpadSupreme::AudioFile *newAF = new NumpadSupreme::AudioFile(filepath);
  bool loaded = audioSystem->PreloadFile(*newAF);
  if (loaded)
  {
    std::pair<char, NumpadSupreme::AudioFile *> keyPair(strKey[0], newAF);
    sounds->push_back(keyPair);
  }
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
      std::getline(std::cin, inputFilepath);
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


// Parse file takes in the config file filepath, sound vector to populate with
// pairs, and the audiosystem to initialize the sounds with.
// Returns bool that tells us if we prompt for input afterwards.
static bool ParseFile(
  char* filepath
  , SoundVec *sounds
  , NumpadSupreme::AudioSystem *audioSystem)
{
  // Open config file specified. If invalid, alert user and prepare to parse.
  FileUtil::File configFile (filepath);
  if (configFile.GetLineCount() < 2)
  {
    std::cout << "WARNING: File at " << std::string(filepath) << " is not valid!\n";
    return true;
  }

  // For each line, 
  bool parseAfter = true;
  for (unsigned int i = 0; i < configFile.GetLineCount(); ++i)
  {
    // If it is the first line, parse out of we are prompting after file is read.
    std::string line = configFile[i];
    if (i == 0)
    {
      parseAfter = line == "true" ? true : false;
      continue;
    }

    // Otherwise, split at the ':' and parse the key and filepath.
    std::string lval = line.substr(0, 1);                       // the key
    std::string rval = line.substr(line.find_last_of(":") + 1); // audio filepath

    // Create audio pair
    ParseInput(lval, rval, sounds, audioSystem);
  }

  return parseAfter;
}


/* Application entry point, args as follows:
 * ===============================
 * Program Usage:
 *
 * <applicationName.exe> <optional config file filepath>
 *
 * If no config file is specified, you will be prompted for key bindings.
 *
 * ===============================
 * Config File Format:
 * <bool: prompt for key bindings after parse?>
 * <char: key to press> : <audio filepath>
 * 
 * example:
 * true
 * 1 : ../Test.wav
 */
int main(int argc, char** argv)
{
  // Audio system setup
  NumpadSupreme::AudioSystem audioSystem(100);
  bool promptForInput = true;
  SoundVec sounds;

  // Read config file and determine if we prompt for input.
  if (argc >= 2)
    promptForInput = ParseFile(argv[1], &sounds, &audioSystem);

  // Handle input if we should.
  if(promptForInput)
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

