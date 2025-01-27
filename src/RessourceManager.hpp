#pragma once
#include <map>
#include <SFML/Graphics.hpp>

#define ASSET_SIZE (384)

enum FontsID {
    CHEESE_MARKET,
};

enum TexturesID {
    TETROMINO,
};

class RessourceManager
{
public:
    RessourceManager() = default;
    ~RessourceManager() = default;

    void LoadFont(FontsID id, const std::string& path);
    void LoadTexture(TexturesID id, const std::string& path);

    sf::Font& GetFont(FontsID id);
    sf::Texture& GetTexture(TexturesID id);

private:
    std::map<FontsID, std::unique_ptr<sf::Font>> m_fonts;
    std::map<TexturesID, std::unique_ptr<sf::Texture>> m_textures;
};