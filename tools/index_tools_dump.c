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
File Name: INDEX_TOOLS_DUMP.C
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
#include "index_if.h"

typedef struct dump_para
{
    uint8_t depth; 
    uint64_t vbn;  
    uint32_t no;  
    net_para_t *net;
} dump_para_t;

static int32_t dump_callback(object_handle_t *tree, dump_para_t *para)
{
    uint32_t i = 0;
    uint8_t *uc = NULL;

    ASSERT(NULL != tree);
    ASSERT(NULL != para);

    if ((para->depth != tree->depth)
        || (para->vbn != tree->cache->vbn))
    {
        para->depth = tree->depth;
        para->vbn = tree->cache->vbn;
        OS_PRINT(para->net, "-------------iDepth: %d, llVbn: %lld-------------\n",
            tree->depth, tree->cache->vbn);
    }

    OS_PRINT(para->net, "%-7d(%d, %d) ", ++para->no,
        tree->ie->prev_len, tree->ie->len);

    uc = GET_IE_KEY(tree->ie);
    switch (tree->obj_info->attr_record.flags & CR_MASK)
    {
        case CR_ANSI_STRING:
            for (i = 0; i < tree->ie->key_len; i++)
            {
                OS_PRINT(para->net, "%c", uc[i]);
            }
            break;
        case CR_U64:
        case CR_EXTENT:
            OS_PRINT(para->net, "%8lld", os_bstr_to_u64(uc, tree->ie->key_len));
            break;
        default:
            for (i = 0; i < tree->ie->key_len; i++)
            {
                OS_PRINT(para->net, "%02X", uc[i]);
            }
            break;
    }
    

    OS_PRINT(para->net, "%s", " : ");

    uc = GET_IE_VALUE(tree->ie);
    switch ((tree->obj_info->attr_record.flags >> 4) & CR_MASK)
    {
        case CR_ANSI_STRING:
            for (i = 0; i < tree->ie->value_len; i++)
            {
                OS_PRINT(para->net, "%c", uc[i]);
            }
            break;
        case CR_U64:
        case CR_EXTENT:
            OS_PRINT(para->net, "%8lld", os_bstr_to_u64(uc, tree->ie->value_len));
            break;
        default:
            for (i = 0; i < tree->ie->value_len; i++)
            {
                OS_PRINT(para->net, "%02X", uc[i]);
            }
            break;
    }

    OS_PRINT(para->net, "%s", "\n");

    return 0;
}

static int32_t dump_key(object_handle_t *tree, const bool_t v_bReverse, net_para_t *net)
{
    int32_t ret = 0;
    dump_para_t para;

    ASSERT(NULL != tree);

	memset(&para, 0, sizeof(dump_para_t));
    
	OS_PRINT(net, "Start dump all keys. objid(%lld)\n", tree->obj_info->objid);

    para.net = net;
    ret = index_walk_all(tree, v_bReverse, 0, &para, (tree_walk_cb_t)dump_callback);
    if (ret < 0)
    {
        OS_PRINT(net, "Walk tree failed. objid(%lld) ret(%d)\n", tree->obj_info->objid, ret);
    }
    
	OS_PRINT(net, "Finished dump all keys. objid(%lld)\n", tree->obj_info->objid);
    
	return ret;
}

void dump_cmd(ifs_tools_para_t *para)
{
    index_handle_t *index = NULL;
    object_handle_t *obj = NULL;
    int32_t ret = 0;
    
    ASSERT (NULL != para);

    if (0 == strlen(para->index_name))
    {
        OS_PRINT(para->net, "invalid index name.\n");
        return;
    }
    
    ret = index_open(para->index_name, para->start_lba, &index);
    if (0 > ret)
    {
        OS_PRINT(para->net, "Open index failed. index_name(%s) start_lba(%lld) ret(%d)\n",
            para->index_name, para->start_lba, ret);
        return;
    }

    if (OBJID_IS_INVALID(para->objid)) // obj id not specified
    {
        para->objid = OBJID_OBJ_ID;
    }
    
    ret = index_open_object(index, para->objid, &obj);
    if (0 > ret)
    {
        OS_PRINT(para->net, "Open obj failed. index_name(%s) start_lba(%lld) objid(%lld) ret(%d)\n",
            para->index_name, para->start_lba, para->objid, ret);
        (void)index_close(index);
		return;
    }

    ret = dump_key(obj, para->flags & TOOLS_FLAGS_REVERSE, para->net);

    (void)index_close_object(obj);
	(void)index_close(index);
	
	return;
}


int do_dump_cmd(int argc, char *argv[], net_para_t *net)
{
    ifs_tools_para_t *para = NULL;

    para = OS_MALLOC(sizeof(ifs_tools_para_t));
    if (NULL == para)
    {
        OS_PRINT(net, "Allocate memory failed. size(%d)\n",
            (uint32_t)sizeof(ifs_tools_para_t));
        return -1;
    }

    parse_all_para(argc, argv, para);
    para->net = net;
    dump_cmd(para);

    OS_FREE(para);
    
    return 0;
}


