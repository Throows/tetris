#include "RessourceManager.hpp"

void RessourceManager::LoadFont(FontsID id, const std::string &path)
{
    auto font = std::make_unique<sf::Font>();
    if (!font->openFromFile(path))
    {
        throw std::runtime_error("Failed to load font: " + path);
    }
    m_fonts[id] = std::move(font);
}

void RessourceManager::LoadTexture(TexturesID id, const std::string &path)
{
    auto texture = std::make_unique<sf::Texture>();
    if (!texture->loadFromFile(path)) {
        throw std::runtime_error("Failed to load texture: " + path);
    }
    m_textures[id] = std::move(texture);
}

sf::Font &RessourceManager::GetFont(FontsID id)
{
    if (m_fonts.find(id) == m_fonts.end()) {
        throw std::runtime_error("Font not found with id: " + std::to_string(id));
    }
    return *m_fonts.at(id);
}

sf::Texture &RessourceManager::GetTexture(TexturesID id)
{
    if (m_textures.find(id) == m_textures.end()) {
        throw std::runtime_error("Texture not found with id: " + std::to_string(id));
    }
    return *m_textures.at(id);
}
