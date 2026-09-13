#ifndef FILE_HPP_
#define FILE_HPP_

#include <expected>
#include <filesystem>
#include <string>
#include <string_view>

#include "errors.hpp"

namespace utility::file {

[[nodiscard]]
std::expected<std::string, FileUtilityError> read(const std::filesystem::path& path);

[[nodiscard]]
std::expected<std::string, FileUtilityError> read(
    const std::string_view& file_name,
    const std::filesystem::path& relative_file_path = std::filesystem::path(),
    const std::filesystem::path& absolute_file_path = std::filesystem::path());
}  // namespace utility::file

#endif  // FILE_HPP_
