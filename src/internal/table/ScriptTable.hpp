#pragma once

#include <optional>
#include <functional>
#include <string>

#include <unicode/uscript.h>
#include <unicode/unistr.h>
#include <harfbuzz/hb.h>

namespace cobbletext::internal {

class ScriptTable {
    std::optional<std::reference_wrapper<const icu::UnicodeString>> text;

public:
    static hb_script_t scriptToHarfBuzz(std::string script);
    static hb_language_t languageToHarfBuzz(std::string language);

    void setText(const icu::UnicodeString & text);

    hb_script_t getHarfBuzzScript(int32_t codeUnitIndex);

};

}
