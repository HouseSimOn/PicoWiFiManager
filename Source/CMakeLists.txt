add_subdirectory(HAL)
add_subdirectory(WebServer)
add_subdirectory(Logger)
add_subdirectory(Commands)
add_subdirectory(WiFiManager)

list(
    APPEND INCLUDE_DIRS
    "${CMAKE_CURRENT_SOURCE_DIR}/HAL"
    "${CMAKE_CURRENT_SOURCE_DIR}/WebServer"
    "${CMAKE_CURRENT_SOURCE_DIR}/Logger"
    "${CMAKE_CURRENT_SOURCE_DIR}/Commands"
    "${CMAKE_CURRENT_SOURCE_DIR}/WiFiManager"
)

list(
    APPEND SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/main.c"
)

set(SOURCES "${SOURCES}" PARENT_SCOPE)
set(INCLUDE_DIRS "${INCLUDE_DIRS}" PARENT_SCOPE)
