cmake_minimum_required (VERSION 3.11)
set(CMAKE_CXX_STANDARD 17)
SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ldl -lpthread")
project(grammarsolver)

include_directories(lib/StanfordCPPLib)
file(GLOB stanford_files
        "lib/StanfordCPPLib/graphics/*.h"
        "lib/StanfordCPPLib/graphics/*.cpp"
        "lib/StanfordCPPLib/util/*.h"
        "lib/StanfordCPPLib/util/*.cpp"
        "lib/StanfordCPPLib/system/*.h"
        "lib/StanfordCPPLib/system/*.cpp"
        "lib/StanfordCPPLib/collections/*.h"
        "lib/StanfordCPPLib/collections/*.cpp"
        "lib/StanfordCPPLib/private/*.h"
        "lib/StanfordCPPLib/private/*.cpp"
        "lib/StanfordCPPLib/io/*.h"
        "lib/StanfordCPPLib/io/*.cpp"
        "src/grammarsolver.h"
        "src/grammarsolver.cpp"
        )
add_library(stanford ${stanford_files})

set(grammarsolver_SOURCES src/grammarmain.cpp)
link_libraries(stanford)
add_executable(grammarsolver ${grammarsolver_SOURCES})

target_link_libraries(${PROJECT_NAME}
PUBLIC ${Boost_STACKTRACE_BASIC_LIBRARY}
PUBLIC ${Boost_STACKTRACE_BACKTRACE_LIBRARY}
PUBLIC ${Boost_STACKTRACE_ADDR2LINE_LIBRARY}
PUBLIC ${Boost_STACKTRACE_NOOP_LIBRARY}
PUBLIC pthread dl
)
