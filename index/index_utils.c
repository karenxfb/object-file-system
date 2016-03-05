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
File Name: INDEX_UTILS.C
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
MODULE(PID_INDEX);
#include "os_log.h"

extern int32_t FixupTree(void *tree, void *para);

/*******************************************************************************
获取当前树总共有多少个key
*******************************************************************************/
int64_t index_get_total_key(ATTR_HANDLE *tree)
{
    int64_t cnt = 0;
    
    if (NULL == tree)
    {
        LOG_ERROR("Invalid parameter. tree(%p)\n", tree);
        return -INDEX_ERR_PARAMETER;
    }

    if (0 == walk_tree(tree, INDEX_GET_FIRST))
    {
	    do
	    {
	    	cnt++;
	    } while (0 == walk_tree(tree, 0));
    }
    
    return cnt;
}

/*******************************************************************************
获取指定序号的key
*******************************************************************************/
int64_t index_get_target_key(ATTR_HANDLE *tree, uint64_t target)
{
	int64_t cnt = 0;
	
    if (NULL == tree)
    {
        LOG_ERROR("Invalid parameter. tree(%p)\n", tree);
        return -INDEX_ERR_PARAMETER;
    }

    if (0 == walk_tree(tree, INDEX_GET_FIRST))
    {
	    do
	    {
	    	cnt++;
	    	if (0 == --target)
	    	{
	    		break;
	    	}
	    } while (0 == walk_tree(tree, 0));
    }
    
    return cnt;
}

/*******************************************************************************
函数名称: index_walk_all
功能说明: 遍历所有的entry，并执行指定的操作
输入参数:
    v_pTree    : 要操作的树
    v_uiFlags    : 遍历标记
    para    : 回调函数的输入参数
    v_pCallBack: 在每个entry时执行的回调函数
输出参数: 无
返 回 值:
    >=0: 成功
    < 0: 错误代码
说    明: 无
*******************************************************************************/
int32_t index_walk_all(ATTR_HANDLE *tree, bool_t v_bReverse, uint8_t flags,
    void *para, WalkAllCallBack v_pCallBack)
{
    int32_t ret = 0;
    uint8_t ucIfFlag = (B_FALSE == v_bReverse)
        ? INDEX_GET_FIRST : INDEX_GET_LAST;
    uint8_t ucWhileFlag = (B_FALSE == v_bReverse)
        ? 0 : INDEX_GET_PREV;

    if ((NULL == tree) || (NULL == v_pCallBack))
    {
        LOG_ERROR("Invalid parameter. tree(%p) v_pCallBack(%p)\n",
            tree, v_pCallBack);
        return -INDEX_ERR_PARAMETER;
    }

    ucIfFlag |= (flags & ~INDEX_WALK_MASK);
    ucWhileFlag |= (flags & ~INDEX_WALK_MASK);
    
    OS_RWLOCK_WRLOCK(&tree->attr_info->attr_lock);
    
    if (walk_tree(tree, ucIfFlag) == 0)
    {
	    do
	    {
            ret = v_pCallBack(tree, para);
            if (ret < 0)
            {
                LOG_ERROR("Call back failed. tree(%p) para(%p) ret(%d)\n",
                    tree, para, ret);
                OS_RWLOCK_WRUNLOCK(&tree->attr_info->attr_lock);
                return ret;
            }
	    } while (walk_tree(tree, ucWhileFlag) == 0);
    }
    
    OS_RWLOCK_WRUNLOCK(&tree->attr_info->attr_lock);
    
    return 0;
}

/*******************************************************************************
对指定的树进行操作
*******************************************************************************/
static int32_t tree_callback(void *tree, void *para)
{
#if 0
    int32_t ret = 0;
    char name[OBJ_NAME_SIZE];
    WALK_ALL_TREES_PARA_S *para = para;
    ATTR_HANDLE *tree = NULL;

    ASSERT(NULL != tree);
    ASSERT(NULL != para);

    memset(name, 0, sizeof(name));
    (void)IndexDumpKey(tree, name, (uint16_t)sizeof(name));
    
    ret = INDEX_OpenTreeNoLock(&tree, tree, name, 0);
    if (ret < 0)
    {
        LOG_ERROR("Open tree failed. tree(%s) ret(%d)\n", name, ret);
        return ret;
    }

    ret = para->pCallBack(tree, para);
    if (0 > ret)
    {
        LOG_ERROR("pCallBack return failed. name(%s) ret(%d)\n",
            tree->pstInode->name, ret);
    }
    else if (FixupTree == para->pCallBack)
    { /* 如果存储体有多份,那么需要写两次,保证每个存储体里面的数据都一样 */
        SET_TREE_DIRTY(tree);
        INDEX_CommitTreeTransNoLock(tree, COMMIT_FLAG_FORCE);
        SET_TREE_DIRTY(tree);
        INDEX_CommitTreeTransNoLock(tree, COMMIT_FLAG_FORCE);
    }

    (void)INDEX_CloseTreeNoLock(tree);

    return ret;
#endif
	return 0;
}

/*******************************************************************************
函数名称: IndexWalkAllTrees
功能说明: 遍历指定目录树和目录树下所有的子树，并执行指定的操作
输入参数:
    dir_tree: 目录树句柄
    para: 遍历参数
输出参数: 无
返 回 值:
    >=0: 成功
    < 0: 错误代码
说    明: 无
*******************************************************************************/
int32_t index_walk_all_attrs(ATTR_HANDLE *dir_tree,
    WALK_ALL_TREES_PARA_S *para)
{
#if 0
    int32_t ret = 0;
    
    /* 检查输入参数 */
    if ((NULL == dir_tree) || (NULL == para)
        || (NULL == para->pCallBack))
    {
        LOG_ERROR("Invalid parameter. tree(%p) para(%p)\n",
            dir_tree, para);
        return -INDEX_ERR_PARAMETER;
    }

    /* 对目录树进行操作 */
    ret = para->pCallBack(dir_tree, para);
    if (0 > ret)
    {
        LOG_ERROR("pCallBack return failed. name(%s) ret(%d)\n",
            dir_tree->pstInode->name, ret);
    }
    else if (FixupTree == para->pCallBack)
    { /* 如果存储体有多份,那么需要写两次,保证每个存储体里面的数据都一样 */
        SET_TREE_DIRTY((ATTR_HANDLE *)dir_tree);
        INDEX_CommitTreeTransNoLock(dir_tree, COMMIT_FLAG_FORCE);
        SET_TREE_DIRTY((ATTR_HANDLE *)dir_tree);
        INDEX_CommitTreeTransNoLock(dir_tree, COMMIT_FLAG_FORCE);
    }

    ret = index_walk_all(dir_tree, B_FALSE, para->flags, para, tree_callback);
    if (0 > ret)
    {
        LOG_ERROR("index_walk_all return failed. name(%s) ret(%d)\n",
            dir_tree->pstInode->name, ret);
        return ret;
    }
    
    return ret;
#endif
	return 0;
}

/*******************************************************************************
函数名称: IndexGetOpenedTreesNum
功能说明: 获取指定索引区中已经打开树的数目
输入参数:
    v_pIndex: 要操作的索引区
输出参数: 无
返 回 值:
    >=0: 树的数目
    < 0: 错误代码
说    明: 无
*******************************************************************************/
int32_t index_get_opened_attr_num(ATTR_HANDLE * tree)
{
#if 0
    int32_t ret = 0;
    
    /* 检查输入参数 */
    if (NULL == tree)
    {
        LOG_ERROR("Invalid parameter. tree(%p)\n", tree);
        return -INDEX_ERR_PARAMETER;
    }

    OS_RWLOCK_WRLOCK(&((ATTR_HANDLE *)tree)->index->rwlock);
    ret = dlist_count(&((ATTR_HANDLE *)tree)->index->stTreesList);
    OS_RWLOCK_WRUNLOCK(&((ATTR_HANDLE *)tree)->index->rwlock);

    return ret;
#endif
	return 0;
}


