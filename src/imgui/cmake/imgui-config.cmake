cmake_minimum_required(VERSION 3.0)

# Copied from `configure_package_config_file`
macro(set_and_check _var _file)
    set(${_var} "${_file}")
    if(NOT EXISTS "${_file}")
        message(FATAL_ERROR "File or directory ${_file} referenced by variable ${_var} does not exist !")
    endif()
endmacro()

# Copied from `configure_package_config_file`
macro(check_required_components _NAME)
    foreach(comp ${${_NAME}_FIND_COMPONENTS})
        if(NOT ${_NAME}_${comp}_FOUND)
            if(${_NAME}_FIND_REQUIRED_${comp})
                set(${_NAME}_FOUND FALSE)
            endif()
        endif()
    endforeach()
endmacro()

set_and_check(IMGUI_PREFIX              "${CMAKE_CURRENT_LIST_DIR}/..")
set_and_check(IMGUI_INCLUDE_DIR         "${IMGUI_PREFIX}/")
set_and_check(IMGUI_INCLUDE_BEND_DIR    "${IMGUI_PREFIX}/backends")
set(IMGUI_INCLUDE_DIRS                  "${IMGUI_INCLUDE_DIR}")
set(IMGUI_INCLUDE_BEND_DIRS             "${IMGUI_INCLUDE_BEND_DIR}")