#ifndef SIGNALS_H
#define SIGNALS_H

#include "defines.h"
#include "logging.h"

#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * Initialize signal capture
 * @return Success of function
 */
int signalsInit();

/**
 * Handler for signals
 * @param signal Signal sent to program
 */
void handler(int signal);

#endif
