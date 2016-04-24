#pragma once

#include <Maple/FWD.h>
#include <Bibim/String.h>

namespace Maple
{
    class Version
    {
        BBThisIsStaticClass(Version);
        public:
            static const int Major = 1;
            static const int Minor = 0;
            static const int Build = 358;

            static inline const Bibim::String& ToString()
            {
                static String s = String::Empty;
                if (s.IsEmpty())
                    s = String::CFormat("%d.%d.%d", Major, Minor, Build);
        
                return s;
            }
    };
}

