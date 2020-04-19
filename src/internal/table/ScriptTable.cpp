#include "internal/table/ScriptTable.hpp"

#include <cassert>

#include "internal/ICUError.hpp"

namespace cobbletext::internal {

hb_script_t ScriptTable::scriptToHarfBuzz(std::string script) {
    return hb_script_from_string(script.c_str(), script.size());
}

hb_language_t ScriptTable::languageToHarfBuzz(std::string language) {
    return hb_language_from_string(language.c_str(), language.size());
}

void ScriptTable::setTextBuffer(std::shared_ptr<const icu::UnicodeString> text) {
    this->text = text;
}

hb_script_t ScriptTable::getHarfBuzzScript(int32_t codeUnitIndex) {
    assert(text);
    ICUError errorCode;
    auto codePoint = text->char32At(codeUnitIndex);

    auto scriptCode = uscript_getScript(codePoint, errorCode);
    auto usage = uscript_getUsage(scriptCode);

    if (usage == USCRIPT_USAGE_RECOMMENDED) {
        auto name4Letter = uscript_getShortName(scriptCode);

        if (name4Letter == nullptr) {
            return HB_SCRIPT_COMMON;
        } else {
            auto hbScript = hb_script_from_string(name4Letter, 4);
            return hbScript;
        }

    } else {
        return HB_SCRIPT_UNKNOWN;
    }
}

}
