/******************************************************************************
 * packlist.h
 * 
 * Copyright (c) 2013 by Li.Hua <lihua_338@163.com>.
 * 
 * DESCRIPTION: - 
 * 
 * Modification history
 * ----------------------------------------------------------------------------
 * Date         Version  Author       History
 * ----------------------------------------------------------------------------
 * 2013-08-04   1.0.0    Li.Hua       written
 ******************************************************************************/

#include <stdint.h>
#include <stdio.h>
#include "list.h"
#include "circ_buf.h"
#ifndef __PACK_LIST_H__
#define __PACK_LIST_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


typedef struct tagPackList PackList_t;

struct tagPackList {
	struct list_head	entry;
	struct circ_buf		circ;
	char				data[1<<16];
};

static inline void packlist_init(PackList_t *pl)
{
	INIT_LIST_HEAD(&pl->entry);
	pl->circ.buf  = pl->data;
	pl->circ.head = pl->circ.tail = 0;
}

static int packlist_cnt(PackList_t *pl)
{
	struct circ_buf	*circ = &pl->circ;
	return CIRC_CNT(circ->head, circ->tail, sizeof(pl->data));
}

static inline void packlist_put(PackList_t *pl, struct list_head *head)
{
	list_add_tail(&pl->entry, head);
}

static inline void packlist_insert(PackList_t *pl, struct list_head *head)
{
	list_add(&pl->entry, head);
}

static inline PackList_t *packlist_get(struct list_head * head)
{
	PackList_t *pl = NULL;
	
	if (list_empty(head))
		return pl;

	pl = list_first_entry(head, PackList_t, entry);
	list_del(&pl->entry);
	return pl;
}

void hexdump(const void *data, uint32_t len);
#define Debuger(fmt, arg...)	//printf("\r"fmt, ##arg)
//printf(fmt, ##arg)


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __SERVER_PACK_H__ */

