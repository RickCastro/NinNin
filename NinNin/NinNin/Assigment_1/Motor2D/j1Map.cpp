#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Map.h"
#include <math.h>

j1Map::j1Map() : j1Module(), map_loaded(false)
{
	name.create("map");
}

// Destructor
j1Map::~j1Map()
{}

// Called before render is available
bool j1Map::Awake(pugi::xml_node& config)
{
	LOG("Loading Map Parser");
	bool ret = true;

	folder.create(config.child("folder").child_value());

	return ret;
}

void j1Map::Draw()
{
	if(map_loaded == false)
		return;

	// TODO 6: Iterate all tilesets and draw all their 
	// images in 0,0 (you should have only one tileset for now)

	for (uint i = 0; i < first_map.map_tileset.count(); i++) {

		SDL_Texture* aux_texture;

		aux_texture = App->tex->Load(first_map.map_tileset[i].img_source.GetString());
		Map_texture.add(aux_texture);
		App->render->Blit(Map_texture[i], 0, 0);
	}

}

// Called before quitting
bool j1Map::CleanUp()
{
	LOG("Unloading map");

	// TODO 2: Make sure you clean up any memory allocated
	// from tilesets / map


	map_file.reset();

	return true;
}

// Load new map
bool j1Map::Load(const char* file_name)
{
	bool ret = true;
	p2SString tmp("%s%s", folder.GetString(), file_name);

	pugi::xml_parse_result result = map_file.load_file(tmp.GetString());

	if(result == NULL)
	{
		LOG("Could not load map xml file %s. pugi error: %s", file_name, result.description());
		ret = false;
	}

	if(ret == true)
	{
		// TODO 3: Create and call a private function to load and fill
		// all your map data
		FillMap(map_file);
	}

	// TODO 4: Create and call a private function to load a tileset
	// remember to support more any number of tilesets!
	FillTilset(map_file, first_map.map_tileset);
	FillLayer(map_file, first_map.map_layers);

	if(ret == true)
	{
		// TODO 5: LOG all the data loaded
		// iterate all tilesets and LOG everything
		LogAll();
	}

	map_loaded = ret;

	return ret;
}

void j1Map::FillMap(const pugi::xml_document& document) {

	pugi::xml_node aux_node = document.child("map");

	
	first_map.orientation = String_to_Enum_2(aux_node.attribute("orientation").as_string());
	

	first_map.renderorder = String_to_Enum_1(aux_node.attribute("renderorder").as_string());
	

	first_map.width = aux_node.attribute("width").as_uint();
	

	first_map.height = aux_node.attribute("height").as_uint();
	

	first_map.tilewidth = aux_node.attribute("tilewidth").as_uint();
	

	first_map.tileheight = aux_node.attribute("tileheight").as_uint();
	

	first_map.nextobject = aux_node.attribute("nextobjectid").as_uint();
	
}

void j1Map::FillTilset(const pugi::xml_document& document, p2List<Tileset>& tileset_map) {

	pugi::xml_node tilset;
	Tileset aux_tileset;
	uint counter = 1;

	p2SString source = "maps/";

	for (tilset = document.child("map").child("tileset"); tilset; tilset = tilset.next_sibling("tileset")) {
		

	

		aux_tileset.name = tilset.attribute("name").as_string();
		

		aux_tileset.img_source = (source += tilset.child("image").attribute("source").as_string());
		

		aux_tileset.tileWidth = tilset.attribute("tilewidth").as_uint();
		

		aux_tileset.tileHeight = tilset.attribute("tileheight").as_uint();
		

		aux_tileset.spacing = tilset.attribute("spacing").as_uint();
		

		aux_tileset.margin = tilset.attribute("margin").as_uint();
		

		tileset_map.add(aux_tileset);
		counter++;
	}
}

void j1Map::FillLayer(const pugi::xml_document& document, p2List<Layer>& layer_map) {

	Layer aux_layer;
	pugi::xml_node layer_node=document.child("map").child("layer");
	uint i = 0;

	aux_layer.name = layer_node.attribute("name").as_string();
	aux_layer.width = layer_node.attribute("width").as_uint();
	aux_layer.height = layer_node.attribute("height").as_uint();
	
	for (pugi::xml_node data_node = layer_node.child("data").child("tile"); data_node; data_node = data_node.next_sibling("tile")) {

		aux_layer.data.add(data_node.attribute("gid").as_uint());

	}

	first_map.map_layers.add(aux_layer);

}

void j1Map::LogAll() {

	LOG("-----------------Map Info--------------------------------------");
	
	LOG("orientation --> %s ( %d )", map_file.child("map").attribute("orientation").as_string(), first_map.orientation);

	LOG("Renderorder --> %s ( %d )", map_file.child("map").attribute("renderorder").as_string(), first_map.renderorder);

	LOG("width --> %d", first_map.width);

	LOG("height --> %d", first_map.height);

	LOG("tilewidth --> %d", first_map.tilewidth);

	LOG("tileheight --> %d", first_map.tileheight);

	LOG("nextobject --> %d", first_map.nextobject);


	LOG("---------------------------------------------------------------");

	for (uint i = 0; i < first_map.map_tileset.count();i++) {

		LOG("-----TileSet %d----", i);

		LOG("Name: %s", first_map.map_tileset[i].name.GetString());

		LOG("Image source: %s", first_map.map_tileset[i].img_source);

		LOG("TileWidth: %d", first_map.map_tileset[i].tileWidth);

		LOG("TileHeight: %d", first_map.map_tileset[i].tileHeight);

		LOG("Spacing: %d", first_map.map_tileset[i].spacing);

		LOG("Margin: %d", first_map.map_tileset[i].margin);

	}

	for (uint i = 0; i < first_map.map_layers.count(); i++) {

		LOG("-----Layer %d----", i);

		LOG("Name: %s", first_map.map_layers[i].name.GetString());

		LOG("Width: %d", first_map.map_layers[i].width);

		LOG("Height: %d", first_map.map_layers[i].height);

		LOG("Data: %d", first_map.map_layers[i].data.count());

	}
}

Map_renderorder j1Map::String_to_Enum_1(p2SString str) {
	
	if (str == "right-down")
		return right_down;

	if (str == "right-up")
		return right_up;

	if (str == "left-down")
		return left_down;

	if (str == "left-up")
		return left_up;
}

Map_Orientation j1Map::String_to_Enum_2(p2SString str) {
	
	if (str == "orthogonal")
		return orthogonal;

	if (str == "isometric")
		return isometric;

	if (str == "staggered")
		return staggered;

	if (str == "hexagonal")
		return hexagonal;

}