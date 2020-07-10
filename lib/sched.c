#include <env.h>
#include <pmap.h>
#include <printf.h>

/* Overview:
 *  Implement simple round-robin scheduling.
 *
 *
 * Hints:
 *  1. The variable which is for counting should be defined as 'static'.
 *  2. Use variable 'env_sched_list', which is a pointer array.
 *  3. CANNOT use `return` statement!
 */
/*** exercise 3.14 ***/



void sched_yield(void)
{
    static int count = 0; // remaining time slices of current env
    static int point = 0; // current env_sched_list index
	static struct Env *e = NULL;
    int judge = 0;
    const bingo = 1;
    int r;
    /*  hint:
     *  1. if (count==0), insert `e` into `env_sched_list[1-point]`
     *     using LIST_REMOVE and LIST_INSERT_TAIL.
     *  2. if (env_sched_list[point] is empty), point = 1 - point;
     *     then search through `env_sched_list[point]` for a runnable env `e`, 
     *     and set count = e->env_pri
     *  3. count--
     *  4. env_run()
     *
     *  functions or macros below may be used (not all):
     *  LIST_INSERT_TAIL, LIST_REMOVE, LIST_FIRST, LIST_EMPTY
     */

    
//     while((count <= 0) || (e && (e->env_status != ENV_RUNNABLE))){
// 		if(LIST_EMPTY(&env_sched_list[point])){
// 			point = 1 - point;
// 		}
// 		if(e = LIST_FIRST(&env_sched_list[point])){
// 			int i = 1 - point;
// 			LIST_REMOVE(e, env_sched_link);
// 			LIST_INSERT_HEAD(&env_sched_list[i], e, env_sched_link);
// 			count = e->env_pri;
// 		}
// 	}
// 	count--;
// 	env_run(e);
    
    if ((!curenv) || (!count) || (curenv && curenv->env_status != ENV_RUNNABLE)) {
        if (curenv) {
            int i = 1 - point;
            LIST_INSERT_HEAD(&env_sched_list[i], curenv, env_sched_link);
        }
        while (bingo) {
            if (r = LIST_EMPTY(&env_sched_list[point])) {
                if (judge) {
                    return;
                }
                judge = 1 - judge;
                point = 1 - point;
                continue;
            }
            if (e = LIST_FIRST(&env_sched_list[point])) {
                LIST_REMOVE(e, env_sched_link);
                if (e->env_status == ENV_NOT_RUNNABLE) {
                    int j = 1 - point;
                    LIST_INSERT_HEAD(&env_sched_list[j], e, env_sched_link);
                }
                if (e->env_status == ENV_RUNNABLE) {
                    count = e->env_pri - 1;
                    env_run(e);
                    break;
                }
            }
        }
    }
    count--;
    env_run(curenv);
    

}
