#ifndef _PROGRAM_STATE_H_
#define _PROGRAM_STATE_H_

/**
 * Notify `init` that the shell executed `exit` or `shutdown`
 */
typedef enum {
  MESSAGE_EXIT_SHELL,
  MESSAGE_SHUTDOWN_SHELL
} ShellMessage;

#endif
