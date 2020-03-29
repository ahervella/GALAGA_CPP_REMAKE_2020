#include "ResourceManager.hpp"
#include <iostream>

// TODO: implement ResourceManager


ResourceManager* ResourceManager::singleton;

ResourceManager::ResourceManager(){

}

ResourceManager* ResourceManager::getInstance() {
    if (!singleton) {
    	ResourceManager::singleton = new ResourceManager;
    }

    return ResourceManager::singleton;
}



int ResourceManager::startUp(){
	if(ResourceManager::singleton) {
		return 0;
	}
    ResourceManager::singleton = new ResourceManager;
    return 0;
}

int ResourceManager::shutDown(){
    delete ResourceManager::singleton;
    return 0;
}

std::shared_ptr<SDL_Texture*> ResourceManager::getTextureResource(SDL_Renderer * ren, std::string fileString){
    //check if file already in table
    if (textureResourceTable.find(fileString) == textureResourceTable.end()){
        SDL_Texture* texture = IMG_LoadTexture(ren, fileString.c_str());
        if(!texture) {
        	std::cout << SDL_GetError() << std::endl;
        }
        std::shared_ptr<SDL_Texture*> sharedTexture = std::make_shared<SDL_Texture*>(texture);
    	//std::shared_ptr<SDL_Texture*> sharedTexture(IMG_LoadTexture(ren, fileString.c_str()), deleteTexture);
        std::cout << "Init use count: " << sharedTexture.use_count() << std::endl;
        textureResourceTable.insert(std::pair<std::string, std::shared_ptr<SDL_Texture*>>(fileString, sharedTexture));
        return sharedTexture;
    }
    return textureResourceTable.find(fileString)->second;
}

//
std::shared_ptr<Mix_Chunk*> ResourceManager::getSFXResource(std::string fileString) {
	if(sfxResourceTable.find(fileString) == sfxResourceTable.end()) {
		Mix_Chunk* chunk = Mix_LoadWAV(fileString.c_str());
		std::shared_ptr<Mix_Chunk*> sharedChunk = std::make_shared<Mix_Chunk*>(chunk);
		sfxResourceTable.insert(std::pair<std::string, std::shared_ptr<Mix_Chunk*>>(fileString, sharedChunk));
		return sharedChunk;
	}
	return sfxResourceTable.find(fileString)->second;
}

std::shared_ptr<Mix_Music*> ResourceManager::getMusicResource(std::string fileString) {
	if(musicResourceTable.find(fileString) == musicResourceTable.end()) {
		Mix_Music* music = Mix_LoadMUS(fileString.c_str());
		std::shared_ptr<Mix_Music*> sharedMusic = std::make_shared<Mix_Music*>(music);
		musicResourceTable.insert(std::pair<std::string, std::shared_ptr<Mix_Music*>>(fileString, sharedMusic));
		return sharedMusic;
	}
	return musicResourceTable.find(fileString)->second;
}

std::shared_ptr<std::ifstream*> ResourceManager::getConfigFileResource(std::string fileString) {
	if(configFileResourceTable.find(fileString) == configFileResourceTable.end()) {
		std::ifstream* config = new std::ifstream(fileString.c_str(), std::ifstream::in);
		std::shared_ptr<std::ifstream*> sharedConfig = std::make_shared<std::ifstream*>(config);
		configFileResourceTable.insert(std::pair<std::string, std::shared_ptr<std::ifstream*>>(fileString, sharedConfig));
		return sharedConfig;
	}
	return configFileResourceTable.find(fileString)->second;
}


void ResourceManager::deleteTextureResource(std::string filename) {
	//If the resource manager has the only reference to the texture, delete it
	if(textureResourceTable.find(filename)->second.use_count() == 1) {
		textureResourceTable.find(filename)->second.reset();
		textureResourceTable.erase(filename);
	}
}

void ResourceManager::deleteSFXResource(std::string filename) {
	//If the resource manager has the only reference to the chunk, delete it
	if(sfxResourceTable.find(filename)->second.use_count() == 1) {
		sfxResourceTable.find(filename)->second.reset();
		sfxResourceTable.erase(filename);
	}
}

void ResourceManager::deleteMusicResource(std::string filename) {
	//If the resource manager has the only reference to the music, delete it
	if(musicResourceTable.find(filename)->second.use_count() == 1) {
		musicResourceTable.find(filename)->second.reset();
		musicResourceTable.erase(filename);
	}
}

void ResourceManager::deleteConfigFileResource(std::string filename) {
	//If the resource manager has the only reference to the ifstream, delete it
	std::cout<<"couintt: "<<configFileResourceTable.find(filename)->second.use_count()<<std::endl;
	if(configFileResourceTable.find(filename)->second.use_count() == 1) {
		//Close the ifstream
		(*(configFileResourceTable.find(filename)->second))->close();
		//Remove the last shared_ptr to the ifstream
		configFileResourceTable.find(filename)->second.reset();
		//Erase the entry for this filename from the configFileResourceTable
		configFileResourceTable.erase(filename);
	}
}

void ResourceManager::deleteTexture(SDL_Texture* texture) {
	std::cout << "Destroying Texture..." << std::endl;
	SDL_DestroyTexture(texture);
}

void ResourceManager::deleteMixChunk(Mix_Chunk* chunk) {
	std::cout << "Destroying Mix_Chunk..." << std::endl;
	Mix_FreeChunk(chunk);
}

void ResourceManager::deleteMixMusic(Mix_Music* music) {
	std::cout << "Destroying Mix_Music..." << std::endl;
	Mix_FreeMusic(music);
}
