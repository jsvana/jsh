#include "signals.h"

int signals_init() {
	sig_t ret;

	ret = signal(SIGINT, handler);

	if (ret == SIG_ERR) {
		term_err("Error attaching handler (%d)\n", errno);
	}

	return TRUE;
}

void handler(int signal) {
	if (signal == 2) {
		fprintf(stdout, "\nExiting\n");
		exit(0);
	}
}

