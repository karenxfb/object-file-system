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
�� �� ��: OS_LINUX_USER.H
��    ��: 1.00
��    ��: 2011��9��29��
��������: 
�����б�: 
    1. ...: 
�޸���ʷ: 
    �汾��1.00  ����: ������ (zeng_hr@163.com)  ����: 2011��9��29��
--------------------------------------------------------------------------------
    1. ��ʼ�汾
*******************************************************************************/
#ifndef __OS_LINUX_USER_H__
#define __OS_LINUX_USER_H__

#define OS_MALLOC   malloc
#define OS_FREE     free
//#define OS_PRINT   (void)printf
#define OS_VSNPRINTF             vsnprintf
    
#define _CrtDumpMemoryLeaks()

#include <pthread.h>
#include <unistd.h>

// ANSI header file
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define OSStrToUll(pcBuf, end, base)   strtoull(pcBuf, end, base)
#define OS_SLEEP_SECOND(x)               sleep(x)
#define OS_SLEEP_MS(x)                  
#define OSThreadExit()                  pthread_exit(NULL)

typedef pthread_mutex_t             OS_MUTEX;
typedef pthread_rwlock_t            OS_RWLOCK;
typedef pthread_t                   OS_THREAD_ID;
typedef pthread_t                   OS_THREAD_T;

#define OS_GET_THREAD_ID()         pthread_self()

#define OS_MUTEX_INIT(v_pMutex)    pthread_mutex_init(v_pMutex, NULL)
#define OS_MUTEX_LOCK(v_pMutex)    pthread_mutex_lock(v_pMutex)
#define OS_MUTEX_UNLOCK(v_pMutex)  pthread_mutex_unlock(v_pMutex)
#define OS_MUTEX_DESTROY(v_pMutex) pthread_mutex_destroy(v_pMutex)

#define OS_RWLOCK_INIT(v_pMutex)      pthread_rwlock_init(v_pMutex, NULL)
#define OS_RWLOCK_RDLOCK(v_pMutex)    pthread_rwlock_rdlock(v_pMutex)
#define OS_RWLOCK_RDUNLOCK(v_pMutex)  pthread_rwlock_unlock(v_pMutex)
#define OS_RWLOCK_WRLOCK(v_pMutex)    pthread_rwlock_wrlock(v_pMutex)
#define OS_RWLOCK_WRUNLOCK(v_pMutex)  pthread_rwlock_unlock(v_pMutex)
#define OS_RWLOCK_DESTROY(v_pMutex)   pthread_rwlock_destroy(v_pMutex)

#define OS_SNPRINTF    (void)snprintf

#define ASSERT(x)
#define EXPORT_SYMBOL(x)
#define module_init(x)
#define module_exit(x)

#endif