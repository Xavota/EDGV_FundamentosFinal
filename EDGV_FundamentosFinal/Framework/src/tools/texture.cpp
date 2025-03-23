#include "tools/texture.h"

#include <iostream>

namespace gl {
  Map<String, sf::Texture> TextureManager::mTextures{};

  void TextureManager::addTexture(String name, String fileName)
  {
    sf::Texture tex;
    if (!tex.loadFromFile(fileName)) 
    {
      std::cout << "Texture file [" << fileName << "] not found" << std::endl;
      return;
    }
    mTextures[name] = tex;
  }
  
  void TextureManager::addTexture(String name, sf::Image image)
  {
    sf::Texture tex;
    if (!tex.loadFromImage(image)) 
    {
      std::cout << "Image not valid" << std::endl;
      return;
    }
    mTextures[name] = tex;
  }
  
  void TextureManager::addTexture(String name, String fileName, sf::Color maskColor)
  {
    sf::Image img;
    if (!img.loadFromFile(fileName))
    {
      std::cout << "Texture file [" << fileName << "] not found" << std::endl;
      return;
    }
    img.createMaskFromColor(maskColor);
  
    sf::Texture tex;
    if (!tex.loadFromImage(img)) 
    {
      std::cout << "Image not valid" << std::endl;
      return;
    }
  
    mTextures[name] = tex;
  }
  
  void TextureManager::addTexture(String name, sf::Image image, sf::Color maskColor)
  {
    image.createMaskFromColor(maskColor);
  
    sf::Texture tex;
    if (!tex.loadFromImage(image))
    {
      std::cout << "imagen no valida" << std::endl;
      return;
    }
  
    mTextures[name] = tex;
  }
  
  sf::Texture* TextureManager::getTexture(String name)
  {
    const auto it = mTextures.find(name);
    if (it == mTextures.end()) {
      std::cout << "Texture [" << name << "] doesn't exists" << std::endl;
      return nullptr;
    }
    return &it->second;
  }
}