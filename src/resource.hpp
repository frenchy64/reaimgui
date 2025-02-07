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

#ifndef REAIMGUI_RESOURCE_HPP
#define REAIMGUI_RESOURCE_HPP

#include <memory>

class Context;

class Resource {
public:
  static constexpr const char *api_type_name { "ImGui_Resource" };

  Resource();
  Resource(const Resource &) = delete;
  virtual ~Resource();

  void keepAlive();

  virtual bool attachable(const Context *) const = 0;

  template<typename T>
  static bool isValid(T *userData)
  {
    static_assert(!std::is_same_v<Resource, T>);

    // short-circuiting dynamic_cast for faster exact type match
    Resource *resource { static_cast<Resource *>(userData) };
    return isValid(resource) &&
      (typeid(*resource) == typeid(T) || dynamic_cast<T *>(resource));
  }

  static void destroyAll();

protected:
  virtual bool heartbeat();
  virtual bool isValid() const;

private:
  class Timer;
  std::shared_ptr<Timer> m_timer;
  unsigned int m_keepAlive;
};

template<>
bool Resource::isValid<Resource>(Resource *);

using ImGui_Resource = Resource;

#endif
