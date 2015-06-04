add_subdirectory(http)
add_subdirectory(inverter)
add_executable(invdaemon config.h invdaemon.c cfg.c ui.c utils.c server.c socket.c log.c)
target_link_libraries (invdaemon LINK_PUBLIC -pthread -lm http inverter)