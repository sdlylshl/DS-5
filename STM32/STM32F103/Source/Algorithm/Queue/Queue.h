/*
 * Queue.h
 *
 *  Created on: 2015年2月9日
 *      Author: star
 */


#ifndef Queue_H
#define Queue_H

//单链表实现的队列
//typedef unsigned int Item_t;
typedef struct _data{
	unsigned char flag;
	unsigned char time;
	unsigned char resend;
	unsigned char checksum;
	unsigned int id;
}Item_t;

//定义单链表节点
//typedef struct node * PNode;

//队列节点包含队列指针 和 数据成员
typedef struct __node
{
    Item_t * pdata;
    struct __node * next;
}Node_t;
//队列由队列节点组成
typedef struct __queue
{
	Node_t * head;
	Node_t * tail;
    int size;
}Queue_t;

//1.创建一个队列并初始化为空 Queue RecvQueue={0};
//2.创建一个节点 Node RecvNode[100];
//3.将节点添加进队列
/*构造一个空队列*/
Queue_t *InitQueue(void);

/*销毁一个队列*/
void DestroyQueue(Queue_t *pqueue);

/*清空一个队列*/
void ClearQueue(Queue_t *pqueue);

/*判断队列是否为空*/
int IsEmpty(Queue_t * const pqueue);

/*返回队列大小*/
int GetQueueSize(Queue_t * const pqueue);

/*返回队头元素*/
Node_t * GetQueueHead(Queue_t * const pqueue,Item_t *pitem);

/*返回队尾元素*/
Node_t * GetQueueTail(Queue_t * const pqueue,Item_t *pitem);
/*将新节点入队尾*/
Node_t * InsertQueue(Queue_t *pqueue,Node_t *pnode);
/*将新元素入队尾*/
Node_t * EnQueue(Queue_t *pqueue,Item_t *pitem);

/*队头元素出队*/
Node_t * DeQueue(Queue_t *pqueue,Item_t *pitem);

/*遍历队列并对各数据项调用visit函数*/
void QueueTraverse(Queue_t *pqueue,void (*operate)());

#endif
