# Find all source files in a single current directory
# Save the name to DIR_ePaper_SRCS
aux_source_directory(. DIR_ePaper_SRCS)

include_directories(../Config)

# Generate the link library
add_library(ePaper ${DIR_ePaper_SRCS})
target_link_libraries(ePaper PUBLIC Config)

# Expose the current directory as an include path for consumers of this library
target_include_directories(ePaper PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})