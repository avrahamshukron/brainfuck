#include <stdio.h>

#include "bf.h"
#include "debug.h"

void bf_init(struct bf_context *ctx, char *prog, size_t prog_size)
{
	debug("init\n");
	int i;

	ctx->prog = prog;
	ctx->prog_size = prog_size;
	ctx->pc = prog; /* Sets the PC to the start of the program. */
	ctx->dp = ctx->data;
	for (i = 0; i < DATA_SIZE; i++)
		ctx->data[i] = 0;
}

static inline int bf_cmd_index(struct bf_context *ctx)
{
	return ctx->pc - ctx->prog;
}

static inline int bf_data_index(struct bf_context *ctx)
{
	return ctx->dp - ctx->data;
}

static inline enum status bf_set_value(struct bf_context *ctx, char value)
{
	int dp_index = bf_data_index(ctx);

	if (dp_index < 0 || dp_index >= DATA_SIZE)
		return DP_OUT_OF_BOUNDS;

	*ctx->dp = value;
	return OK;
}

static inline int bf_has_ended(struct bf_context *ctx)
{
	int offset = bf_cmd_index(ctx);

	return offset > ctx->prog_size || offset < 0;
}

static inline void bf_move_pc(struct bf_context *ctx, int steps)
{
	ctx->pc = ctx->pc + steps;
}

static inline void bf_move_dp(struct bf_context *ctx, int steps)
{
	ctx->dp = ctx->dp + steps;
}

enum status bf_jump(struct bf_context *ctx, char starter, char ender, int step)
{
	int stack = 1;

	while (stack > 0) {
		bf_move_pc(ctx, step);
		if (bf_has_ended(ctx))
			return PC_OUT_OF_BOUNDS;

		if (*ctx->pc == starter)
			stack++;
		else if (*ctx->pc == ender)
			stack--;
	}
	return JUMPED;
}

enum status bf_jump_forward(struct bf_context *ctx)
{
	if (*ctx->dp == 0)
		return bf_jump(ctx, BRACKET_OPEN, BRACKET_CLOSE, STEP_FORWARD);

	return OK;
}

enum status bf_jump_backward(struct bf_context *ctx)
{
	if (*ctx->dp != 0)
		return bf_jump(ctx, BRACKET_CLOSE, BRACKET_OPEN, STEP_BACKWARD);

	return OK;
}

enum status bf_act(struct bf_context *ctx)
{
	enum status ret = OK;
	char c = *ctx->pc;
	char d = *ctx->dp;

	debug("C[%i] = %c, D[%i] = %i\n",
		bf_cmd_index(ctx), c, bf_data_index(ctx), d);
	switch (c) {
	case DATA_PTR_INC:
		bf_move_dp(ctx, STEP_FORWARD);
		break;
	case DATA_PTR_DEC:
		bf_move_dp(ctx, STEP_BACKWARD);
		break;
	case DATA_VAL_INC:
		ret = bf_set_value(ctx, *ctx->dp + 1);
		break;
	case DATA_VAL_DEC:
		ret = bf_set_value(ctx, *ctx->dp - 1);
		break;
	case DATA_VAL_IN:
		*ctx->dp = getchar();
		debug("Read character: %c\n", *ctx->dp);
		break;
	case DATA_VAL_OUT:
		putchar(d);
		break;
	case JMP_IFZ_FWD:
		ret = bf_jump_forward(ctx);
		break;
	case JMP_NZ_BWD:
		ret = bf_jump_backward(ctx);
		break;
	default:
		debug("Ignoring non-brainfuck character: %c\n", c);
		/* Ignore any non-command char. */
		break;
	}
	return ret;
}

enum status bf_run(struct bf_context *ctx)
{
	enum status result;

	debug("running\n");
	while (!bf_has_ended(ctx)) {
		result = bf_act(ctx);
		if (result == OK)
			bf_move_pc(ctx, STEP_FORWARD);
		else if (result == JUMPED)
			continue;
		else if (result == DP_OUT_OF_BOUNDS) {
			printf("Data pointer out of bounds!\n");
			break;
		} else if (result == PC_OUT_OF_BOUNDS) {
			printf("Program counter out of bounds!\n");
			break;
		}
	}
	debug("finished\n");
	return result;
}
