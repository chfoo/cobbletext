#include "FontInfo.hpp"

#include <ostream>

namespace cobbletext {

std::ostream & operator<<(std::ostream & stream, const FontInfo & font) {
    stream
        << "[FontInfo "
        << "id=" << font.id << " "
        << "familyName=" << font.familyName << " "
        << "styleName=" << font.styleName << " "
        << "unitsPerEM=" << font.unitsPerEM << " "
        << "ascender=" << font.ascender << " "
        << "descender=" << font.descender << " "
        << "underlinePosition=" << font.underlinePosition << " "
        << "underlineThickness=" << font.underlineThickness << " "
        << "]";

    return stream;
}

}
