add_library(http http.c response.c)
target_include_directories(http PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})