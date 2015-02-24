/*
 * Queue.c
 *
 *  Created on: 2015年2月9日
 *      Author: star
 */

#include"Queue.h"
#include<stdlib.h>
#include<stdio.h>

/*构造一个空队列*/
Queue_t *InitQueue(void)
{
    Queue_t *pqueue = (Queue_t *)malloc(sizeof(Queue_t));
    if(pqueue!=NULL)
    {
        pqueue->head = NULL;
        pqueue->tail = NULL;
        pqueue->size = 0;
    }
    return pqueue;
}

/*销毁一个队列*/
void DestroyQueue(Queue_t *pqueue)
{
    if(IsEmpty(pqueue)!=1)
        ClearQueue(pqueue);
    free(pqueue);
}

/*清空一个队列*/
void ClearQueue(Queue_t *pqueue)
{
    while(IsEmpty(pqueue)!=1)
    {
        DeQueue(pqueue,NULL);
    }

}

/*判断队列是否为空*/
int IsEmpty( Queue_t *const pqueue)
{
    if(pqueue->head==NULL&&pqueue->tail==NULL&&pqueue->size==0)
        return 1;
    else
        return 0;
}

/*返回队列大小*/
int GetQueueSize( Queue_t *const pqueue)
{
    return pqueue->size;
}

/*返回队头元素*/
Node_t * GetQueueHead( Queue_t *const pqueue,Item_t *pitem)
{
    if(IsEmpty(pqueue)!=1&&pitem!=NULL)
    {
        pitem = pqueue->head->pdata;
    }
    return pqueue->head;
}

/*返回队尾元素*/

Node_t * GetQueueTail( Queue_t *const pqueue,Item_t *pitem)
{
    if(IsEmpty(pqueue)!=1&&pitem!=NULL)
    {
        pitem = pqueue->tail->pdata;
    }
    return pqueue->tail;
}

Node_t * InsertQueue(Queue_t *pqueue,Node_t *pnode)
{
	//Node * pnode = (Node *)malloc(sizeof(Node));
    if(pnode != NULL)
    {
        if(IsEmpty(pqueue))
        {
            pqueue->head = pnode;
        }
        else
        {
            pqueue->tail->next = pnode;
        }
        pqueue->tail = pnode;
        pqueue->size++;
    }
    return pnode;
}

/*将新元素入队*/
Node_t * EnQueue(Queue_t *pqueue,Item_t *pitem)
{
	Node_t * pnode = (Node_t *)malloc(sizeof(Node_t));
    if(pnode != NULL)
    {
        pnode->pdata = pitem;
        pnode->next = NULL;

        if(IsEmpty(pqueue))
        {
            pqueue->head = pnode;
        }
        else
        {
            pqueue->tail->next = pnode;
        }
        pqueue->tail = pnode;
        pqueue->size++;
    }
    return pnode;
}

/*队头元素出队*/
Node_t * DeQueue(Queue_t *pqueue,Item_t *pitem)
{
    Node_t * pnode = pqueue->head;
    if(IsEmpty(pqueue)!=1&&pnode!=NULL)
    {
        if(pitem!=NULL)
            pitem = pnode->pdata;
        pqueue->size--;
        pqueue->head = pnode->next;
        free(pnode);
        if(pqueue->size==0)
            pqueue->tail = NULL;
    }
    return pqueue->head;
}

/*遍历队列并对各数据项调用visit函数*/
void QueueTraverse(Queue_t *pqueue,void (*operate)())
{
    Node_t * pnode = pqueue->head;
    int i = pqueue->size;
    while(i--)
    {
    	operate(pnode->pdata);
        pnode = pnode->next;
    }

}


void print(Item_t i)
{
    printf("该节点元素为%d\n",i);
}
void queue_main()
{
    Queue_t *pq = InitQueue();
    int i;
    Item_t its;
    printf("0-9依次入队并输出如下：\n");
    for(i=0;i<10;i++)
    {
        EnQueue(pq,&its);
        GetQueueTail(pq,&its);
        printf("%d ",its);
    }

    printf("\n从队头到队尾遍历并对每个元素执行print函数：\n");
    QueueTraverse(pq,print);

    printf("队列中元素依次出队列并输出如下：\n");
    for(i=0;i<10;i++)
    {
        DeQueue(pq,&its);
        printf("%d ",its);
    }
    ClearQueue(pq);
    if(IsEmpty(pq))
        printf("\n将队列置空成功\n");
    DestroyQueue(pq);
    printf("队列已被销毁\n");
}

