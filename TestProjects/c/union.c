#include "stdio.h"
struct align {
char *ptr1;
char c;
char *ptr2;
};


typedef unsigned short uint16_t;
struct rte_mbuf{
char *ptr1;
char c;
char *ptr2;
};

struct ixgbe_tx_entry_v {
	struct rte_mbuf *mbuf; /**< mbuf associated with TX desc, if any. */
};

struct ixgbe_tx_entry {
	struct rte_mbuf *mbuf; /**< mbuf associated with TX desc, if any. */
	uint16_t next_id; /**< Index of next descriptor in ring. */
	uint16_t last_id; /**< Index of last scattered descriptor. */
};

static union {
	struct ixgbe_tx_entry *sw_ring; /**< address of SW ring for scalar PMD. */
	struct ixgbe_tx_entry_v *sw_ring_v; /**< address of SW ring for vector PMD */
};


struct ixgbe_tx_queue {
	/** TX ring virtual address. */
	union {
		struct ixgbe_tx_entry *sw_ring; /**< address of SW ring for scalar PMD. */
		struct ixgbe_tx_entry_v *sw_ring_v; /**< address of SW ring for vector PMD */
	};
	uint16_t            nb_tx_desc;    /**< number of TX descriptors. */
	uint16_t            tx_tail;      /**< current value of TDT reg. */
};

#define A(x)  T_##x
#define B(x)  #x
//#define C(x)  #@x

//A(aa)
//B(aa)
//C(aa)
int main()
{
//	struct align a;
//	printf("&a.ptr1 is 0x%x\n",&a.ptr1);
//	printf("&a.c    is 0x%x\n",&a.c);
//	printf("&a.ptr2 is 0x%x\n",&a.ptr2);

	struct ixgbe_tx_queue q_a;
	q_a.sw_ring = 0;
	q_a.sw_ring_v = 0;
	q_a.sw_ring = malloc(sizeof(struct ixgbe_tx_entry)*100);
	printf("q_a.sw_ring is 0x%x\n",q_a.sw_ring);
	printf("q_a.sw_ring_v is 0x%x\n",q_a.sw_ring_v);
	q_a.sw_ring_v = malloc(sizeof(struct ixgbe_tx_entry)*100);
	printf("q_a.sw_ring is 0x%x\n",q_a.sw_ring);
	printf("q_a.sw_ring_v is 0x%x\n",q_a.sw_ring_v);
	printf("q_a.sw_ring[0] is 0x%x\n",q_a.sw_ring[0]);
	printf("q_a.sw_ring_v[0] is 0x%x\n",q_a.sw_ring_v[0]);
	printf("&q_a.sw_ring[0] is 0x%x\n",&q_a.sw_ring[0]);
	printf("&q_a.sw_ring_v[0] is 0x%x\n",&q_a.sw_ring_v[0]);
	
	return 0;
}
