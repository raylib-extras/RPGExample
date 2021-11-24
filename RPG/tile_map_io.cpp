#include "tile_map.h"
#include "sprites.h"

#include "PUGIXML/pugixml.hpp"

const unsigned FLIPPED_HORIZONTALLY_FLAG = 0x80000000;
const unsigned FLIPPED_VERTICALLY_FLAG = 0x40000000;
const unsigned FLIPPED_DIAGONALLY_FLAG = 0x20000000;


std::string GetRelativeResource(const std::string& rootFile, const std::string& relFile)
{
	std::string fullPath = rootFile;
	auto term = fullPath.find_last_of('/');
	if (term != std::string::npos)
		fullPath = fullPath.substr(0, term);

	return fullPath + "/" + relFile;
}

static bool ReadImageData(int& width, int& height, std::string& source, pugi::xml_node image)
{
	if (image.root() == nullptr)
		return false;

	width = image.attribute("width").as_int();
	height = image.attribute("height").as_int();

	source = image.attribute("source").as_string();

	if (source.size() > 0)
	{
		if (source[0] == '.')
		{
			size_t firstSlash = source.find_first_of('/');
			if (firstSlash != std::string::npos)
				source = source.substr(firstSlash + 1);
		}
	}
	return true;
}

bool ReadTileSetNode(pugi::xml_node root, int idOffset, TileMap& map)
{
	float tileWidth = root.attribute("tilewidth").as_float();
	float tileHeight = root.attribute("tileheight").as_float();

	int tileCount = root.attribute("tilecount").as_int();

	int columCount = root.attribute("columns").as_int();
	int spacing = root.attribute("spacing").as_int();
	int margin = root.attribute("margin").as_int();

	for (pugi::xml_node child : root.children())
	{
		std::string n = child.name();
		if (n == "tile")
		{
			int id = child.attribute("id").as_int();

			int width, height;
			std::string source;
			if (!ReadImageData(width, height, source, child.child("image")))
				continue;
			
			// this is where tilesheet data would go
		}
		else if (n == "image")
		{
			int width, height;
			std::string source;
			ReadImageData(width, height, source, child);

			// this is where tilesheet data would go
		}
	}

	return true;
}

bool ReadTileSetFile(const std::string& tilesetFileName, int idOffset, TileMap& map)
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(tilesetFileName.c_str());

	if (result.status != pugi::xml_parse_status::status_ok)
		return false;

	pugi::xml_node root = doc.child("tileset");

	return ReadTileSetNode(root, idOffset, map);
}

bool ReadTiledXML(pugi::xml_document& doc, TileMap& map, const std::string& filePath = std::string())
{
	auto root = doc.child("map");

	auto version = root.attribute("version");
	auto tiledVersion = root.attribute("tiledversion");

	std::string orient = root.attribute("orientation").as_string();
	std::string renderorder = root.attribute("renderorder").as_string();

	map.MapType = TileMapTypes::Isometric;
	if (orient == "orthogonal")
		map.MapType = TileMapTypes::Orthographic;

	int width = root.attribute("width").as_int();
	int height = root.attribute("height").as_int();

	int tilewidth = root.attribute("tilewidth").as_int();
	int tileheight = root.attribute("tileheight").as_int();

	for (auto child : root.children())
	{
		std::string childName = child.name();
		if (childName == "tileset")
		{
			int idOffset = 0;
			if (!child.attribute("firstgid").empty())
				idOffset = child.attribute("firstgid").as_int();

			std::string tilesetFile = child.attribute("source").as_string();
			if (tilesetFile.size() == 0)
			{
				if (!ReadTileSetNode(child, idOffset, map))
					return false;
			}
			else if (!ReadTileSetFile(GetRelativeResource(filePath, tilesetFile), idOffset, map))
				return false;
		}
		else if (childName == "objectgroup")
		{

		}
		else if (childName == "layer")
		{
			int layerID = child.attribute("id").as_int();

			std::string name = child.attribute("name").as_string();

			map.Layers[layerID] = TileLayer();

			TileLayer& layer = map.Layers[layerID];
			layer.Size.x = float(width);
			layer.Size.y = float(height);
			layer.TileSize.x = float(tilewidth);
			layer.TileSize.y = float(tileheight);

			auto data = child.child("data");
			std::string encoding = data.attribute("encoding").as_string();
			if (encoding == "csv")
			{
				std::string contents = data.first_child().value();
				// if it's a windows file it will have \r\n for newlines, not just \n 
				// so remove all instances of \r so that we can consistently read based on \n
				contents.erase(std::remove(contents.begin(), contents.end(), '\r'), contents.end());

				std::vector<std::vector<int>> rawData;

				int posX = 0;
				int posY = 0;

				size_t linePos = 0;
				do
				{
					size_t nextLine = contents.find_first_of('\n', linePos);
					if (nextLine == std::string::npos)
						nextLine = contents.size();

					std::string colText = contents.substr(linePos, nextLine - linePos);
					linePos = nextLine + 1;

					if (colText.size() == 0)
						continue;
					size_t charPos = 0;
					posX = 0;
					do
					{
						size_t nextDelim = colText.find_first_of(',', charPos);
						if (nextDelim == std::string::npos || nextDelim == colText.size() - 1)
							nextDelim = colText.size();

						std::string valStr = colText.substr(charPos, nextDelim - charPos);
						uint32_t val = static_cast<uint32_t>(std::atoll(valStr.c_str()));
						charPos = nextDelim + 1;

						Tile tile;
						if (val & FLIPPED_HORIZONTALLY_FLAG)
							tile.Flip |= SpriteFlipX;

						if (val & FLIPPED_VERTICALLY_FLAG)
							tile.Flip |= SpriteFlipY;

						if (val & FLIPPED_DIAGONALLY_FLAG)
							tile.Flip |= SpriteFlipDiagonal;

						val &= ~(FLIPPED_HORIZONTALLY_FLAG | FLIPPED_VERTICALLY_FLAG | FLIPPED_DIAGONALLY_FLAG);

						// subtract 1 from the index, since our sprites start at 0 not 1
						tile.Sprite = static_cast<int16_t>(val-1);

						layer.Tiles.emplace_back(tile);
						posX++;

					} while (charPos <= colText.size());
					posY++;
				} while (linePos < contents.size() && linePos != std::string::npos);
			}
		}
	}

	return true;
}

bool ReadTileMap(const char* filename, TileMap& map)
{
	map.Layers.clear();

	if (filename == nullptr)
		return false;

	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(filename);
	return result.status == pugi::xml_parse_status::status_ok && ReadTiledXML(doc, map, filename);
}
