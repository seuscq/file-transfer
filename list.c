#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define PRINTARRAY(a, n) 	\
do {				\
	int i;				\
	printf(#a ": ");		\
	for(i = 0; i < n; i++)		\
		printf("%d\t", a[i]);	\
	printf("\n");			\
}while(0)

struct ListNode {
	struct ListNode *next;
	int i;
};

int list_length(struct ListNode *list)
{
	int i = 0;
	struct ListNode *iter = list;

	while (iter) {
		iter = iter->next;
		i++;
	}
	return i;
}

struct ListNode *list_push_tail(struct ListNode *list, int i)
{
	struct ListNode *iter = list;
	struct ListNode *new = malloc(sizeof(struct ListNode));

	memset(new, 0, sizeof(*new));
	new->i = i;
	new->next = NULL;
	
	if (!iter)
		return new;

	while (iter->next) {
		iter = iter->next;
	} /* now iter->next = NULL;, iter point to the last elem */

	iter->next = new;
	return list;
}

/* in-place version
 * 4-step process */
struct ListNode *list_reverse(struct ListNode *list)
{
	struct ListNode *iter = list;
	struct ListNode *prev = NULL;
	struct ListNode *curr = NULL;

	//if(!list) return NULL;

	while(iter) {
		curr = iter->next;
		iter->next = prev;
		prev = iter;
		iter = curr;
	}
	return prev;
}

struct ListNode *list_get_last_k(struct ListNode *list, int k) 
{
	struct ListNode *iter1, *iter2;
	int i;

	if (k > list_length(list)) return NULL;

	iter1 = list;
	for (i = 1; i < k; i++) iter1 = iter1->next;
	/* now iter1 points to the kth elem */

	iter2 = list;
	while (iter1->next) {
		iter1 = iter1->next;
		iter2 = iter2->next;
	}

	return iter2;
}

void list_print_all(struct ListNode *list)
{
	struct ListNode *iter = list;

	if (!iter) return; /* empty list */

	printf("l: ");
	while (iter) {
		printf("%d\t", iter->i);
		iter = iter->next;
		if (!iter)
			break;
	}
	puts("");
}

int main()
{
	int a[8];
	struct ListNode *list = NULL;
	struct ListNode *rev;
	
	srand(time(NULL));
	int i;
	for (i = 0; i < sizeof(a)/sizeof(a[0]); i++)
		a[i] = rand() % 1000;

	PRINTARRAY(a, sizeof(a)/sizeof(a[0]));

	for (i = 0; i < sizeof(a)/sizeof(a[0]); i++)
		list = list_push_tail(list, a[i]);

	list_print_all(list);

	//rev = list_reverse(list);
	//printf("length of rev is %d\n", list_length(rev));
	//list_print_all(rev);

	
	//rev = list_get_last_k(rev, 5);
	//printf("length of rev is %d\n", list_length(rev));
	//list_print_all(rev);

	//list = list_get_last_k(list, sizeof(a)/sizeof(a[0]));
	//printf("length of list is %d\n", list_length(list));
	//list_print_all(list);

}
