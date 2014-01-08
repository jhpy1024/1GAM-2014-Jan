#ifndef TEXTURE_MANAGER_HPP
#define TEXTURE_MANAGER_HPP

#include <map>
#include <string>
#include <memory>
#include <SFML/Graphics.hpp>

class TextureManager
{
public:
        void addTexture(const std::string& id, const std::string& fileName);
        void removeTexture(const std::string& id);
        
        sf::Texture& getTexture(const std::string& id);

private:
        std::map<std::string, sf::Texture> textures_;
};

#endif