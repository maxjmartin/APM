#pragma once

/*********************************************************************/
//
//			Copyright 2022 Max J. Martin
//
//			This file is part of APM.
// 
//			APM is free software : you can redistribute it
//			and /or modify it under the terms of the GNU General
//			Public License as published by the Free Software
//			Foundation, either version 3 of the License, or
//			(at your option) any later version.
//			
//			APM is distributed in the hope that it will be
//			useful, but WITHOUT ANY WARRANTY; without even the
//			implied warranty of MERCHANTABILITY or FITNESS FOR
//			A PARTICULAR PURPOSE.See the GNU General Public
//			
//			You should have received a copy of the GNU General
//			Public License along with APM.If not, see
//			< https://www.gnu.org/licenses/>.
//			
/*********************************************************************/

#include <algorithm>
#include "config.h"

namespace Olly {

    static const Text ESCAPE_CHARS(" \t\r\n\a\f\v\b");

    Text to_lower(Text text);                                  // Set all text in a Text to lower case.
    Text to_upper(Text text);                                  // Set all text in a Text to upper case.

    void ltrim(Text& text);                                    // Mutable remove left white space.
    void rtrim(Text& text);                                    // Mutable remove right white space.
    void lrtrim(Text& text);                                   // Mutable remove left and right white space.

    Text left_trim(Text text);                                 // Copy and remove left white space.
    Text right_trim(Text text);                                // Copy and remove right white space.
    Text trim(Text text);                                      // Copy and remove left and right white space.

    static Text_Tokens split(Text text, Char delim);           // Split a string at a single character.
    static Text_Tokens split(const Text& text, Text delim);    // Split a string using a series of characters

    template<typename T> T to(Text text);                      // Convert a string to a specific type T.

    /********************************************************************************************/
    //
    //   The functions are inline defined below, since they will need to be used by the base
    //   templated data types managed by the var class.
    //
    /********************************************************************************************/

    inline Text to_lower(Text text) {

        std::transform(text.begin(), text.end(), text.begin(), ::tolower);

        return text;
    }

    inline Text to_upper(Text text) {

        std::transform(text.begin(), text.end(), text.begin(), ::toupper);

        return text;
    }

    inline void ltrim(Text& text) {

        if (text.empty()) {
            return;
        }

        text.erase(text.begin(), std::find_if(text.begin(), text.end(), [](Char ch) {
            return !std::isspace(ch);
            }
        ));
    }

    inline void rtrim(Text& text) {
        if (text.empty()) {
            return;
        }
        text.erase(std::find_if(text.rbegin(), text.rend(), [](Char ch) {
            return !std::isspace(ch);
            }
        ).base(), text.end());
    }

    inline void lrtrim(Text& text) {
        ltrim(text);
        rtrim(text);
    }

    inline Text left_trim(Text text) {
        ltrim(text);
        return text;
    }

    inline Text right_trim(Text text) {
        rtrim(text);
        return text;
    }

    inline Text trim(Text text) {
        lrtrim(text);
        return text;
    }

    inline Text_Tokens split(Text text, Char delim) {

        Text_Stream stream;
        stream.str(text);

        Text_Tokens tokens;

        while (std::getline(stream, text, delim)) {

            tokens.push_back(text);
        }

        return tokens;
    }

    inline Text_Tokens split(const Text& text, Text delim) {

        if (delim == "") {
            return split(text, ESCAPE_CHARS);
        }

        Text_Tokens tokens, buffer, temp;

        Char d;

        d = delim.back();
        delim.pop_back();

        tokens = split(text, d);

        while (delim.size()) {

            d = delim.back();
            delim.pop_back();

            buffer = tokens;
            tokens.clear();

            for (Size i = 0, stop = buffer.size(); i < stop; i += 1) {

                temp = split(buffer.at(i), d);

                for (Size j = 0, stop = temp.size(); j < stop; j += 1) {

                    tokens.push_back(temp.at(j));
                }

                temp.clear();
            }
        }

        return tokens;
    }

    template<typename T> inline T to(Text text) {

        T n = T();

        Text_Stream stream;

        try {
            stream << trim(text);
            stream >> n;
        }
        catch (...) {
            n = T();
        }

        return n;
    }

}