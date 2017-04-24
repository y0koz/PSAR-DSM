#ifndef DSM_MEMORY_H
#define DSM_MEMORY_H

#include "list.h"

#define READERS_INITIAL_CAPACITY 4
#define MASTER_NODE -8

typedef struct dsm_page_request_s
{
	int sockfd;
	int rights;
} dsm_page_request_t;

typedef struct dsm_page_s
{
	int protection;
	pthread_mutex_t mutex_page;
	pthread_cond_t cond_not_uptodate;
	unsigned short uptodate;
	int write_owner;
	/* Following fields are used by master node only */
	list_t *requests_queue;
	list_t *current_readers_queue;
	unsigned short invalidate_sent;
} dsm_page_t;

typedef struct dsm_memory_s
{
	size_t pagesize;
	size_t page_count;
	void* base_addr;
	dsm_page_t *pages;
} dsm_memory_t;

void dsm_memory_init(dsm_memory_t *dsm_mem, size_t pagesize, size_t page_count, 
	unsigned short is_master);

void dsm_memory_destroy(dsm_memory_t *dsm_mem);

int dsm_add_reader(dsm_memory_t *dsm_mem, unsigned int page_idx, int node_fd);

#endif