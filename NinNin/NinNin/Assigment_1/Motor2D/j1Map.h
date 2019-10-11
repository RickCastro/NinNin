#ifndef __j1MAP_H__
#define __j1MAP_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "p2DynArray.h"
#include "j1Module.h"
#include "SDL/include/SDL.h"

// TODO 2: Create a struct to hold information for a TileSet
// Ignore Terrain Types and Tile Types for now, but we want the image!
// ----------------------------------------------------

struct Tileset {

	p2SString	name;
	p2SString	img_source;

	uint		tileWidth;
	uint		tileHeight;
	uint		spacing;
	uint		margin;

};
struct Layer {

	p2SString		name;

	uint			width;
	uint			height;

	p2List<uint>	data;

};


// TODO 1: Create a struct needed to hold the information to Map node
enum Map_Orientation
{
	orthogonal,
	isometric,
	staggered,
	hexagonal
};

enum Map_renderorder 
{
	right_down=0,
	right_up,
	left_down,
	left_up
};

struct Map{

	float				version;

	Map_Orientation		orientation;
	Map_renderorder		renderorder;

	uint				width;
	uint				height;
	uint				tilewidth;
	uint				tileheight;
	uint				nextobject;

	p2List<Tileset>		map_tileset;
	p2List<Layer>		map_layers;

};


// ----------------------------------------------------
class j1Map : public j1Module
{
public:

	j1Map();

	// Destructor
	virtual ~j1Map();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	// Called each loop iteration
	void Draw();

	// Called before quitting
	bool CleanUp();

	// Load new map
	bool Load(const char* path);

private:

	//Load and fill map data
	void FillMap(const pugi::xml_document& document);

	//Load and fill tilset
	void FillTilset(const pugi::xml_document& document, p2List<Tileset>& tileset_map);

	//Load and fill layers
	void FillLayer(const pugi::xml_document& document, p2List<Layer>& layer_map);

	//Log all
	void LogAll();

	//Transform from string to Map_renderorder enum
	Map_renderorder String_to_Enum_1(p2SString str);

	//Transform from string to Map_orientation enum
	Map_Orientation String_to_Enum_2(p2SString str);

public:

	// TODO 1: Add your struct for map info as public for now
	Map first_map;

private:

	pugi::xml_document		map_file;
	p2SString				folder;
	bool					map_loaded;
	p2List<SDL_Texture*>	Map_texture;
};

#endif // __j1MAP_H__