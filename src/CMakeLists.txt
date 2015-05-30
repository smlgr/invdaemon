add_executable(invdaemon config.h invdaemon.c cfg.c ui.c)
target_link_libraries (invdaemon -pthread)