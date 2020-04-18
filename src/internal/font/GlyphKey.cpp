#include "internal/font/GlyphKey.hpp"

#include <functional>

#include <boost/container_hash/hash.hpp>

#include "internal/RandomUtil.hpp"

namespace cobbletext::internal {

bool GlyphKey::operator==(const GlyphKey & other) const {
    return fontFace == other.fontFace
        && fontSize == other.fontSize
        && index == other.index;
}

std::size_t GlyphKeyHasher::operator()(GlyphKey const & glyphKey) const {
    std::size_t hash = RandomUtil::getSeed();
    boost::hash_combine(hash, std::hash<FontID>{}(glyphKey.fontFace));
    boost::hash_combine(hash, std::hash<double>{}(glyphKey.fontSize));
    boost::hash_combine(hash, std::hash<uint32_t>{}(glyphKey.index));

    return hash;
}

}
