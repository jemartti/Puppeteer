#ifndef SCENE_LUA_HPP
#define SCENE_LUA_HPP

#include <string>
#include "scene.hpp"
#include "viewer.hpp"


SceneNode* import_lua(const std::string& filename);

#endif
