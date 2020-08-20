#pragma
#include <SFML/Graphics.hpp>

class Level : public sf::Drawable, public sf::Transformable
{
public:
	Level();
	~Level();

	void ReadFromFile(std::ifstream* file);
	void WriteToFile(std::ofstream* file);

	bool LoadLevel(std::string& tilesetTexturePath, sf::Vector2u tileSize, int *tiles, int width, int height, int tileGap, bool* collisionTiles);

	void PlaceTileAtMousePosition(float x, float y, int tileValue);
	void PlaceTilesInArea(sf::FloatRect rectangle, int tileValue);

	void PlaceCollisionTileAtMousePosition(float x, float y, bool collision);
	void PlaceCollisionTilesInArea(sf::FloatRect rectangle, bool collision);

	sf::Vector2u &ReturnTileSize();
	sf::Vector2u &ReturnLevelSize();
	std::string &ReturnTexturePath();
	int ReturnTileGap();

	void EnableCollisionTiles();
	void DisableCollisionTiles();

private:
	sf::Texture m_tilesetTexture;
	std::string m_tilesetTexturePath;

	sf::VertexArray m_tileVertices;
	sf::VertexArray m_collisionVertices;

	int* m_numberOfTiles;
	sf::Vector2u m_tileSize;
	int m_tileGap;

	sf::Vector2u m_levelSize;

	bool *m_collisionTileValues;
	bool m_collisionTilesEnabled;

	std::string ReadString(std::ifstream *file);
	void WriteString(std::ofstream *file, std::string& string);

	void UpdateTileVertices(int x, int y);
	void UpdateCollisionTileVertices(int x, int y, bool collision);

	void draw(sf::RenderTarget &renderTarget, sf::RenderStates renderStates) const override;
};