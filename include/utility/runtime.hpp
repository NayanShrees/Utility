#ifndef RUNTIME_HPP_
#define RUNTIME_HPP_

#include <expected>
#include <filesystem>

#include "utility/errors.hpp"

namespace utility::runtime {

[[nodiscard]]
std::expected<std::filesystem::path, RuntimeUtilityError> executable_path();

[[nodiscard]]
std::expected<std::filesystem::path, RuntimeUtilityError> executable_directory();

}  // namespace utility::runtime

#endif  // RUNTIME_HPP_
