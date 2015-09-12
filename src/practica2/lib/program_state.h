#ifndef _PROGRAM_STATE_H_
#define _PROGRAM_STATE_H_

/**
 * The status of the program.
 * Running all the time, until 'shutdown' is called
 */
typedef enum {
  RUNNING,
  STOPPED
} State;

#endif
