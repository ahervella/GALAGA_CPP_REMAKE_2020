#ifndef RESOURCE_MANAGER_HPP
#define RESOURCE_MANAGER_HPP

//if we're managing resources here?
#include <map>
#include <string>
#include <memory>
#include <fstream>
#include <SDL.h>
//for mac users
//#include "SDL2_image/SDL_image.h"
//For Windows
//#include "SDL2/SDL_image.h"
#include <SDL_image.h>
#include <SDL_mixer.h>

/**
 * A class for managing game resources.  Allows access to resources
 * and makes sure there is only one copy of each resource loaded at a
 * time.
 */
class ResourceManager {
private:
  ResourceManager();  // Private Singleton
  ResourceManager(ResourceManager const&) = delete; // Avoid copy constructor.
  void operator=(ResourceManager const&) = delete; // Don't allow copy assignment.
  ResourceManager(ResourceManager const&&) = delete; // Avoid move constructor.
  void operator=(ResourceManager const&&) = delete; // Don't allow move assignment.

  static ResourceManager* singleton;

  std::map<std::string, std::shared_ptr<SDL_Texture*>> textureResourceTable;
  std::map<std::string, std::shared_ptr<Mix_Chunk*>> sfxResourceTable;
  std::map<std::string, std::shared_ptr<Mix_Music*>> musicResourceTable;
  std::map<std::string, std::shared_ptr<std::ifstream*>> configFileResourceTable;
  
  //Custom delete functions for shared pointers that use SDL functions
  void deleteTexture(SDL_Texture* texture);
  void deleteMixChunk(Mix_Chunk* chunk);
  void deleteMixMusic(Mix_Music* music);

public:
  
  /**
   * Static function that returns the ResourceManager singleton. Initializes the singleton if it doesn't exist yet.
   */
  static ResourceManager *getInstance();
  

  /**
   * Initializes the ResourceManager singleton
   *
   * @return return code (0 is good)
   */
  int startUp();
  
  // 'equivalent' to our destructor
  /**
   * Destroys the ResourceManager singleton
   *
   * @return return code (0 is good)
   */
  int shutDown();


  /**
   * Gets the texture for a given image file path
   *
   * @param ren: The renderer to create the texture on
   * @param fileString: the file path of the image file we want a texture for
   * @return A shared pointer to the texture corresponding to the image file
   */
  std::shared_ptr<SDL_Texture*> getTextureResource(SDL_Renderer * ren, std::string fileString);

  /**
   * Gets the sound effect for a given sound effect file path
   *
   * @param fileString: The file path to the sound effect file we want a Mix_Chunk* for
   * @return A shared pointer to the Mix_Chunk* corresponding to the sound effect file
   */
  std::shared_ptr<Mix_Chunk*> getSFXResource(std::string fileString);

  /**
   * Gets the music for a given music file path
   *
   * @param fileString: The file path to the music file we want a Mix_Music* for
   * @return A shared pointer to the Mix_Music* corresponding to the sound effect file
   */
  std::shared_ptr<Mix_Music*> getMusicResource(std::string fileString);

  /**
   * Gets the level config for a given config file path
   *
   * @param fileString: The file path to the config file we want a SDL_RWops* for
   * @return A shared pointer to the SDL_RWops* corresponding to the sound effect file
   */
  std::shared_ptr<std::ifstream*> getConfigFileResource(std::string fileString);


  /**
   * Deletes a texture resource if the resource manager is the only one who has a shared pointer to the given resource.
   *
   * @param location: filename that specifies the texture that should be deleted
   */
  void deleteTextureResource(std::string location);
  
  /**
   * Deletes a Mix_Chunk resource if the resource manager is the only one who has a shared pointer to the given resource.
   *
   * @param location: filename that specifies the Mix_Chunk that should be deleted
   */
  void deleteSFXResource(std::string location);

  /**
   * Deletes a Mix_Music resource if the resource manager is the only one who has a shared pointer to the given resource.
   *
   * @param location: filename that specifies the Mix_Music that should be deleted
   */
  void deleteMusicResource(std::string location);

  /**
   * Deletes a SDL_RWops resource if the resource manager is the only one who has a shared pointer to the given resource.
   *
   * @param location: filename that specifies the SDL_RWops that should be deleted
   */
  void deleteConfigFileResource(std::string location);

};

#endif
