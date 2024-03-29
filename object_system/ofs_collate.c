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
File Name: INDEX_COLLATE.C
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
#include "os_adapter.h"
#include "ofs_globals.h"
#include "ofs_if.h"
    
#ifdef __KERNEL__

char os_to_upper(char c)
{
    return (((c <= 'z') && (c >= 'a')) ? (c - ('a' - 'A')) : c);
}

unicode_char_t os_to_wupper(unicode_char_t c)
{
    return (((c <= 'z') && (c >= 'a')) ? (c - ('a' - 'A')) : c);
}

#else

#include <ctype.h>
#include <wctype.h>

#define os_to_upper toupper
#define os_to_wupper towupper

#endif
	
/*
return value:
    <0: b1 < b2
    =0: b1 == b2
    >0: b1 > b2
*/
int32_t os_collate_binary(const uint8_t *b1, uint32_t b1_size, const uint8_t *b2, uint32_t b2_size)
{
    ASSERT(b1_size > 0);
    ASSERT(b2_size > 0);

    /* discard the beginning 0 */
	while ((0 == *b1) && (b1_size > 0))
	{
		b1++;
		b1_size--;
	}
	
    /* discard the beginning 0 */
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

/*
return value
    <0: s1 < s2
    =0: s1 == s2
    >0: s1 > s2
*/
int32_t os_collate_unicode_string(const unicode_char_t *s1, uint32_t s1_size, const unicode_char_t *s2, uint32_t s2_size)
{
	unicode_char_t c1 = 0;
    unicode_char_t c2 = 0;

    ASSERT(s1_size > 0);
    ASSERT(s2_size > 0);

	while ((0 != s1_size) && (0 != s2_size))
	{
		c1 = os_to_wupper(*s1);
		c2 = os_to_wupper(*s2);
        if (c1 > c2)
		{
			return 1;
		}
        
		if (c1 < c2)
		{
			return -1;
		}

		s1++;
		s2++;
        s1_size--;
        s2_size--;
	}

    if (s1_size > s2_size)
	{
		return 1;
	}
    
	if (s1_size < s2_size)
	{
		return -1;
	}

	return 0;
}

/*
    <0: s1 < s2
    =0: s1 == s2
    >0: s1 > s2
*/
int32_t os_collate_ansi_string(const char *s1, uint32_t s1_size, const char *s2, uint32_t s2_size)
{
	char c1 = 0;
    char c2 = 0;

    ASSERT(s1_size > 0);
    ASSERT(s2_size > 0);

	while ((0 != s1_size) && (0 != s2_size))
	{
		c1 = os_to_upper(*s1);
		c2 = os_to_upper(*s2);
        if (c1 > c2)
		{
			return 1;
		}
        
		if (c1 < c2)
		{
			return -1;
		}

		s1++;
		s2++;
        s1_size--;
        s2_size--;
	}

    if (s1_size > s2_size)
	{
		return 1;
	}
    
	if (s1_size < s2_size)
	{
		return -1;
	}

	return 0;
} 

// little endian u64
uint64_t os_bstr_to_u64(const uint8_t *b, uint32_t b_size)
{
    uint64_t u64 = 0;
    uint8_t *uc = (uint8_t *)&u64;

    ASSERT(0 != b_size);
    ASSERT(sizeof(uint64_t) >= b_size);

    while (b_size--)
    {
        *uc = *b;
        uc++;
        b++;
    }

    return u64;
}

// little endian u64
uint32_t os_u64_size(uint64_t u64)
{
    uint8_t *uc = (uint8_t *)&u64;
    uint32_t b_size = sizeof(uint64_t);
    uint32_t pos = sizeof(uint64_t) - 1;

    while (pos > 0)
    {
        if (uc[pos] != 0)
        {
            break;
        }
        
        b_size--;
        pos--;
    }

    if (b_size == 0)
    {
        b_size = 1;
    }

    return b_size;
}

// little endian u64
uint32_t os_u64_to_bstr(uint64_t u64, uint8_t *b)
{
    uint8_t *uc = (uint8_t *)&u64;
    uint32_t b_size;
    uint32_t pos = sizeof(uint64_t) - 1;

    b_size = os_u64_size(u64);

    pos = 0;
    while (pos < b_size)
    {
        *b = *uc;
        b++;
        uc++;
		pos++;
    }
    
    return b_size;
}

	
/*
return value:
    <0: b1 < b2
    =0: b1 == b2
    >0: b1 > b2
*/
int32_t os_collate_u64(const uint8_t *b1, uint32_t b1_size, const uint8_t *b2, uint32_t b2_size)
{
    uint64_t u64_1 = os_bstr_to_u64(b1, b1_size);
    uint64_t u64_2 = os_bstr_to_u64(b2, b2_size);
	
    if (u64_1 > u64_2)
	{
		return 1;
	}

	if (u64_1 < u64_2)
	{
		return -1;
	}

	return 0;
}

uint32_t os_extent_to_extent_pair(const index_extent_t *ext, uint8_t *ext_pair)
{
    uint8_t addr_size;

    if (ext->len == 0) // invalid extent
    {
        return 0;
    }
    
    addr_size = os_u64_to_bstr(ext->addr, ext_pair + EXT_PAIR_HEADER_SIZE);
    ext_pair[0] = addr_size;
    
    return (EXT_PAIR_HEADER_SIZE + addr_size + os_u64_to_bstr(ext->len, ext_pair + EXT_PAIR_HEADER_SIZE + addr_size));
}

uint32_t os_extent_to_extent_pair2(uint64_t addr, uint64_t len, uint8_t *ext_pair)
{
    uint8_t addr_size;

    if (len == 0) // invalid extent
    {
        return 0;
    }
    
    addr_size = os_u64_to_bstr(addr, ext_pair + EXT_PAIR_HEADER_SIZE);
    ext_pair[0] = addr_size;
    
    return (EXT_PAIR_HEADER_SIZE + addr_size + os_u64_to_bstr(len, ext_pair + EXT_PAIR_HEADER_SIZE + addr_size));
}

uint64_t os_extent_pair_to_extent(const uint8_t *ext_pair, uint32_t ext_pair_size, uint64_t *addr)
{
    uint8_t addr_size = *ext_pair;

    if (ext_pair_size <= addr_size + EXT_PAIR_HEADER_SIZE) // invalid extent pair
    {
        return 0;
    }

    *addr = os_bstr_to_u64(ext_pair + EXT_PAIR_HEADER_SIZE, addr_size);
    return os_bstr_to_u64(ext_pair + EXT_PAIR_HEADER_SIZE + addr_size, ext_pair_size - addr_size - EXT_PAIR_HEADER_SIZE);
}

/*
return value:
    <0: k1 < k2
    =0: overlap
    >0: k1 > k2
*/
int32_t os_collate_extent(const uint8_t *k1, uint32_t k1_size, const uint8_t *v1, uint32_t v1_size,
    const uint8_t *k2, uint32_t k2_size, const uint8_t *v2, uint32_t v2_size)
{
    uint64_t addr1, addr2;
    uint64_t len1, len2;

    if ((v2_size == 0) || (v1_size == 0))
    {
        return os_collate_u64(k1, k1_size, k2, k2_size);
    }
    
    addr1 = os_bstr_to_u64(k1, k1_size);
    addr2 = os_bstr_to_u64(k2, k2_size);
    len1 = os_bstr_to_u64(v1, v1_size);
    len2 = os_bstr_to_u64(v2, v2_size);
	
    if (addr1 >= (addr2 + len2))
	{
		return 1;
	}

	if ((addr1 + len1) <= addr2)
	{
		return -1;
	}

	return 0; // overlap
}


// collate key
int32_t collate_key(uint16_t cr, index_entry_t *ie,
    const void *key, uint16_t key_len, const void *value, uint16_t value_len)
{
    ASSERT(cr < CR_BUTT);
    ASSERT(ie != NULL);
    ASSERT(key != NULL);
    ASSERT(key_len != 0);
    
    switch (cr)
    {
        case CR_BINARY:
            return os_collate_binary((uint8_t *)GET_IE_KEY(ie), ie->key_len,
                (uint8_t *)key, key_len);

        case CR_ANSI_STRING:
            return os_collate_ansi_string((char *)GET_IE_KEY(ie),
                ie->key_len, (char *)key, key_len);

        case CR_UNICODE_STRING:
            return os_collate_unicode_string((unicode_char_t *)GET_IE_KEY(ie),
                ie->key_len, (unicode_char_t *)key, key_len);

        case CR_U64:
            return os_collate_u64((uint8_t *)GET_IE_KEY(ie), ie->key_len,
                (uint8_t *)key, key_len);

        case CR_EXTENT:
            return os_collate_extent((uint8_t *)GET_IE_KEY(ie), ie->key_len, (uint8_t *)GET_IE_VALUE(ie), ie->value_len,
                (uint8_t *)key, key_len, (uint8_t *)value, value_len);
            
        default:
            break;
    }

    return -INDEX_ERR_COLLATE;
}

