#include "stdio.h"
#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
/**
 * container_of - cast a member of a structure out to the containing structure
 * @ptr:    the pointer to the member.
 * @type:   the type of the container struct this is embedded in.
 * @member: the name of the member within the struct.
 *
 */
#define container_of(ptr, type, member) ({          \
	const typeof(((type *)0)->member)*__mptr = (ptr);    \
		     (type *)((char *)__mptr - offsetof(type, member)); })

/*
 * Simple doubly linked list implementation.
 *
 * Some of the internal functions ("__xxx") are useful when
 * manipulating whole lists rather than single entries, as
 * sometimes we already know the next/prev entries and we can
 * generate better code by using them directly rather than
 * using the generic single-entry routines.
 */

struct list_head {
	struct list_head *next, *prev;
};

/**
 * list_entry - get the struct for this entry
 * @ptr:	the &struct list_head pointer.
 * @type:	the type of the struct this is embedded in.
 * @member:	the name of the list_head within the struct.
 */
#define list_entry(ptr, type, member) \
	container_of(ptr, type, member)

#ifndef ARCH_HAS_PREFETCH
#define prefetch(x) __builtin_prefetch(x)
#endif

/**
 * list_for_each_entry  -   iterate over list of given type
 * @pos:    the type * to use as a loop cursor.
 * @head:   the head for your list.
 * @member: the name of the list_struct within the struct.
 */
#define list_for_each_entry(pos, head, member)              \
    for (pos = list_entry((head)->next, typeof(*pos), member);   \
         prefetch(pos->member.next), &pos->member != (head);  \
         pos = list_entry(pos->member.next, typeof(*pos), member))

/*
 * Simple doubly linked list implementation.
 *
 * Some of the internal functions ("__xxx") are useful when
 * manipulating whole lists rather than single entries, as
 * sometimes we already know the next/prev entries and we can
 * generate better code by using them directly rather than
 * using the generic single-entry routines.
 */

#define LIST_HEAD_INIT(name) { &(name), &(name) }

#define LIST_HEAD(name) \
	struct list_head name = LIST_HEAD_INIT(name)

static inline void INIT_LIST_HEAD(struct list_head *list)
{
	list->next = list;
	list->prev = list;
}

typedef int member;
struct example {
        member a;
        struct list_head node;
        member b;
};

static inline void __list_add(struct list_head *new, struct list_head *prev, struct list_head *next)
{
	next->prev = new;
	new->next = next;
	new->prev = prev;
	prev->next = new;
}
static inline void list_add(struct list_head *new, struct list_head *head)
{
   __list_add(new, head, head->next);
}

int main()
{
	LIST_HEAD(my_listhead);

	int aa = 3;
	printf("sizeof int is %ld\n",sizeof(int));
	printf("sizeof unsigned long is %ld\n",sizeof(unsigned long));
	void *p = 0;
	printf("sizeof pointer is %ld\n",sizeof(p));
	
	struct example test1;
	test1.a = 1;
	test1.b = 2;
	list_add(&test1.node, &my_listhead);

	struct example test2;
	test2.a = 3;
	test2.b = 4;
	list_add(&test2.node, &test1.node);

	struct example test3;
	test3.a = 5;
	test3.b = 6;
	list_add(&test3.node, &test2.node);

	struct example test4;
	test4.a = 7;
	test4.b = 8;
	list_add(&test4.node, &test3.node);

	struct example test5;
	test5.a = 9;
	test5.b = 10;
	list_add(&test5.node, &test3.node);

	struct example* tmp;
	list_for_each_entry(tmp, &my_listhead, node) printf( "tmp.a = %d\n",tmp->a);
	return 0;
}
