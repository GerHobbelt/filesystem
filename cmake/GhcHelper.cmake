macro(AddExecutableWithStdFS targetName)
if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang" AND (CMAKE_CXX_COMPILER_VERSION VERSION_EQUAL 7.0 OR CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 7.0))
    if(APPLE)
        include_directories(/usr/local/opt/llvm/include)
        link_directories(/usr/local/opt/llvm/lib)
    endif()
    add_executable(${targetName} ${ARGN})
    set_property(TARGET ${targetName} PROPERTY CXX_STANDARD 17)
    if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 9.0)
        if(APPLE)
            target_link_libraries(${targetName} -lc++fs)
        else()
            target_compile_options(${targetName} PRIVATE "-stdlib=libc++")
            target_link_libraries(${targetName} -stdlib=libc++ -lc++fs $<$<PLATFORM_ID:Linux>:rt>)
        endif()
    else()
        if(NOT APPLE)
            target_compile_options(${targetName} PRIVATE "-stdlib=libc++")
            target_link_libraries(${targetName} -stdlib=libc++)
        endif()
    endif()
    if(${CMAKE_SYSTEM_NAME} MATCHES "(SunOS|Solaris)")
        target_link_libraries(filesystem_test xnet)
    endif()
    if(${CMAKE_SYSTEM_NAME} MATCHES "Haiku")
        target_link_libraries(filesystem_test network)
    endif()
    target_compile_definitions(${targetName} PRIVATE USE_STD_FS)
endif()

if (CMAKE_COMPILER_IS_GNUCXX AND (CMAKE_CXX_COMPILER_VERSION VERSION_EQUAL 8.0 OR CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 8.0))
    add_executable(${targetName} ${ARGN})
    set_property(TARGET ${targetName} PROPERTY CXX_STANDARD 17)
    if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 9.0)
        target_link_libraries(${targetName} -lstdc++fs)
    endif()
    if(${CMAKE_SYSTEM_NAME} MATCHES "(SunOS|Solaris)")
        target_link_libraries(${targetName} xnet)
    endif()
    if(${CMAKE_SYSTEM_NAME} MATCHES "Haiku")
        target_link_libraries(${targetName} network)
    endif()
    target_compile_options(${targetName} PRIVATE $<$<BOOL:${CYGWIN}>:-Wa,-mbig-obj>)
    target_compile_definitions(${targetName} PRIVATE USE_STD_FS)
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES MSVC AND (CMAKE_CXX_COMPILER_VERSION VERSION_EQUAL 19.15 OR CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 19.15))
    add_executable(${targetName} ${ARGN})
    set_property(TARGET ${targetName} PROPERTY CXX_STANDARD 17)
    set_property(TARGET ${targetName} PROPERTY CXX_STANDARD_REQUIRED ON)
    target_compile_options(${targetName} PRIVATE "/Zc:__cplusplus")
    target_compile_definitions(${targetName} PRIVATE USE_STD_FS _CRT_SECURE_NO_WARNINGS)
endif()

endmacro()

macro(AddTestExecutableWithStdCpp cppStd)
    add_executable(filesystem_test_cpp${cppStd} ${ARGN})
    set_property(TARGET filesystem_test_cpp${cppStd} PROPERTY CXX_STANDARD ${cppStd})
    target_link_libraries(filesystem_test_cpp${cppStd} ghc_filesystem)
    if(${CMAKE_SYSTEM_NAME} MATCHES "(SunOS|Solaris)")
        target_link_libraries(filesystem_test_cpp${cppStd} xnet)
    endif()
    if(${CMAKE_SYSTEM_NAME} MATCHES "Haiku")
        target_link_libraries(filesystem_test_cpp${cppStd} network)
    endif()
    target_compile_options(filesystem_test_cpp${cppStd} PRIVATE
            $<$<BOOL:${EMSCRIPTEN}>:-s DISABLE_EXCEPTION_CATCHING=0>
            $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>>:-Wall -Wextra -Wshadow -Wconversion -Wsign-conversion -Wpedantic -Werror -Wno-error=deprecated-declarations>
            $<$<CXX_COMPILER_ID:GNU>:-Wall -Wextra -Wshadow -Wconversion -Wsign-conversion -Wpedantic -Wno-psabi -Werror -Wno-error=deprecated-declarations>
            $<$<CXX_COMPILER_ID:MSVC>:/WX /wd4996>
            $<$<BOOL:${CYGWIN}>:-Wa,-mbig-obj>
            $<$<BOOL:${GHC_COVERAGE}>:--coverage>)
    if(CMAKE_CXX_COMPILER_ID MATCHES MSVC)
        target_compile_definitions(filesystem_test_cpp${cppStd} PRIVATE _CRT_SECURE_NO_WARNINGS)
    endif()
    if(EMSCRIPTEN)
        set_target_properties(filesystem_test_cpp${cppStd} PROPERTIES LINK_FLAGS "-g4 -s DISABLE_EXCEPTION_CATCHING=0 -s ALLOW_MEMORY_GROWTH=1")
    endif()
    ParseAndAddCatchTests(filesystem_test_cpp${cppStd})
endmacro()
