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
�� �� ��: OS_COLLATE.C
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
#include "os_types.h"
#include "os_collate.h"
    
#ifdef __KERNEL__

/*******************************************************************************
��������: os_to_upper
����˵��: ��ANSI�ַ�ת���ɴ�д��ANSI�ַ�
�������:
    c: ��ת����ANSI�ַ�
�������: ��
�� �� ֵ: ��д��ANSI�ַ�
˵    ��: ��
*******************************************************************************/
char os_to_upper(char c)
{
    return (((c <= 'z') && (c >= 'a')) ? (c - ('a' - 'A')) : c);
}

/*******************************************************************************
��������: os_to_wupper
����˵��: ��UNICODE�ַ�ת���ɴ�д��UNICODE�ַ�
�������:
    c: ��ת����UNICODE�ַ�
�������: ��
�� �� ֵ: ��д��UNICODE�ַ�
˵    ��: ��
*******************************************************************************/
UNICODE_CHAR os_to_wupper(UNICODE_CHAR c)
{
    return (((c <= 'z') && (c >= 'a')) ? (c - ('a' - 'A')) : c);
}

#else
#include <ctype.h>
#include <wctype.h>

#define os_to_upper toupper
#define os_to_wupper towupper

#endif
	
/*******************************************************************************
��������: OSCollateBinary
����˵��: �Ƚ϶������ִ�
�������:
    b1   : Ҫ�Ƚϵĵ�һ���������ִ�
    b1_size: ��һ���������ִ��ĳ���
    b2   : Ҫ�Ƚϵĵڶ����������ִ�
    b2_size: �ڶ����������ִ��ĳ���
�������: ��
�� �� ֵ:
    <0: b1 < b2
    =0: b1 == b2
    >0: b1 > b2
˵    ��: ��
*******************************************************************************/
int32_t os_collate_binary(const uint8_t *b1, uint32_t b1_size,
    const uint8_t *b2, uint32_t b2_size)
{
    /* ȥ����0��ͷ�Ķ������ַ� */
	while ((0 == *b1) && (b1_size > 0))
	{
		b1++;
		b1_size--;
	}
	
    /* ȥ����0��ͷ�Ķ������ַ� */
	while ((0 == *b2) && (b2_size > 0))
	{
		b2++;
		b2_size--;
	}
	
    if (b1_size > b2_size)
	{
		return 1;
	}

	if (b1_size < b2_size)
	{
		return -1;
	}
    
    /* �Ƚ��ִ�����ʱv_uiSizeB1 == v_uiSizeB2 */
	while (0 != b1_size)
	{
		if (*b1 > *b2)
		{
			return 1;
		}

        if (*b1 < *b2)
		{
			return -1;
		}
        
		b1++;
		b2++;
        b1_size--;
	}

	return 0;
}

/*******************************************************************************
��������: OSCollateUnicodeString
����˵��: �Ƚ�unicode�ִ�
�������:
    str1   : Ҫ�Ƚϵĵ�һ��unicode�ִ�
    str1_size: ��һ��unicode�ִ��ĳ���
    str2   : Ҫ�Ƚϵĵڶ���unicode�ִ�
    str2_size: �ڶ���unicode�ִ��ĳ���
�������: ��
�� �� ֵ:
    <0: str1 < str2
    =0: str1 == str2
    >0: str1 > str2
˵    ��: ��
*******************************************************************************/
int32_t os_collate_unicode_string(const UNICODE_CHAR *str1, uint32_t str1_size,
	const UNICODE_CHAR *str2, uint32_t str2_size)
{
	UNICODE_CHAR c1 = 0;
    UNICODE_CHAR c2 = 0;

    /* �Ƚ��ִ� */
	while ((0 != str1_size) && (0 != str2_size))
	{
		c1 = os_to_wupper(*str1);
		c2 = os_to_wupper(*str2);
        if (c1 > c2)
		{
			return 1;
		}
        
		if (c1 < c2)
		{
			return -1;
		}

		str1++;
		str2++;
        str1_size--;
        str2_size--;
	}

    if (str1_size > str2_size)
	{
		return 1;
	}
    
	if (str1_size < str2_size)
	{
		return -1;
	}

	return 0;
}

/*******************************************************************************
��������: OSCollateAnsiString
����˵��: �Ƚ�ansi�ִ�
�������:
    str1   : Ҫ�Ƚϵĵ�һ��ansi�ִ�
    str1_size: ��һ��ansi�ִ��ĳ���
    str2   : Ҫ�Ƚϵĵڶ���ansi�ִ�
    str2_size: �ڶ���ansi�ִ��ĳ���
�������: ��
�� �� ֵ:
    <0: str1 < str2
    =0: str1 == str2
    >0: str1 > str2
˵    ��: ��
*******************************************************************************/
int32_t os_collate_ansi_string(const char *str1, uint32_t str1_size,
	const char *str2, uint32_t str2_size)
{
	char c1 = 0;
    char c2 = 0;

    /* �Ƚ��ִ� */
	while ((0 != str1_size) && (0 != str2_size))
	{
		c1 = os_to_upper(*str1);
		c2 = os_to_upper(*str2);
        if (c1 > c2)
		{
			return 1;
		}
        
		if (c1 < c2)
		{
			return -1;
		}

		str1++;
		str2++;
        str1_size--;
        str2_size--;
	}

    if (str1_size > str2_size)
	{
		return 1;
	}
    
	if (str1_size < str2_size)
	{
		return -1;
	}

	return 0;
} 

