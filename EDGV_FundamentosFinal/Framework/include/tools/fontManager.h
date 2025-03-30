#pragma once
#include <SFML/Graphics.hpp>

#include "platform/defines.h"
#include "platform/stdHeaders.h"
#include "platform/module.hpp"

namespace gl {
  /**
   * @brief  Loads and store the fonts for later use in any part of the document
   */
  class FontManager : public Module<FontManager>
  {
   public:
    /**
     * @brief  Default constructor
     */
    FontManager() = default;
    /**
     * @brief  Default destructor
     */
    ~FontManager() = default;

    /**
     * @brief  Adds a font to the list
     * @param    name: the name of the font that will be stored as
     * @param    fontName: the file where the font is
     * @bug    No know Bugs
     */
    void addFont(const String& name, const String& fontName);
    /**
     * @brief  Adds a font to the list
     * @param    name: the name of the font that will be stored as
     * @param    font: the font to replicate
     * @bug    No know Bugs
     */
    void addFont(const String& name, const sf::Font& font);
    /**
     * @brief  Returns a pointer to the font with that name
     * @param    name: the name to identify the font
     * @bug    No know Bugs
     */
    sf::Font* getFont(const String& name);
    
   private:
    /**
     * @brief  The array of fonts added
     */
    Map<String, sf::Font> mFonts;
  };
}
