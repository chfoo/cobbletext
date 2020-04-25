#pragma once

#include <optional>
#include <functional>
#include <string>
#include <memory>

#include <unicode/uscript.h>
#include <unicode/unistr.h>

#ifdef __EMSCRIPTEN__
    #include <hb.h>
#else
    #include <harfbuzz/hb.h>
#endif


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
