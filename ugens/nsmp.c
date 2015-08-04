#include "plumber.h"
#include "macros.h"

int sporth_nsmp(sporth_stack *stack, void *ud) 
{
    if(stack->error > 0) return PLUMBER_NOTOK;

    plumber_data *pd = ud;
    SPFLOAT out = 0, trig = 0, index = 0;
    char *wav, *ini;
    sp_nsmp *nsmp;
    switch(pd->mode){
        case PLUMBER_CREATE:
#ifdef DEBUG_MODE
            printf("Creating nsmp function... \n");
#endif
            sp_nsmp_create(&nsmp);
            plumber_add_module(pd, SPORTH_NSMP, sizeof(sp_nsmp), nsmp);
            break;
        case PLUMBER_INIT:
            if(sporth_check_args(stack, "ffss") != SPORTH_OK) {
                stack->error++;
                fprintf(stderr, "Invalid arguments for nsmp.\n");
                return PLUMBER_NOTOK;
            }
            nsmp = pd->last->ud;

            wav = sporth_stack_pop_string(stack);
            ini = sporth_stack_pop_string(stack);
            index = sporth_stack_pop_float(stack);
            trig = sporth_stack_pop_float(stack);

            if(sp_nsmp_init(pd->sp, nsmp, wav, ini) == SP_NOT_OK) {
                fprintf(stderr, "nsmp: there was an error opening the files\n");
                stack->error++;    
            };
            sporth_stack_push_float(stack, 0.0);
            free(wav);
            free(ini);
            break;

        case PLUMBER_COMPUTE:
            nsmp = pd->last->ud;
            if(sporth_check_args(stack, "ff") != SPORTH_OK) {
                stack->error++;
                fprintf(stderr, "Invalid arguments for nsmp.\n");
                return PLUMBER_NOTOK;
            }

            index = sporth_stack_pop_float(stack);
            trig = sporth_stack_pop_float(stack);
            nsmp->index = (uint32_t) index;
            sp_nsmp_compute(pd->sp, nsmp, &trig, &out);
            sporth_stack_push_float(stack, out);
            break;
        case PLUMBER_DESTROY:
            nsmp = pd->last->ud;
            sp_nsmp_destroy(&nsmp);
            break;
        default:
            printf("Error: Unknown mode!"); 
            stack->error++;
            return PLUMBER_NOTOK;
            break;
    }   
    return PLUMBER_OK;
}
