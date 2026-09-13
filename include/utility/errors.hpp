#ifndef ERRORS_HPP_
#define ERRORS_HPP_

enum class FileUtilityError {
    FILE_NOT_FOUND = 1,
    FILE_OPEN_ERROR = 2,
    RUNTIME_RESOLVE_ERROR = 1000,
};

enum class RuntimeUtilityError {
    UNSUPORTED_PLATFORM = 1,
    LINUX_FIND_EXE_ERROR = 2,
    APPLE_FIND_EXE_ERROR = 3,
    WINDOWS_FIND_EXE_ERROR = 4,
    EXECUTABLE_PATH_RESOLVE_ERROR = 5,
};

#endif  // ERRORS_HPP_
