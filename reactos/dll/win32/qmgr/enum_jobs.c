/*
 * Queue Manager (BITS) Job Enumerator
 *
 * Copyright 2007 Google (Roy Shea)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
 */

#include "qmgr.h"
#include <wine/debug.h>

WINE_DEFAULT_DEBUG_CHANNEL(qmgr);

typedef struct
{
    IEnumBackgroundCopyJobs IEnumBackgroundCopyJobs_iface;
    LONG ref;
    IBackgroundCopyJob **jobs;
    ULONG numJobs;
    ULONG indexJobs;
} EnumBackgroundCopyJobsImpl;

static inline EnumBackgroundCopyJobsImpl *impl_from_IEnumBackgroundCopyJobs(IEnumBackgroundCopyJobs *iface)
{
    return CONTAINING_RECORD(iface, EnumBackgroundCopyJobsImpl, IEnumBackgroundCopyJobs_iface);
}

static HRESULT WINAPI BITS_IEnumBackgroundCopyJobs_QueryInterface(IEnumBackgroundCopyJobs *iface,
        REFIID riid, void **ppv)
{
    TRACE("(%p,%s,%p)\n", iface, debugstr_guid(riid), ppv);

    if (IsEqualGUID(riid, &IID_IUnknown) || IsEqualGUID(riid, &IID_IEnumBackgroundCopyJobs))
    {
        *ppv = iface;
        IEnumBackgroundCopyJobs_AddRef(iface);
        return S_OK;
    }

    *ppv = NULL;
    return E_NOINTERFACE;
}

static ULONG WINAPI BITS_IEnumBackgroundCopyJobs_AddRef(IEnumBackgroundCopyJobs *iface)
{
    EnumBackgroundCopyJobsImpl *This = impl_from_IEnumBackgroundCopyJobs(iface);
    ULONG ref = InterlockedIncrement(&This->ref);

    TRACE("(%p) ref=%d\n", This, ref);

    return ref;
}

static ULONG WINAPI BITS_IEnumBackgroundCopyJobs_Release(IEnumBackgroundCopyJobs *iface)
{
    EnumBackgroundCopyJobsImpl *This = impl_from_IEnumBackgroundCopyJobs(iface);
    ULONG ref = InterlockedDecrement(&This->ref);
    ULONG i;

    TRACE("(%p) ref=%d\n", This, ref);

    if (ref == 0) {
        for(i = 0; i < This->numJobs; i++)
            IBackgroundCopyJob_Release(This->jobs[i]);
        HeapFree(GetProcessHeap(), 0, This->jobs);
        HeapFree(GetProcessHeap(), 0, This);
    }

    return ref;
}

static HRESULT WINAPI BITS_IEnumBackgroundCopyJobs_Next(IEnumBackgroundCopyJobs *iface, ULONG celt,
        IBackgroundCopyJob **rgelt, ULONG *pceltFetched)
{
    EnumBackgroundCopyJobsImpl *This = impl_from_IEnumBackgroundCopyJobs(iface);
    ULONG fetched;
    ULONG i;
    IBackgroundCopyJob *job;

    fetched = min(celt, This->numJobs - This->indexJobs);
    if (pceltFetched)
        *pceltFetched = fetched;
    else
    {
        /* We need to initialize this array if the caller doesn't request
           the length because length_is will default to celt.  */
        for (i = 0; i < celt; ++i)
            rgelt[i] = NULL;

        /* pceltFetched can only be NULL if celt is 1 */
        if (celt != 1)
            return E_INVALIDARG;
    }

    /* Fill in the array of objects */
    for (i = 0; i < fetched; ++i)
    {
        job = This->jobs[This->indexJobs++];
        IBackgroundCopyJob_AddRef(job);
        rgelt[i] = job;
    }

    return fetched == celt ? S_OK : S_FALSE;
}

static HRESULT WINAPI BITS_IEnumBackgroundCopyJobs_Skip(IEnumBackgroundCopyJobs *iface, ULONG celt)
{
    EnumBackgroundCopyJobsImpl *This = impl_from_IEnumBackgroundCopyJobs(iface);

    if (This->numJobs - This->indexJobs < celt)
    {
        This->indexJobs = This->numJobs;
        return S_FALSE;
    }

    This->indexJobs += celt;
    return S_OK;
}

static HRESULT WINAPI BITS_IEnumBackgroundCopyJobs_Reset(IEnumBackgroundCopyJobs *iface)
{
    EnumBackgroundCopyJobsImpl *This = impl_from_IEnumBackgroundCopyJobs(iface);
    This->indexJobs = 0;
    return S_OK;
}

static HRESULT WINAPI BITS_IEnumBackgroundCopyJobs_Clone(IEnumBackgroundCopyJobs *iface,
        IEnumBackgroundCopyJobs **ppenum)
{
    FIXME("Not implemented\n");
    return E_NOTIMPL;
}

static HRESULT WINAPI BITS_IEnumBackgroundCopyJobs_GetCount(IEnumBackgroundCopyJobs *iface,
    ULONG *puCount)
{
    EnumBackgroundCopyJobsImpl *This = impl_from_IEnumBackgroundCopyJobs(iface);
    *puCount = This->numJobs;
    return S_OK;
}

static const IEnumBackgroundCopyJobsVtbl BITS_IEnumBackgroundCopyJobs_Vtbl =
{
    BITS_IEnumBackgroundCopyJobs_QueryInterface,
    BITS_IEnumBackgroundCopyJobs_AddRef,
    BITS_IEnumBackgroundCopyJobs_Release,
    BITS_IEnumBackgroundCopyJobs_Next,
    BITS_IEnumBackgroundCopyJobs_Skip,
    BITS_IEnumBackgroundCopyJobs_Reset,
    BITS_IEnumBackgroundCopyJobs_Clone,
    BITS_IEnumBackgroundCopyJobs_GetCount
};

HRESULT enum_copy_job_create(BackgroundCopyManagerImpl *qmgr, IEnumBackgroundCopyJobs **enumjob)
{
    EnumBackgroundCopyJobsImpl *This;
    BackgroundCopyJobImpl *job;
    ULONG i;

    TRACE("%p, %p)\n", qmgr, enumjob);

    This = HeapAlloc(GetProcessHeap(), 0, sizeof *This);
    if (!This)
        return E_OUTOFMEMORY;
    This->IEnumBackgroundCopyJobs_iface.lpVtbl = &BITS_IEnumBackgroundCopyJobs_Vtbl;
    This->ref = 1;

    /* Create array of jobs */
    This->indexJobs = 0;

    EnterCriticalSection(&qmgr->cs);
    This->numJobs = list_count(&qmgr->jobs);

    if (0 < This->numJobs)
    {
        This->jobs = HeapAlloc(GetProcessHeap(), 0,
                               This->numJobs * sizeof *This->jobs);
        if (!This->jobs)
        {
            LeaveCriticalSection(&qmgr->cs);
            HeapFree(GetProcessHeap(), 0, This);
            return E_OUTOFMEMORY;
        }
    }
    else
        This->jobs = NULL;

    i = 0;
    LIST_FOR_EACH_ENTRY(job, &qmgr->jobs, BackgroundCopyJobImpl, entryFromQmgr)
    {
        IBackgroundCopyJob *job_iface = (IBackgroundCopyJob*)&job->IBackgroundCopyJob2_iface;
        IBackgroundCopyJob_AddRef(job_iface);
        This->jobs[i++] = job_iface;
    }
    LeaveCriticalSection(&qmgr->cs);

    *enumjob = &This->IEnumBackgroundCopyJobs_iface;
    return S_OK;
}
