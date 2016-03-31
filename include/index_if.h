/*
 * CDDL HEADER START
 *
 * The contents of this file are subject to the terms of the
 * Common Development and Distribution License (the "License").
 * You may not use this file except in compliance with the License.
 *
 * You can obtain a copy of the license at usr/src/OPENSOLARIS.LICENSE
 * or http://www.opensolaris.org/os/licensing.
 * See the License for the specific language governing permissions
 * and limitations under the License.
 *
 * When distributing Covered Code, include this CDDL HEADER in each
 * file and include the License file at usr/src/OPENSOLARIS.LICENSE.
 * If applicable, add the following below this CDDL HEADER, with the
 * fields enclosed by brackets "[]" replaced with your own identifying
 * information: Portions Copyright [yyyy] [name of copyright owner]
 *
 * CDDL HEADER END
 */
/*******************************************************************************

            Copyright(C), 2016~2019, axen.hook@foxmail.com
********************************************************************************
File Name: INDEX_IF.H
Author   : axen.hook
Version  : 1.00
Date     : 02/Mar/2016
Description: 
Function List: 
    1. ...: 
History: 
    Version: 1.00  Author: axen.hook  Date: 02/Mar/2016
--------------------------------------------------------------------------------
    1. Primary version
*******************************************************************************/
#ifndef __INDEX_IF_H__
#define __INDEX_IF_H__

#include "avl.h"
#include "os_adapter.h"
#include "os_list_double.h"
#include "os_queue.h"
#include "os_disk_if.h"
#include "os_utils.h"
#include "os_threads_group.h"
#include "os_cmd_ui.h"

#include "index_layout.h"
#include "index_collate.h"
#include "index_block_if.h"

#ifdef	__cplusplus
extern "C" {
#endif

typedef enum tagCACHE_STATUS_E
{
    EMPTY = 0,  /* no data in cache */
    CLEAN,      /* data in buffer is the same to data on disk */
    DIRTY,      /* data in buffer read from disk is changed */
    FLUSH       /* the dirty data is flushed to disk */
} CACHE_STATUS_E;

#define INVALID_OBJID                ((uint64_t)(-1))
#define OBJID_IS_INVALID(id)          ((id) == INVALID_OBJID)


#define IBC_SET_DIRTY(ibc)  ((ibc)->state = DIRTY)
#define IBC_SET_CLEAN(ibc)  ((ibc)->state == CLEAN)
#define IBC_SET_EMPTY(ibc)  ((ibc)->state == EMPTY)
#define IBC_DIRTY(ibc)  ((ibc)->state == DIRTY)
#define IBC_CLEAN(ibc)  ((ibc)->state == CLEAN)
#define IBC_EMPTY(ibc)  ((ibc)->state == EMPTY)

#define INDEX_NAME_SIZE            256


#define INDEX_MAX_DEPTH           16

#define ATTR_INFO_DIRTY(obj_info)   IBC_DIRTY(&(obj_info)->root_ibc)
#define ATTR_INFO_SET_DIRTY(obj_info) IBC_SET_DIRTY(&(obj_info)->root_ibc)
#define ATTR_INFO_CLR_DIRTY(obj_info) IBC_SET_CLEAN(&(obj_info)->root_ibc)

#define OBJ_FLAG_DIRTY   0x01

#define INODE_SET_DIRTY(obj)      ((obj)->obj_state |= OBJ_FLAG_DIRTY)
#define INODE_CLR_DIRTY(obj)      ((obj)->obj_state &= ~OBJ_FLAG_DIRTY)
#define INODE_DIRTY(obj)         ((obj)->obj_state & OBJ_FLAG_DIRTY)

typedef struct _ifs_block_cache
{
	uint64_t vbn;
	uint32_t state;
	struct _INDEX_BLOCK *ib;
	avl_node_t entry;
} ifs_block_cache_t;

typedef struct _ifs_old_block
{
	uint64_t vbn;
	avl_node_t entry;
} ifs_old_block_t;

typedef struct _object_info
{
    struct _index_handle *index;       // index handle
    
    inode_record_t inode;                // inode
    inode_record_t old_inode;            // old inode
    
    uint32_t obj_state;                // state

    uint64_t objid;                    // object id
    uint64_t inode_no;                 // inode no, also the position stored on disk

    char obj_name[OBJ_NAME_MAX_SIZE];
    
    avl_node_t entry;                  // register in index handle

    ATTR_RECORD attr_record;           // attr record
    OS_RWLOCK attr_lock;               // lock  tree handle

    
    DLIST_HEAD_S obj_hnd_list;        // all object handle
    OS_RWLOCK    obj_hnd_lock;        // lock the obj_hnd_list operation

    ifs_block_cache_t root_ibc;
    avl_tree_t caches;            // record all new block data
    avl_tree_t old_blocks;        // record old block info
    OS_RWLOCK caches_lock;
    
    uint32_t obj_ref_cnt;
    
    OS_RWLOCK obj_lock;
} object_info_t;

typedef struct _object_handle
{
    struct _index_handle *index;       // index handle
    object_info_t *obj_info;

    // tree handle structure
    uint8_t max_depth;
    uint8_t depth;             // Number of the parent nodes
    ifs_block_cache_t *cache_stack[INDEX_MAX_DEPTH];
    uint64_t position_stack[INDEX_MAX_DEPTH];
    ifs_block_cache_t *cache;
    uint64_t position;
    INDEX_ENTRY *ie;        

    DLIST_ENTRY_S entry;
} object_handle_t;

#define MAX_BLK_NUM  10
#define MIN_BLK_NUM  3

typedef struct space_manager
{
    object_handle_t *space_obj;

    uint64_t first_free_block;
    uint64_t total_free_blocks;

    OS_RWLOCK lock;
} space_manager_t;

typedef struct space_base_manager
{
    space_manager_t sm;

    QUEUE_S *q;
    OS_RWLOCK lock;
} space_base_manager_t;

typedef struct _index_handle
{
    block_handle_t *hnd;               // The block file handle
    char name[INDEX_NAME_SIZE];        // index name

    object_handle_t *id_obj;
    //OBJECT_HANDLE *log_obj;
    //OBJECT_HANDLE *space_obj;
    
    space_manager_t sm;
    space_base_manager_t sbm;
    
    uint32_t index_ref_cnt;
    avl_tree_t obj_list;              // all opened object info

    avl_node_t entry;
    
    OS_RWLOCK index_lock;             // lock
} index_handle_t;

#define INDEX_UPDATE_INODE(obj) \
    index_update_block_pingpong(obj->index->hnd, &obj->inode.head, obj->inode_no)
    
#define INDEX_READ_INODE(index, obj, inode_no) \
    index_read_block_pingpong(index->hnd, &obj->inode.head, inode_no, INODE_MAGIC, INODE_SIZE);

#define INDEX_ALLOC_BLOCK(index, objid, vbn) index_alloc_space(index, objid, 1, vbn)
#define INDEX_FREE_BLOCK(index, objid, vbn)  index_free_space(index, objid, vbn, 1)

extern int32_t index_block_read(struct _object_handle *obj, uint64_t vbn);
extern int32_t index_alloc_cache_and_block(struct _object_info *obj_info, ifs_block_cache_t **cache);
extern int32_t index_release_all_dirty_blocks(struct _object_info *obj_info);
extern void index_release_all_old_blocks_mem(struct _object_info *obj_info);
extern void index_release_all_old_blocks(struct _object_info *obj_info);
extern int32_t index_record_old_block(struct _object_info *obj_info, uint64_t vbn);
extern int32_t index_release_all_caches(struct _object_info *obj_info);
extern int32_t index_flush_all_dirty_caches(struct _object_info * obj_info);

extern int32_t walk_all_opened_index(
    int32_t (*func)(void *, index_handle_t *), void *para);



#ifdef	__cplusplus
}
#endif

#include "index_tree.h"
#include "index_object.h"
#include "index_manager.h"
#include "index_space_manager.h"

#include "index_tools_if.h"

#endif


