add_subdirectory(http)
add_executable(invdaemon config.h invdaemon.c cfg.c ui.c utils.c)
target_link_libraries (invdaemon LINK_PUBLIC -pthread http)