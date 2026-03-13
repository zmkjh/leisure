// 2026/3/12
// resume.h
// jump to the last set resume point

#ifndef RESUME_H
#define RESUME_H

#include <setjmp.h>
#include <stdlib.h>

// size of the first block
const int __env_block_first_size = 2;

// stack of jmp_buf
typedef struct __env_block_tag {
    int      top;
    int      size;
    jmp_buf* buf;
    struct __env_block_tag* last;
    struct __env_block_tag* next;
} __env_block_t;

// block linked list of jmp_buf
struct {
    __env_block_t* envs_blocks_first;
    __env_block_t* envs_blocks_current;
} __jmp_stack;

// push a new block to the block linked list
void __jmp_stack_push_block() {
    if (__jmp_stack.envs_blocks_current->next != NULL) {
        __jmp_stack.envs_blocks_current = __jmp_stack.envs_blocks_current->next;
    } else {
        __env_block_t* new_block = (__env_block_t*)malloc(sizeof(__env_block_t));
        new_block->size = __jmp_stack.envs_blocks_current->size * 2;
        new_block->top  = -1;
        new_block->buf  = (jmp_buf*)malloc(sizeof(jmp_buf) * new_block->size);

        __jmp_stack.envs_blocks_current->next = new_block;
        new_block->last = __jmp_stack.envs_blocks_current;

        __jmp_stack.envs_blocks_current = new_block;
    }
}

// push a jmp_buf to the stack, allocating a new block if necessary
jmp_buf* __jmp_stack_push() {
    if (__jmp_stack.envs_blocks_current->top >= __jmp_stack.envs_blocks_current->size-1) {
        __jmp_stack_push_block();
    }
    __jmp_stack.envs_blocks_current->top++;
    return &(__jmp_stack.envs_blocks_current->buf[__jmp_stack.envs_blocks_current->top]);
}

// get the top jmp_buf from the stack
jmp_buf* __jmp_stack_top() {
    return &(__jmp_stack.envs_blocks_current->buf[__jmp_stack.envs_blocks_current->top]);
}

// initialize the jmp stack with a first block
__attribute__((constructor)) void __jmp_stack_init() {
    __env_block_t* new_block = (__env_block_t*)malloc(sizeof(__env_block_t));
    new_block->size = __env_block_first_size;
    new_block->top  = -1;
    new_block->buf  = (jmp_buf*)malloc(sizeof(jmp_buf) * new_block->size);
    new_block->last = NULL;
    new_block->next = NULL;

    __jmp_stack.envs_blocks_first   = new_block;
    __jmp_stack.envs_blocks_current = new_block;
}

// free the jmp stack block linked list
__attribute__((destructor)) void __jmp_stack_des() {
    __env_block_t* next = __jmp_stack.envs_blocks_first;
    while (next != NULL) {
        __env_block_t* cur = next;
        next = cur->next;
        free(cur->buf);
        free(cur);
    }
}

// pop a jmp_buf from the stack
void __jmp_stack_pop(int*) {
    __jmp_stack.envs_blocks_current->top--;
    if (__jmp_stack.envs_blocks_current->top <= -1) {
        if (__jmp_stack.envs_blocks_current->last != NULL)
            __jmp_stack.envs_blocks_current = __jmp_stack.envs_blocks_current->last;
    }
}

// set a resume point using the jmp stack
#define set_resume_point()                         \
    int __resume_point_flag##__COUNTER__           \
        __attribute__((cleanup(__jmp_stack_pop)))  \
    = setjmp(*__jmp_stack_push())

// jump to the most recent resume point
void jmp_resume_point() {
    longjmp(*__jmp_stack_top(), 0);
}

#endif // RESUME_H
