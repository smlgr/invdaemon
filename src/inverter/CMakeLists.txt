add_library(inverter inverter.c solarmax.c)
target_include_directories(inverter PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})