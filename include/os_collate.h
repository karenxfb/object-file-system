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
�� �� ��: OS_COLLATE.H
��    ��: 1.00
��    ��: 2011��6��19��
��������: ���ֱȽϺ���
�����б�: 
    1. ...: 
�޸���ʷ: 
    �汾��1.00  ����: ������ (zeng_hr@163.com)  ����: 2011��6��19��
--------------------------------------------------------------------------------
    1. ��ʼ�汾
*******************************************************************************/
#ifndef __OS_COLLATE_H__
#define __OS_COLLATE_H__

#ifdef  __cplusplus
extern "C"
{
#endif

typedef uint16_t UNICODE_CHAR;

extern int32_t os_collate_binary(const uint8_t *b1, uint32_t v_sizeB1,
    const uint8_t *b2, uint32_t b2_size);
extern int32_t os_collate_unicode_string(const UNICODE_CHAR *str1, uint32_t str1_size,
    const UNICODE_CHAR *str2, uint32_t str2_size);
extern int32_t os_collate_ansi_string(const char *str1, uint32_t str1_size,
    const char *str2, uint32_t str2_size);

#ifdef  __cplusplus
}
#endif

#endif

