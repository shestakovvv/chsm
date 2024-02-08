set(SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/src/chsm.c
)

set(INCLUDES
        ${CMAKE_CURRENT_LIST_DIR}/src
)

add_library(chsm STATIC ${SOURCES})

target_include_directories(chsm PUBLIC
        ${INCLUDES}
)