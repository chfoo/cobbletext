#pragma once

#include <optional>
#include <functional>
#include <string>
#include <memory>

#include <unicode/uscript.h>
#include <unicode/unistr.h>
#include <harfbuzz/hb.h>

namespace cobbletext::internal {

class ScriptTable {
    std::shared_ptr<const icu::UnicodeString> text;

public:
    static hb_script_t scriptToHarfBuzz(std::string script);
    static hb_language_t languageToHarfBuzz(std::string language);

    void setTextBuffer(std::shared_ptr<const icu::UnicodeString> text);

    hb_script_t getHarfBuzzScript(int32_t codeUnitIndex);

};

}
