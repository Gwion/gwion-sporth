#include "plumber.h"

int sporth_randi(sporth_stack *stack, void *ud)
{
    plumber_data *pd = ud;
    SPFLOAT out;
    int seed;
    SPFLOAT min;
    SPFLOAT max;
    SPFLOAT cps;
    SPFLOAT mode;
    sp_randi *randi;

    switch(pd->mode) {
        case PLUMBER_CREATE:

#ifdef DEBUG_MODE
            fprintf(stderr, "randi: Creating\n");
#endif

            sp_randi_create(&randi);
            plumber_add_module(pd, SPORTH_RANDI, sizeof(sp_randi), randi);
            break;
        case PLUMBER_INIT:

#ifdef DEBUG_MODE
            fprintf(stderr, "randi: Initialising\n");
#endif

            if(sporth_check_args(stack, "fff") != SPORTH_OK) {
                fprintf(stderr,"Not enough arguments for randi\n");
                stack->error++;
                return PLUMBER_NOTOK;
            }
            cps = sporth_stack_pop_float(stack);
            max = sporth_stack_pop_float(stack);
            min = sporth_stack_pop_float(stack);
            randi = pd->last->ud;
            sp_randi_init(pd->sp, randi, pd->seed);
            sporth_stack_push_float(stack, 0);
            break;
        case PLUMBER_COMPUTE:
            if(sporth_check_args(stack, "fff") != SPORTH_OK) {
                fprintf(stderr,"Not enough arguments for randi\n");
                stack->error++;
                return PLUMBER_NOTOK;
            }
            cps = sporth_stack_pop_float(stack);
            max = sporth_stack_pop_float(stack);
            min = sporth_stack_pop_float(stack);
            randi = pd->last->ud;
            randi->min = min;
            randi->max = max;
            randi->cps = cps;
            sp_randi_compute(pd->sp, randi, NULL, &out);
            sporth_stack_push_float(stack, out);
            break;
        case PLUMBER_DESTROY:
            randi = pd->last->ud;
            sp_randi_destroy(&randi);
            break;
        default:
            fprintf(stderr, "randi: Uknown mode!\n");
            break;
    }
    return PLUMBER_OK;
}
