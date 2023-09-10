#ifndef visualarrangementtools_hpp
#define visualarrangementtools_hpp

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>

// Function to split a string into multiple lines based on maximum characters per line
void splitTextIntoLines(const char* inputText, int window_width, TTF_Font* font, int maxLines, char** lines, int* numLines) {
    *numLines = 0;
    const char* start = inputText;
    const char* end = inputText;

    while (*end != '\0' && *numLines < maxLines) {
        int linewidth = 0;
        int charsThisLine = 0;

        // Find the end of the line or the end of the string
        while (*end != '\0' && *end != '\n' && linewidth < window_width) {
            end++;
            int charWidth;
            TTF_GlyphMetrics(font, *end, NULL, NULL, NULL, NULL, &charWidth);
            linewidth += charWidth;
            charsThisLine++;
        }

        // Allocate memory for this line
        lines[*numLines] = (char*)malloc(charsThisLine + 1);

        // Copy the line into the allocated memory
        strncpy(lines[*numLines], start, charsThisLine);
        lines[*numLines][charsThisLine] = '\0';

        // Move to the next line
        (*numLines)++;

        // Skip newline character if present
        if (*end == '\n') {
            end++;
        }

        start = end;
    }
}
#endif 
