/*
 * SDLGP_Editor.hpp
 *
 *  Created on: Apr 6, 2020
 *      Author: Karina
 */

#ifndef SRC_EDITOR_SDLGP_EDITOR_HPP_
#define SRC_EDITOR_SDLGP_EDITOR_HPP_

#include "SDLGraphicsProgram.hpp"

class SDLGP_Editor : SDLGraphicsProgram
{
public:

	SDLGP_Editor(int gc);
	~SDLGP_Editor();

	void update();
	void render();
	void loop();

    void initLevelLoadingEditor();

    /**
     * edits the tile at the given position to be of new type blockStr
     * @param blockPos the position of the tile we will edit.
     * @param blockStr a strig correlating the new type of block it will be.
     */
	void editTile(Vector3D blockPos, std::string blockStr);

	void editTileBreakout(Vector3D blockPos, std::string blockStr);

    void editTilePlatformer(Vector3D blockPos, std::string blockStr);

	void editTileGalaga(Vector3D blockPos, int badyTypeInt);

    //helper for editing the editorTile files in real time.
    void levelHelper(int lvlInt);

private:
    int gameCode = 0;

    //level editor variables
    int edt_currLevelIndex;
    GameObject edt_cursor;
    Vector3D edt_cursorBlockPos = Vector3D(0,0);

    std::vector<Level *> edt_levels_platformer;
    std::vector<BreakoutLevel *> edt_levels_breakout;
    std::vector<GalagaLevel *> edt_levels_galaga;
    std::vector<Textbox> edt_menuTexts;

};



#endif /* SRC_EDITOR_SDLGP_EDITOR_HPP_ */
