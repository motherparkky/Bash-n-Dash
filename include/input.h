#ifndef BASH_N_DASH_INPUT_H
#define BASH_N_DASH_INPUT_H

#include <mqueue.h>

#define MQ_NAME "/bash_n_dash_mq"

typedef struct { char key; } InputMsg;

/* Child process entry */
void input_child_loop(void);

#endif
