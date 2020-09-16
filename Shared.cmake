# Common build functions

include(FetchContent)

#! INSTALL_FROM_REPO
# Checkout and install an external library from remote repository.
#
# \arg:REPO Repository URL.
# \arg:NAME Library name.
# \arg:BRANCH Branch name.
# \flag:BUILD Build subdir flag (use false if CMakeLists.txt is not present or for header-only libraries).
# \arg:INCLUDE_SUFFIX The suffix to append to include directory.
#
function(INSTALL_FROM_REPO REPO NAME BRANCH BUILD INCLUDE_SUFFIX)
    message(STATUS "Installing external library ${NAME}")
    FetchContent_Declare(
            ${NAME}
            GIT_REPOSITORY ${REPO}
            GIT_TAG ${BRANCH}
    )

    string(TOLOWER ${NAME} LIB_NAME_LOWERCASE)
    string(CONCAT SRC_DIR ${LIB_NAME_LOWERCASE}_SOURCE_DIR)
    string(CONCAT BIN_DIR ${LIB_NAME_LOWERCASE}_BINARY_DIR)
    FetchContent_GetProperties(${NAME})
    if (NOT ${LIB_NAME_LOWERCASE}_POPULATED)
        FetchContent_Populate(${NAME})
        if (BUILD)
            message(STATUS "Building ${NAME}")
            add_subdirectory(${${SRC_DIR}} ${${BIN_DIR}})
            message(STATUS "Building ${NAME} - done")
        endif ()
    endif ()
    set(INCLUDE_DIR ${${SRC_DIR}}${INCLUDE_SUFFIX})
    include_directories(${INCLUDE_DIR})
    message(STATUS "Include directory for ${NAME}: ${INCLUDE_DIR}")
    message(STATUS "Installing external library ${NAME} - done")

endfunction()