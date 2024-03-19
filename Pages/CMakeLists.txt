add_custom_target(pages
    COMMAND sh ./GeneratePages.sh

    COMMENT "Running GeneratePages script..."
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
