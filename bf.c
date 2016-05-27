#include <stdio.h>

#include "bf.h"
#include "debug.h"

void bf_init(struct bf_context *ctx, char *prog, size_t prog_size)
{
	debug("init\n");
	int i;
	ctx->prog = prog;
	ctx->prog_size = prog_size;
	ctx->pc = prog; // Sets the PC to the start of the program.
	ctx->dp = ctx->data;
	for (i = 0; i < DATA_SIZE; i++) {
		ctx->data[i] = 0;
	}
}

static inline int bf_cmd_index(struct bf_context *ctx)
{
	return ctx->pc - ctx->prog;
}

static inline int bf_data_index(struct bf_context *ctx)
{
	return ctx->dp - ctx->data;
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

void bf_jump(struct bf_context *ctx, char starter, char ender, int step)
{
	int stack = 1;
	while (stack > 0 && !bf_has_ended(ctx)) {
		bf_move_pc(ctx, step);
		if (*ctx->pc == starter) {
			stack++;
		} else if (*ctx->pc == ender) {
			stack--;
		}
	}
}

enum status bf_jump_forward(struct bf_context *ctx)
{
	if (*ctx->dp == 0) {
		bf_jump(ctx, BRACKET_OPEN, BRACKET_CLOSE, STEP_FORWARD);
		return JUMPED;
	}
	return OK;
}

enum status bf_jump_backward(struct bf_context *ctx)
{
	if (*ctx->dp != 0) {
		bf_jump(ctx, BRACKET_CLOSE, BRACKET_OPEN, STEP_BACKWARD);
		return JUMPED;
	}
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
		(*ctx->dp)++;
		break;
	case DATA_VAL_DEC:
		(*ctx->dp)--;
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
		// Ignore any non-command char.
		break;
	}
	return ret;
}

void bf_run(struct bf_context *ctx)
{
	debug("running\n");
	while (!bf_has_ended(ctx)) {
		if (bf_act(ctx) == OK) {
			bf_move_pc(ctx, STEP_FORWARD);
		}
	}
	debug("finished\n");
}
