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

            ��Ȩ����(C), 2012~2015, AXEN������
********************************************************************************
�� �� ��: INDEX_TREE.H
��    ��: 1.00
��    ��: 2012��6��13��
��������: 
�����б�: 
    1. ...: 
�޸���ʷ: 
    �汾��1.00  ����: ������ (zeng_hr@163.com)  ����: 2012��6��13��
--------------------------------------------------------------------------------
    1. ��ʼ�汾
*******************************************************************************/

#ifndef __INDEX_TREE_H__
#define __INDEX_TREE_H__


/* ����IndexWalk��v_flags����������ֵ */
#define INDEX_GET_FIRST      0x01 /* ��ȡ��һ��key�����Ϊ0�������ǻ�ȡ��һ�� */
#define INDEX_GET_LAST       0x02 /* ��ȡ���һ��key */
#define INDEX_GET_PREV       0x04 /* ��ȡǰһ��key */
#define INDEX_GET_CURRENT    0x08 /* ��ȡ��ǰkey (�����ڲ�ʹ��) */
#define INDEX_GET_LAST_ENTRY 0x10 /* ��ȡ���һ��entry */
#define INDEX_REMOVE_BLOCK   0x20 /* ɾ��key���ڵĿ� */
#define INDEX_ADD_BLOCK      0x40 /* ��key���ڵĿ�����Ϊ���ã��޸�ģʽʱʹ�� */
#define INDEX_WALK_MASK      0x1F


typedef int32_t (*DeleteFunc) (void *hnd,
    const void *key, uint16_t key_len,
    const void *c, uint16_t value_len);
typedef int32_t (*WalkAllCallBack) (void *obj, void *para);

extern int32_t index_search_key_nolock(ATTR_HANDLE * obj, const void * key,
    uint16_t key_len);
extern int32_t index_remove_key_nolock(ATTR_HANDLE * obj, const void * key,
    uint16_t key_len);
extern int32_t index_insert_key_nolock(ATTR_HANDLE * obj, const void * key,
    uint16_t key_len, const void * c, uint16_t value_len);

extern int32_t index_search_key(ATTR_HANDLE *obj, const void *key,
    uint16_t key_len);
extern int32_t index_remove_key(ATTR_HANDLE *obj, const void *key,
    uint16_t key_len);
extern int32_t index_insert_key(ATTR_HANDLE *obj, const void *key,
    uint16_t key_len, const void *c, uint16_t value_len);
extern int32_t index_update_value(ATTR_HANDLE * tree, const void * key,
    uint16_t key_len, const void * c, uint16_t value_len);

extern int32_t index_walk_all(ATTR_HANDLE *obj, bool_t v_bReverse,
    uint8_t flags, void *para, WalkAllCallBack v_pCallBack);

extern int32_t walk_tree(ATTR_HANDLE *obj, uint8_t flags);
extern int64_t index_get_total_key(ATTR_HANDLE *obj);
extern int64_t index_get_target_key(ATTR_HANDLE *obj, uint64_t target);

/* �������ڲ��ӿڣ��������ԣ���ָ���������ʹ�ã��̲߳���ȫ */
extern int32_t tree_remove_ie(ATTR_HANDLE *obj);

typedef struct tagWALK_ALL_TREES_PARA_S
{
    uint8_t flags;
    int32_t (*pCallBack)(void *obj, void *para);
} WALK_ALL_TREES_PARA_S;

extern int32_t index_walk_all_attrs(ATTR_HANDLE *dir_tree,
    WALK_ALL_TREES_PARA_S *para);


extern void init_ib(INDEX_BLOCK * v_pstIB, uint8_t v_ucNodeType,
    uint32_t alloc_size);

extern int32_t collate_key(uint16_t collate_rule, INDEX_ENTRY * v_pstIE,
    const void * key, uint16_t key_len);
extern int32_t search_key_internal(ATTR_HANDLE * obj, const void * key,
    uint16_t key_len);


#endif
