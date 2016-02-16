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

            ��Ȩ����(C), 2011~2014, AXEN������
********************************************************************************
�� �� ��: OS_INDEX_TREE.C
��    ��: 1.00
��    ��: 2011��8��21��
��������: �����㷨���Գ���
�����б�: 
    1. ...: 
�޸���ʷ: 
    �汾��1.00  ����: ������ (zeng_hr@163.com)  ����: 2011��8��21��
--------------------------------------------------------------------------------
    1. ��ʼ�汾
*******************************************************************************/
#include "index_if.h"

extern int32_t index_verify_attr(void *tree);

#define TEST_KEY_LEN   8
#define TEST_VALUE_LEN 20

typedef struct tagKEY_ACTION_S
{
    uint8_t action;   /* ɾ���������� */
    char *key;     /* key */
} KEY_ACTION_S;

/* ��ϲ���ʱ�Ĳ������� */
const KEY_ACTION_S KEY_ACTION_LIST[]
= {
    {1, "0000000000009FFF"},
    {1, "0000000000001234"},
    {0, "0000000000009FFF"},
};

static int32_t cmd_insert_key(INDEX_TOOLS_PARA_S *para)
{
    int32_t ret = 0;
    INDEX_HANDLE *index = NULL;
    OBJECT_HANDLE *obj = NULL;
    ATTR_HANDLE *attr = NULL;
    uint64_t key = 0;
    uint8_t value[TEST_VALUE_LEN];

    ASSERT(NULL != para);

    if ((0 == strlen(para->index_name))
        || (0 == strlen(para->obj_name)))
    {
        para->net->print(para->net->net, "invalid index name(%s) or obj name(%s).\n",
            para->index_name, para->obj_name);
        return -1;
    }

    ret = index_open(para->index_name, para->start_lba, &index);
    if (ret < 0)
    {
        para->net->print(para->net->net, "Open index failed. index(%s) start_lba(%lld) ret(%d)\n",
            para->index_name, para->start_lba, ret);
        return ret;
    }

    ret = index_open_object(index->root_obj, para->obj_name, &obj);
    if (ret < 0)
    {
        para->net->print(para->net->net, "Create obj failed. obj(%s) ret(%d)\n",
            para->obj_name, ret);
        (void)index_close(index);
        return ret;
    }

    ret = index_open_xattr(obj, para->attr_name, &attr);
    if (ret < 0)
    {
        ret = index_create_xattr(obj, para->attr_name, ATTR_FLAG_TABLE, &attr);
        if (ret < 0)
        {
            para->net->print(para->net->net, "Create tree failed. attr(%s) ret(%d)\n",
                para->attr_name, ret);
            (void)index_close(index);
            return ret;
        }
    }

    memset(value, 0x88, sizeof(value));

    for (key = 0; key < para->keys_num; key++)
    {
        ret = index_insert_key(attr, &key, TEST_KEY_LEN,
            value, TEST_VALUE_LEN);
        if (0 > ret)
        {
            para->net->print(para->net->net, "Insert key failed. key(%lld) ret(%d)\n",
                key, ret);
            break;
        }

        ret = index_verify_attr(obj);
        if (0 > ret)
        {
            para->net->print(para->net->net, "Verify key failed. key(%lld) ret(%d)\n",
                key, ret);
            break;
        }
    }

    //(OS_VOID)index_close_attr(attr);
    //(OS_VOID)index_close_object(obj);
    (void)index_close(index);
    
    return 0;
}

static int32_t cmd_remove_key(INDEX_TOOLS_PARA_S *para)
{
    int32_t ret = 0;
    INDEX_HANDLE *index = NULL;
    OBJECT_HANDLE *obj = NULL;
    ATTR_HANDLE *attr = NULL;
    uint64_t key = 0;

    ASSERT(NULL != para);

    if ((0 == strlen(para->index_name))
        || (0 == strlen(para->obj_name)))
    {
        para->net->print(para->net->net, "invalid index name(%s) or obj name(%s).\n",
            para->index_name, para->obj_name);
        return -1;
    }

    ret = index_open(para->index_name, para->start_lba, &index);
    if (ret < 0)
    {
        para->net->print(para->net->net, "Open index failed. index(%s) start_lba(%lld) ret(%d)\n",
            para->index_name, para->start_lba, ret);
        return ret;
    }

    ret = index_open_object(index->root_obj, para->obj_name, &obj);
    if (ret < 0)
    {
        para->net->print(para->net->net, "Open tree failed. tree(%s) ret(%d)\n",
            para->obj_name, ret);
        (void)index_close(index);
        return ret;
    }

    ret = index_open_xattr(obj, para->attr_name, &attr);
    if (ret < 0)
    {
        ret = index_create_xattr(obj, para->attr_name, ATTR_FLAG_TABLE, &attr);
        if (ret < 0)
        {
            para->net->print(para->net->net, "Create tree failed. attr(%s) ret(%d)\n",
                para->attr_name, ret);
            (void)index_close_object(obj);
            (void)index_close(index);
            return ret;
        }
    }

    for (key = 0; key < para->keys_num; key++)
    {
        ret = index_remove_key(attr, &key, TEST_KEY_LEN);
        
        if (0 > ret)
        {
            para->net->print(para->net->net, "Remove key failed. key(%lld) ret(%d)\n",
                key, ret);
            break;
        }

        ret = index_verify_attr(obj);
        if (0 > ret)
        {
            para->net->print(para->net->net, "Verify key failed. key(%lld) ret(%d)\n",
                key, ret);
            break;
        }
    }

    (void)index_close_attr(attr);
    (void)index_close_object(obj);
    (void)index_close(index);
    
    return 0;
}


int do_insert_key_cmd(int argc, char *argv[], NET_PARA_S *net)
{
    INDEX_TOOLS_PARA_S *para = NULL;

    para = OS_MALLOC(sizeof(INDEX_TOOLS_PARA_S));
    if (NULL == para)
    {
        net->print(net->net, "Allocate memory failed. size(%d)\n",
            (uint32_t)sizeof(INDEX_TOOLS_PARA_S));
        return -1;
    }

    parse_all_para(argc, argv, para);
    para->net = net;
    cmd_insert_key(para);

    OS_FREE(para);

    return 0;
}

int do_remove_key_cmd(int argc, char *argv[], NET_PARA_S *net)
{
    INDEX_TOOLS_PARA_S *para = NULL;

    para = OS_MALLOC(sizeof(INDEX_TOOLS_PARA_S));
    if (NULL == para)
    {
        net->print(net->net, "Allocate memory failed. size(%d)\n",
            (uint32_t)sizeof(INDEX_TOOLS_PARA_S));
        return -1;
    }

    parse_all_para(argc, argv, para);
    para->net = net;
    cmd_remove_key(para);

    OS_FREE(para);

    return 0;
}

