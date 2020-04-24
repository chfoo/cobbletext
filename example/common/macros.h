#pragma once


#define ABORT_WITH_MESSAGE(message) { printf((message)); abort(); }
#define ABORT_IF_NULL(value, message) { if ((value) == NULL) { printf((message)); abort(); } }
