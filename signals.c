#include "signals.h"

extern int cpid;

int signalsInit() {
	sig_t ret;

	ret = signal(SIGINT, handler);

	if (ret == SIG_ERR) {
		TERM_ERR("Error attaching handler (%d)\n", errno);
	}

	return TRUE;
}

void handler(int signal) {
	if (signal == 2) {
		if (cpid > 0) {
			kill(cpid, SIGINT);
		}
		/*
		fprintf(stdout, "\nExiting...\n");
		exit(0);
		*/
	}
}

