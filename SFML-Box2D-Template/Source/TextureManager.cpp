#include "../Include/TextureManager.hpp"

void TextureManager::addTexture(const std::string& id, const std::string& fileName)
{
        sf::Texture texture;
        texture.loadFromFile(fileName);

        textures_[id] = texture;
}

void TextureManager::removeTexture(const std::string& id)
{
        textures_.erase(id);
}

sf::Texture& TextureManager::getTexture(const std::string& id)
{
        return textures_[id];
}