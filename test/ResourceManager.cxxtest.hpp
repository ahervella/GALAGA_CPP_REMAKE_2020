#include <cxxtest/TestSuite.h>
#include "ResourceManager.hpp"


//Because of multiple definition errors with SDL, I'm manually porting over getResourcePath for the time being
std::string getResourcePathClone(const std::string &subDir = ""){
    //We need to choose the path separator properly based on which
    //platform we're running on, since Windows uses a different
    //separator than most systems
#ifdef _WIN32
    const char PATH_SEP = '\\';
#else
    const char PATH_SEP = '/';
#endif
    //This will hold the base resource path: Lessons/res/
    //We give it static lifetime so that we'll only need to call
    //SDL_GetBasePath once to get the executable path
    static std::string baseRes;
    if (baseRes.empty()){
        //SDL_GetBasePath will return NULL if something went wrong in retrieving the path
        char *basePath = SDL_GetBasePath();
        if (basePath){
            baseRes = basePath;
            SDL_free(basePath);
        }
        else {
            std::cerr << "Error getting resource path: " << SDL_GetError() << std::endl;
            return "";
        }
        //We replace the last bin/ with res/ to get the the resource path
        size_t pos = baseRes.rfind("bin");
        baseRes = baseRes.substr(0, pos) + "res" + PATH_SEP;
    }
    //If we want a specific subdirectory path in the resource directory
    //append it to the base path. This would be something like Lessons/res/Lesson0
    return subDir.empty() ? baseRes : baseRes + subDir + PATH_SEP;
}
class ResourceManagerTestSuite: public CxxTest::TestSuite
{
public:
	std::string musicFile1 = getResourcePathClone() + "415804__sunsai__mushroom-background-music (1).wav";
	std::string sfx1 = getResourcePathClone() + "219619__ani-music__pew-pew-sound-effect-peww1.wav";
	std::string sfx2 = getResourcePathClone() + "459145__mattix__retro-pew-shot-01.wav";
	ResourceManager* rm = ResourceManager::getInstance();

	//Testing usage of resource manager (Mix_Music)
	void testResourceManagerAllocationsMusic() {
		std::shared_ptr<Mix_Music*> music = rm->getMusicResource(musicFile1);
		//Use count should be two (in rm map and the one returned by getMusicResource
		TS_ASSERT_EQUALS(music.use_count(), 2);
		std::shared_ptr<Mix_Music*> music2 = rm->getMusicResource(musicFile1);
		TS_ASSERT_EQUALS(music2.use_count(), music.use_count());
		//Should only increment use count by 1 now
		TS_ASSERT_EQUALS(music2.use_count(), 3);

		//Reference count drops, but we shouldn't delete the music file just yet
		music2.reset();
		TS_ASSERT_EQUALS(music.use_count(), 2);
		rm->deleteMusicResource(musicFile1);
		TS_ASSERT_EQUALS(music.use_count(), 2);

		//This should delete the music resource, so reallocating it should give us a reference count of two on the resource after the deletion
		music.reset();
		rm->deleteMusicResource(musicFile1);
		music = rm->getMusicResource(musicFile1);
		TS_ASSERT_EQUALS(music.use_count(), 2);
	}

	//Testing usage of resource manager (Mix_Chunk)
	void testResourceManagerAllocationsChunk() {
		std::shared_ptr<Mix_Chunk*> sfx = rm->getSFXResource(sfx1);
		//Use count should be two (in rm map and the one returned by getMusicResource
		TS_ASSERT_EQUALS(sfx.use_count(), 2);
		std::shared_ptr<Mix_Chunk*> sfx2 = rm->getSFXResource(sfx1);
		TS_ASSERT_EQUALS(sfx2.use_count(), sfx.use_count());
		//Should only increment use count by 1 now
		TS_ASSERT_EQUALS(sfx2.use_count(), 3);

		//Reference count drops, but we shouldn't delete the music file just yet
		sfx2.reset();
		TS_ASSERT_EQUALS(sfx.use_count(), 2);
		rm->deleteSFXResource(sfx1);
		TS_ASSERT_EQUALS(sfx.use_count(), 2);

		//This should delete the music resource, so reallocating it should give us a reference count of two on the resource after the deletion
		sfx.reset();
		rm->deleteSFXResource(sfx1);
		sfx = rm->getSFXResource(sfx1);
		TS_ASSERT_EQUALS(sfx.use_count(), 2);
	}

};


