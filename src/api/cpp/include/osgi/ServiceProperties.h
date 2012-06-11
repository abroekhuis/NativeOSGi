/*=============================================================================

  Copyright (c) Sascha Zelzer

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

=============================================================================*/

#pragma once
#ifndef OSGI_SERVICEPROPERTIES_H_
#define OSGI_SERVICEPROPERTIES_H_

#include <osgi/Config.h>
#include <osgi/Export.h>
#include <osgi/Any.h>

#include <cctype>

/// \cond
namespace osgi {

struct ci_char_traits : public std::char_traits<char>
    // just inherit all the other functions
    //  that we don't need to override
{
    
  static bool eq(char c1, char c2)
  {
    return std::toupper(c1) == std::toupper(c2);
  }

  static bool ne(char c1, char c2)
  {
    return std::toupper(c1) != std::toupper(c2);
  }

  static bool lt(char c1, char c2)
  {
    return std::toupper(c1) < std::toupper(c2);
  }

  static bool gt(char c1, char c2)
  {
    return std::toupper(c1) > std::toupper(c2);
  }

  static int compare(const char* s1, const char* s2, std::size_t n)
  {
    while (n-- > 0)
    {
      if (lt(*s1, *s2)) return -1;
      if (gt(*s1, *s2)) return 1;
      ++s1; ++s2;
    }
    return 0;
  }

  static const char* find(const char* s, int n, char a)
  {
    while (n-- > 0 && std::toupper(*s) != std::toupper(a))
    {
      ++s;
    }
    return s;
  }

};

class ci_string : public std::basic_string<char, ci_char_traits>
{
private:

  typedef std::basic_string<char, ci_char_traits> Super;

public:

  inline ci_string() : Super() {}
  inline ci_string(const ci_string& cistr) : Super(cistr) {}
  inline ci_string(const ci_string& cistr, size_t pos, size_t n) : Super(cistr, pos, n) {}
  inline ci_string(const char* s, size_t n) : Super(s, n) {}
  inline ci_string(const char* s) : Super(s) {}
  inline ci_string(size_t n, char c) : Super(n, c) {}

  inline ci_string(const std::string& str) : Super(str.begin(), str.end()) {}

  template<class InputIterator> ci_string(InputIterator b, InputIterator e)
    : Super(b, e)
  {}

  inline operator std::string () const
  {
    return std::string(begin(), end());
  }
};

} // end namespace osgi

OSGI_HASH_FUNCTION_NAMESPACE_BEGIN
OSGI_HASH_FUNCTION_BEGIN(osgi::ci_string)

  std::string ls(arg);
  std::transform(ls.begin(), ls.end(), ls.begin(), ::tolower);

  using namespace OSGI_HASH_FUNCTION_NAMESPACE;
  return OSGI_HASH_FUNCTION(std::string, ls);

OSGI_HASH_FUNCTION_END
OSGI_HASH_FUNCTION_NAMESPACE_END

/// \endcond

namespace osgi {

/**
 * \ingroup cpp_api
 *
 * A hash table based map class with case-insensitive keys. This class
 * uses ci_string as key type and Any as values. Due
 * to the conversion capabilities of ci_string, std::string objects
 * can be used transparantly to insert or retrieve key-value pairs.
 *
 * <p>
 * Note that the case of the keys will be preserved.
 */
typedef OSGI_UNORDERED_MAP_TYPE<ci_string, Any> ServiceProperties;

} // end namespace osgi

#endif // OSGI_SERVICEPROPERTIES_H_
