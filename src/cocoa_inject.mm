/* ReaImGui: ReaScript binding for Dear ImGui
 * Copyright (C) 2021  Christian Fillion
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

#include "cocoa_inject.hpp"

#include <objc/runtime.h>
#include <string>

constexpr char TARGET_KEY {};

static std::string joinClassNames(Class parent, Class child)
{
  std::string name;
  name += object_getClassName(child);
  name += '_';
  name += object_getClassName(parent);
  return name;
}

static bool isKVOSubclass(NSObject *object)
{
  return [object class] == class_getSuperclass(object_getClass(object));
}

static void injectMethods(Class source, Class target)
{
  unsigned int methodCount {};
  Method *methods { class_copyMethodList(source, &methodCount) };

  for(unsigned int i {}; i < methodCount; ++i) {
    Method method { methods[i] };
    class_addMethod(target, method_getName(method),
      method_getImplementation(method), method_getTypeEncoding(method));
  }

  free(methods);
}

static Class makeSubclass(Class parent, Class child)
{
  const std::string &name { joinClassNames(parent, child) };
  Class subclass { objc_getClass(name.c_str()) };

  if(!subclass &&
      (subclass = objc_allocateClassPair(parent, name.c_str(), 0))) {
    injectMethods(child, subclass);
    objc_registerClassPair(subclass);
  }

  return subclass;
}

void CocoaInject::inject(Class source, NSObject *target)
{
  Class targetClass { object_getClass(target) };

  if(isKVOSubclass(target))
    injectMethods(source, targetClass); // injects into all instances
  else if(Class subclass { makeSubclass(targetClass, source) })
    object_setClass(target, subclass); // modifies only this instance
  else
    return;

  objc_setAssociatedObject(target, &TARGET_KEY, source, OBJC_ASSOCIATION_ASSIGN);
}

bool CocoaInject::isTarget(NSObject *object)
{
  // Checks whether the current instance belongs to us when the custom methods
  // have have been injected directly into KVO's own existing subclass.
  return !!objc_getAssociatedObject(object, &TARGET_KEY);
}
