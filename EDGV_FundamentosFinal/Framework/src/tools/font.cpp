#include "tools/font.h"

#include <iostream>

namespace gl {  
  void FontManager::addFont(const String& name, const String& fontName)
  {
    sf::Font font;
    if (!font.openFromFile(fontName)) {
      std::cout << "Font not found" << std::endl;
    }
    mFonts[name] = font;
  }
  
  void FontManager::addFont(const String& name, const sf::Font& font)
  {
    mFonts[name] = font;
  }
  
  sf::Font* FontManager::getFont(const String& name)
  {
    const auto it = mFonts.find(name);
    if (it == mFonts.end()) {
      std::cout << "The font [" << name << "] doesn't exists" << std::endl;
      return nullptr;
    }
    return &it->second;
  }
}