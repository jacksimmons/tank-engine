// Standard Library Algorithms
#include <iostream>
#include <memory>
#include <algorithm>
#include <functional>
#include <chrono>
#include <filesystem>
#include <type_traits>
#include <regex>

// Standard Library Datatypes
#include <string>
#include <array>
#include <vector>
#include <stack>
#include <map>
#include <unordered_map>
#include <utility>

// Dependencies
#include "nlohmann/json.hpp"
using json = nlohmann::json;
namespace fs = std::filesystem;
#include "glm/glm.hpp"
#include "spdlog/spdlog.h"
#include <sol/sol.hpp>

// Tank
#include <core.hpp>
#include <transform.hpp>
#include <assets/resource.hpp>
