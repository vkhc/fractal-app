
set(thisLib utils)

add_library(${thisLib} INTERFACE timer.h
                                 helpers.h)

#set_target_properties(thisLib PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(${thisLib} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})