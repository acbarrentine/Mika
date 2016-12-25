#pragma once

class Platform
{
public:
    static const char* FileNameStem(const char* filePath);
    static void CreateDirectory(const char* filePath);
    static void Message(FILE* stream, const char* format, ...);
    static void MessageArgs(FILE* stream, const char* format, va_list args);
};
