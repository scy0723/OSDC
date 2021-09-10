#include "sched.h"
#include "linux/random.h"   //random priority number
#include"linux/list_sort.h"   //sort by prio

unsigned int starving;

void init_myprio_rq(struct myprio_rq *myprio_rq)

{
    printk(KERN_INFO "***[MYPRIO] Myprio class is online \n");


    myprio_rq->nr_running=0;


    INIT_LIST_HEAD(&myprio_rq->queue);

}


static void update_curr_myprio(struct rq *rq)

{

}
static int cmp(void *p, struct list_head* a, stuct list_head*b)  //compare tool
{
	struct sched_myprio_entity *aa, *bb;
	aa=container_of(a,struct sched_myprio_entity,run_list);
	bb=container_of(b, struct sched_myprio_entity, run_list);
	int r= bb->prio-aa->prio;
	return r;
}

static void enqueue_task_myprio(struct rq *rq, struct task_struct *p, int flags) 

{
    	struct sched_myprio_entity *myprio_se=&p->myprio;
	if(!starving)  //starving == 0
		starving= p->pid;
	if(starving==p->pid) 
		myprio_se->prio=1;	
	else{ //random priority
		unsigned int ran;
		ran=get_random_bytes(&ran, sizeof(ran));
		myprio_se->prio=ran;
	
	}
    	list_add_tail(&p->myprio_se.run_list, &rq->myprio.queue);
	list_sort(NULL,&rq->myprio.queue,cmp); //sort using cmp
    	rq->myprio.nr_running++;

    /* print myprio enqueue */
	printk("[MYPRIO] enqueue: nr_running%d, pid%d, prio=%d\n", &rq->myprio.nr_running, p->pid, myprio_se->prio);
}


static void dequeue_task_myprio(struct rq *rq, struct task_struct *p, int flags)
{
    struct sched_myprio_entity *myprio_se=&p->myprio;    

        list_del(&p->myprio.run_list);

        rq->myprio.nr_running--;

        /* print myprio dequeue enqueue */
	printk("[MYPRIO] dequeue: nr_running%d, pid=%d, priority=%d\n",rq->myprio.nr_running,p->pid, myprio_se->prio);
      

}


static void check_preempt_curr_myprio(struct rq *rq, struct task_struct *p, int flags) 

{ 

}


struct task_struct *pick_next_task_myprio(struct rq *rq, struct task_struct *prev)

{ 
    struct task_struct *nxt; 
    struct list_head *headp;
    struct sched_myprio_entity *myprio_se = NULL;

    if( list_empty(rq->myprio.queue.next) )  //nothing, return null

    {
        return NULL;
    }

    else  ////////////aging code////////////////

    {


	list_sort(NULL,&rq->myprio.queue,cmp);
	list_for_each_entry(myprio_se, &rq->myprio.queue,run_list){ //list traverse macro
		struct task_struct * t= container_of(myprio_se, struct task_struct, myprio);	
		printk("***[MYPRIO] pid=%d, prio=%d\n", t->pid, myprio_se->prio);
	}
/////////////////////////////////////
        nxt = container_of(myprio_se, struct task_struct, myprio);
        /* print myprio pick_next_task */
	printk("***[MYPRIO] pick_next_task: prev->pid:%d, next->pid=%d, nr_running=%d, prio=%d\n", prev->pid, nxt

        return nxt;

    }

}


static void put_prev_task_myprio(struct rq *rq, struct task_struct *p) 

{


    printk(KERN_INFO "\t***[MYPRIO] put_prev_task: do nothing, p->pid=%d\n", p->pid);

}


static int select_task_rq_myprio(struct task_struct *p, int cpu, int sd_flag, int flags)

{


    return task_cpu(p);

}


static void set_curr_task_myprio(struct rq *rq) 

{ 

}


static void task_tick_myprio(struct rq *rq, struct task_struct *p, int queued) 

{ 
	update_curr_myprio(rq);
}


static void prio_changed_myprio(struct rq *rq, struct task_struct *p, int oldprio) 

{ 

}

/* This routine is called when a task migrates between classes */


static void switched_to_myprio(struct rq *rq, struct task_struct *p)

{
    resched_curr(rq);

}

const struct sched_class myprio_sched_class = {

    .next               = &fair_sched_class,


    .enqueue_task       = enqueue_task_myprio,


    .dequeue_task       = dequeue_task_myprio,


    .check_preempt_curr = check_preempt_curr_myprio,


    .pick_next_task     = pick_next_task_myprio,


    .put_prev_task      = put_prev_task_myprio,


#ifdef CONFIG_SMP

    .select_task_rq     = select_task_rq_myprio,

#endif


    .set_curr_task      = set_curr_task_myprio,


    .task_tick          = task_tick_myprio,


    .prio_changed       = prio_changed_myprio,


    .switched_to        = switched_to_myprio,


    .update_curr        = update_curr_myprio,


};
