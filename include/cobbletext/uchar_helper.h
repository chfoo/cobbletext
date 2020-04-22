#pragma once


#ifndef COBBLETEXT_NO_UCHAR_H
    #include <uchar.h>
    typedef char16_t CobbletextChar16;
    typedef char32_t CobbletextChar32;
#else
    #include <stdint.h>
    typedef uint_least16_t CobbletextChar16;
    typedef uint_least32_t CobbletextChar32;
#endif
