PROJECT(Pheet)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

FILE(GLOB_RECURSE ALL_CPP_SRCS test/*.cpp)
FILE(GLOB_RECURSE Nonworking_Tests test/inarow/*.cpp test/uts/*.cpp  test/sor/*.cpp)
LIST(REMOVE_ITEM ALL_CPP_SRCS  ${Nonworking_Tests} )

include_directories( . )
add_executable (pheet_test ${ALL_CPP_SRCS})
target_link_libraries(pheet_test hwloc pthread )
set(CMAKE_CXX_FLAGS "-std=c++0x ")
SET_TARGET_PROPERTIES(pheet_test PROPERTIES RUNTIME_OUTPUT_DIRECTORY "bin")
