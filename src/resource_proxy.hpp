/* ReaImGui: ReaScript binding for Dear ImGui
 * Copyright (C) 2021-2023  Christian Fillion
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef REAIMGUI_RESOURCE_PROXY_HPP
#define REAIMGUI_RESOURCE_PROXY_HPP

#include "error.hpp"
#include "optional.hpp"
#include "resource.hpp"

#include <cassert>

template<typename ProxyT, typename ResT, typename ObjT>
class ResourceProxy {
public:
  using Key = uint32_t;

  ResourceProxy()  = delete;
  ~ResourceProxy() = delete;

  template<typename Type>
  static ProxyT *encode(const ResT *in)
  {
    static_assert(sizeof(Type::key) <= sizeof(Key));
    uintptr_t out { reinterpret_cast<uintptr_t>(in) };
    out ^= static_cast<uintptr_t>(Type::key);
    assert("out of double range (EEL incompatible)" && out <= 1ull<<53);
    return reinterpret_cast<ProxyT *>(out);
  }

  static bool isValid(const ProxyT *ptr)
  {
    return ProxyT::Decoder::template decode<ValidateStrategy>(ptr);
  }

  bool isValid() const { return isValid(static_cast<const ProxyT *>(this)); }

  static auto get(const ProxyT *ptr, ResT **res = nullptr)
  {
    return ProxyT::Decoder::template decode<GetStrategy>(ptr, res);
  }

  auto get(ResT **res = nullptr) const
  {
    return get(static_cast<const ProxyT *>(this), res);
  }

protected:
  template<typename... Getters>
  struct MakeDecoder {
    template<typename Strategy>
    static auto decode(const ProxyT *ptr, ResT **resOut = nullptr)
    {
      ResT *res;
      std::optional<decltype(Strategy::fail(ptr))> rv;

      static_cast<void>(((
        Resource::isValid(res = reinterpret_cast<ResT *>(
            encode<Getters>(reinterpret_cast<const ResT *>(ptr))))
          // got a match, stop the search
          ? (rv = Strategy::template pass<Getters>(res), false)
          : true // otherwise, try again with the next posible key
      ) && ...));

      if(!rv)
        return Strategy::fail(ptr);
      if(resOut)
        *resOut = res;
      return *rv;
    }
  };

  struct GetStrategy {
    template<typename Getter>
    static ObjT *pass(ResT *resource) { return Getter::get(resource); }
    static ObjT *fail(const ProxyT *ptr) {
      char message[255];
      snprintf(message, sizeof(message),
        "expected a valid %s*, got %p", ProxyT::api_type_name, ptr);
      throw reascript_error { message };
    }
  };

  struct ValidateStrategy {
    template<typename T>
    static bool pass(ResT *)         { return true; }
    static bool fail(const ProxyT *) { return false; }
  };
};

#endif
