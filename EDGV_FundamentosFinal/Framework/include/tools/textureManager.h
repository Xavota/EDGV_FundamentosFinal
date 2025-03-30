#pragma once
#include <SFML/Graphics.hpp>

#include "platform/defines.h"
#include "platform/module.hpp"

namespace gl {
  /**
   * @brief  Loads and store the textures for later use in any part of the document
   */
  class TextureManager : public Module<TextureManager>
  {
   public:
    /**
     * @brief  Default constructor
     */
    TextureManager() = default;
    /**
     * @brief  Default destructor
     */
    ~TextureManager() = default;

    /**
     * @brief  Adds a texture to the list
     * @param    name: the name of the texture that will be stored as
     * @param    fileName: the file where the texture is
     * @bug    No know Bugs
     */
    static void addTexture(String name, String fileName);
    /**
     * @brief  Adds a texture to the list
     * @param    name: the name of the texture that will be stored as
     * @param    image: the image to convert into a texture
     * @bug    No know Bugs
     */
    static void addTexture(String name, sf::Image image);
    
    /**
     * @brief  Adds a texture to the list and make a mask with the given color
     * @param    name: the name of the texture that will be stored as
     * @param    fileName: the file where the texture is
     * @param    maskColor: the color to wich the mask will be applied
     * @bug    No know Bugs
     */
    static void addTexture(String name, String fileName, sf::Color maskColor);
    /**
     * @brief  Adds a texture to the list and make a mask with the given color
     * @param    name: the name of the texture that will be stored as
     * @param    image: the image to convert into a texture
     * @param    maskColor: the color to wich the mask will be applied
     * @bug    No know Bugs
     */
    static void addTexture(String name, sf::Image image, sf::Color maskColor);
    
    /**
     * @brief  Returns a pointer to the texture with that name
     * @param    name: the name to identify the texture
     * @bug    No know Bugs
     */
    static sf::Texture* getTexture(String name);
    
   private:
    /**
     * @brief  The array of textures added
     */
  	static Map<String, sf::Texture> mTextures;
  };
}