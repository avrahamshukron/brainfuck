#ifndef BF_H
#define BF_H

#define DEBUG 0

#define BRACKET_OPEN '['
#define BRACKET_CLOSE ']'
#define STEP_FORWARD 1
#define STEP_BACKWARD (-1)

#define DATA_PTR_INC '>'
#define DATA_PTR_DEC '<'
#define DATA_VAL_INC '+'
#define DATA_VAL_DEC '-'
#define DATA_VAL_OUT '.'
#define DATA_VAL_IN ','
#define JMP_IFZ_FWD '['
#define JMP_NZ_BWD ']'

#define DATA_SIZE (30 * 1024) // 30KB

enum status {
	OK,
	JUMPED
};

struct bf_context {
	char	*prog; /* points to the buffer holding the program */
	size_t	prog_size; /* the size of the program buffer */
	char	*pc; /* program counter */
	char	dp; /* data pointer */
	char	data[DATA_SIZE]; /* data buffer */
};

void bf_init(struct bf_context *ctx, char *prog, size_t prog_size);
void bf_run(struct bf_context *ctx);

#endif /* BF_H */
