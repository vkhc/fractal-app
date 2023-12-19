
set(TARGET
    thread_pool
)

set(SOURCES
    thread_pool.h thread_pool.cpp
    task_queue.h task_queue.cpp
)

add_library(${TARGET} STATIC ${SOURCES})

target_include_directories(${TARGET} PUBLIC ${CMAKE_CURRENT_LIST_DIR})
