#include "utility/runtime.hpp"

#include <cstdint>
#include <expected>
#include <filesystem>
#include <string>
#include <system_error>

#include "utility/errors.hpp"
#include "utility/logger.hpp"

#if defined(__APPLE__)
#include <mach-o/dyld.h>

#include <vector>
#elif defined(__WIN32)
#define NOMINMAX
#include <window.h>
#endif

namespace utility::runtime {

[[nodiscard]]
std::expected<std::filesystem::path, RuntimeUtilityError> executable_path() {
    std::filesystem::path raw_path;
    std::error_code error_code;
#if defined(__linux)
    raw_path = std::filesystem::read_symlink("/proc/self/exe", error_code);
    if (error_code) {
        LOG_ERROR("Error finding executable path: {} {}", error_code.value(), error_code.message());
        return std::unexpected{RuntimeUtilityError::LINUX_FIND_EXE_ERROR};
    };
#elif defined(__APPLE__)
    std::uint32_t size = 0;
    (void)_NSGetExecutablePath(nullptr, &size);

    if (size == 0) {
        LOG_ERROR("Error finding executable path: path size = 0");
        return std::unexpected{RuntimeUtilityError::APPLE_FIND_EXE_ERROR};
    }

    std::vector<char> buffer(size);
    if (_NSGetExecutablePath(buffer.data(), &size) != 0) {
        LOG_ERROR("Error reading executable path");

        return std::unexpected{RuntimeUtilityError::APPLE_FIND_EXE_ERROR};
    }

    raw_path = buffer.data();
#elif defined(__WIN32)
    constexpr std::size_t windows_max_path_length = 32'768;
    std::wstring buffer(windows_max_path_length, L'\0');
    const DWORD size =
        GetModuleFileNameW(nullptr, buffer.data(), static_cast<DWORD>(buffer.size()));
    if (size == 0 || size >= buffer.size()) {
        LOG_ERROR("Error reading executable path: size out of bounds");
        return std::unexpected{RuntimeUtilityError::WINDOWS_FIND_EXE_ERROR};
    }

    buffer.resize(size);
    raw_path = buffer;
#else
    return std::unexpected{RuntimeUtilityError::UNSUPORTED_PLATFORM};
#endif
    auto canonical = std::filesystem::weakly_canonical(raw_path, error_code);
    if (error_code) {
        LOG_ERROR("Error resolving canonical path: {} {}", error_code.value(),
                  error_code.message());
        return std::unexpected{RuntimeUtilityError::EXECUTABLE_PATH_RESOLVE_ERROR};
    }
    return canonical;
}

[[nodiscard]]
std::expected<std::filesystem::path, RuntimeUtilityError> executable_directory() {
    auto executable = executable_path();
    if (!executable) {
        LOG_ERROR("Error resolving directory, executable path not found");
        return std::unexpected{RuntimeUtilityError::EXECUTABLE_PATH_RESOLVE_ERROR};
    }
    return executable->parent_path();
}

}  // namespace utility::runtime
