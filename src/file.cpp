#include "utility/file.hpp"

#include <expected>
#include <filesystem>
#include <format>
#include <fstream>
#include <iterator>
#include <system_error>

#include "utility/errors.hpp"
#include "utility/logger.hpp"
#include "utility/runtime.hpp"

namespace utility::file {

[[nodiscard]]
std::expected<std::string, FileUtilityError> read(const std::filesystem::path& path) {
    std::ifstream input_stream(path, std::ios::in | std::ios::binary | std::ios::ate);
    if (!input_stream) {
        std::error_code error_code;
        if (!std::filesystem::exists(path, error_code) && !error_code) {
            LOG_ERROR("File not found at path: {}\nError code: {}, message: {}", path.string(),
                      error_code.value(), error_code.message());
            return std::unexpected{FileUtilityError::FILE_NOT_FOUND};
        }
        LOG_ERROR("Opening file failed: Error code: {}, message: {}", error_code.value(),
                  error_code.message());
        return std::unexpected{FileUtilityError::FILE_OPEN_ERROR};
    }

    std::streamsize stream_size = input_stream.tellg();
    input_stream.seekg(0, std::ios::beg);

    std::string data;
    data.reserve(stream_size);

    return data.assign(std::istreambuf_iterator<char>(input_stream),
                       std::istreambuf_iterator<char>());
}

[[nodiscard]]
std::expected<std::string, FileUtilityError> read(const std::string_view& file_name,
                                                  const std::filesystem::path& relative_file_path,
                                                  const std::filesystem::path& absolute_file_path) {
    auto working_path = std::filesystem::path();
    // Prefer absolute -> relative -> default to root of build/install
    if (!absolute_file_path.empty() && std::filesystem::exists(absolute_file_path)) {
        working_path = absolute_file_path;
        LOG_DEBUG("Absolute path {}", working_path.string());
    } else if (!relative_file_path.empty()) {
        auto executable_dir = utility::runtime::executable_directory();
        if (!executable_dir) {
            LOG_ERROR("Error resolving relative file path to absolute");
            return std::unexpected{FileUtilityError::RUNTIME_RESOLVE_ERROR};
        }
        working_path = (*executable_dir).parent_path() / relative_file_path;
        LOG_DEBUG("relative path working dir {}", working_path.string());
    }

    // Default to top of build/install folder since app will be in build/bin or
    // install/bin
    if (working_path.empty()) {
        auto executable_dir = utility::runtime::executable_directory();
        if (!executable_dir) {
            LOG_ERROR("Error resolving relative file path to absolute");
            return std::unexpected{FileUtilityError::RUNTIME_RESOLVE_ERROR};
        }
        working_path = (*executable_dir).parent_path();
        LOG_INFO("File path empty defaulting to base file path: {}", working_path.string());
    }
    LOG_INFO("Utility File reader called on {}/{}", working_path.relative_path().string(),
             file_name);

    auto file_path = working_path / file_name;
    return read(file_path);
}

}  // namespace utility::file
