/*
 * Copyright 2007 Jacek Caban for CodeWeavers
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

#define WIN32_NO_STATUS
#define _INC_WINDOWS

#include <stdarg.h>
#include <assert.h>

#define COBJMACROS

#include <windef.h>
#include <winbase.h>
//#include "winuser.h"
#include <ole2.h>

#include <wine/debug.h>

#include "mshtml_private.h"

WINE_DEFAULT_DEBUG_CHANNEL(mshtml);

struct HTMLOptionElement {
    HTMLElement element;

    IHTMLOptionElement IHTMLOptionElement_iface;

    nsIDOMHTMLOptionElement *nsoption;
};

static inline HTMLOptionElement *impl_from_IHTMLOptionElement(IHTMLOptionElement *iface)
{
    return CONTAINING_RECORD(iface, HTMLOptionElement, IHTMLOptionElement_iface);
}

static HRESULT WINAPI HTMLOptionElement_QueryInterface(IHTMLOptionElement *iface,
        REFIID riid, void **ppv)
{
    HTMLOptionElement *This = impl_from_IHTMLOptionElement(iface);

    return IHTMLDOMNode_QueryInterface(&This->element.node.IHTMLDOMNode_iface, riid, ppv);
}

static ULONG WINAPI HTMLOptionElement_AddRef(IHTMLOptionElement *iface)
{
    HTMLOptionElement *This = impl_from_IHTMLOptionElement(iface);

    return IHTMLDOMNode_AddRef(&This->element.node.IHTMLDOMNode_iface);
}

static ULONG WINAPI HTMLOptionElement_Release(IHTMLOptionElement *iface)
{
    HTMLOptionElement *This = impl_from_IHTMLOptionElement(iface);

    return IHTMLDOMNode_Release(&This->element.node.IHTMLDOMNode_iface);
}

static HRESULT WINAPI HTMLOptionElement_GetTypeInfoCount(IHTMLOptionElement *iface, UINT *pctinfo)
{
    HTMLOptionElement *This = impl_from_IHTMLOptionElement(iface);
    return IDispatchEx_GetTypeInfoCount(&This->element.node.dispex.IDispatchEx_iface, pctinfo);
}

static HRESULT WINAPI HTMLOptionElement_GetTypeInfo(IHTMLOptionElement *iface, UINT iTInfo,
                                              LCID lcid, ITypeInfo **ppTInfo)
{
    HTMLOptionElement *This = impl_from_IHTMLOptionElement(iface);
    return IDispatchEx_GetTypeInfo(&This->element.node.dispex.IDispatchEx_iface, iTInfo, lcid,
            ppTInfo);
}

static HRESULT WINAPI HTMLOptionElement_GetIDsOfNames(IHTMLOptionElement *iface, REFIID riid,
                                                LPOLESTR *rgszNames, UINT cNames,
                                                LCID lcid, DISPID *rgDispId)
{
    HTMLOptionElement *This = impl_from_IHTMLOptionElement(iface);
    return IDispatchEx_GetIDsOfNames(&This->element.node.dispex.IDispatchEx_iface, riid, rgszNames,
            cNames, lcid, rgDispId);
}

static HRESULT WINAPI HTMLOptionElement_Invoke(IHTMLOptionElement *iface, DISPID dispIdMember,
                            REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS *pDispParams,
                            VARIANT *pVarResult, EXCEPINFO *pExcepInfo, UINT *puArgErr)
{
    HTMLOptionElement *This = impl_from_IHTMLOptionElement(iface);
    return IDispatchEx_Invoke(&This->element.node.dispex.IDispatchEx_iface, dispIdMember, riid,
            lcid, wFlags, pDispParams, pVarResult, pExcepInfo, puArgErr);
}

static HRESULT WINAPI HTMLOptionElement_put_selected(IHTMLOptionElement *iface, VARIANT_BOOL v)
{
    HTMLOptionElement *This = impl_from_IHTMLOptionElement(iface);
    nsresult nsres;

    TRACE("(%p)->(%x)\n", This, v);

    nsres = nsIDOMHTMLOptionElement_SetSelected(This->nsoption, v != VARIANT_FALSE);
    if(NS_FAILED(nsres)) {
        ERR("SetSelected failed: %08x\n", nsres);
        return E_FAIL;
    }

    return S_OK;
}

static HRESULT WINAPI HTMLOptionElement_get_selected(IHTMLOptionElement *iface, VARIANT_BOOL *p)
{
    HTMLOptionElement *This = impl_from_IHTMLOptionElement(iface);
    cpp_bool selected;
    nsresult nsres;

    TRACE("(%p)->(%p)\n", This, p);

    nsres = nsIDOMHTMLOptionElement_GetSelected(This->nsoption, &selected);
    if(NS_FAILED(nsres)) {
        ERR("GetSelected failed: %08x\n", nsres);
        return E_FAIL;
    }

    *p = selected ? VARIANT_TRUE : VARIANT_FALSE;
    return S_OK;
}

static HRESULT WINAPI HTMLOptionElement_put_value(IHTMLOptionElement *iface, BSTR v)
{
    HTMLOptionElement *This = impl_from_IHTMLOptionElement(iface);
    nsAString value_str;
    nsresult nsres;

    TRACE("(%p)->(%s)\n", This, debugstr_w(v));

    nsAString_InitDepend(&value_str, v);
    nsres = nsIDOMHTMLOptionElement_SetValue(This->nsoption, &value_str);
    nsAString_Finish(&value_str);
    if(NS_FAILED(nsres))
        ERR("SetValue failed: %08x\n", nsres);

    return S_OK;
}

static HRESULT WINAPI HTMLOptionElement_get_value(IHTMLOptionElement *iface, BSTR *p)
{
    HTMLOptionElement *This = impl_from_IHTMLOptionElement(iface);
    nsAString value_str;
    nsresult nsres;

    TRACE("(%p)->(%p)\n", This, p);

    nsAString_Init(&value_str, NULL);
    nsres = nsIDOMHTMLOptionElement_GetValue(This->nsoption, &value_str);
    return return_nsstr(nsres, &value_str, p);
}

static HRESULT WINAPI HTMLOptionElement_put_defaultSelected(IHTMLOptionElement *iface, VARIANT_BOOL v)
{
    HTMLOptionElement *This = impl_from_IHTMLOptionElement(iface);
    FIXME("(%p)->(%x)\n", This, v);
    return E_NOTIMPL;
}

static HRESULT WINAPI HTMLOptionElement_get_defaultSelected(IHTMLOptionElement *iface, VARIANT_BOOL *p)
{
    HTMLOptionElement *This = impl_from_IHTMLOptionElement(iface);
    FIXME("(%p)->(%p)\n", This, p);
    return E_NOTIMPL;
}

static HRESULT WINAPI HTMLOptionElement_put_index(IHTMLOptionElement *iface, LONG v)
{
    HTMLOptionElement *This = impl_from_IHTMLOptionElement(iface);
    FIXME("(%p)->(%d)\n", This, v);
    return E_NOTIMPL;
}

static HRESULT WINAPI HTMLOptionElement_get_index(IHTMLOptionElement *iface, LONG *p)
{
    HTMLOptionElement *This = impl_from_IHTMLOptionElement(iface);
    FIXME("(%p)->(%p)\n", This, p);
    return E_NOTIMPL;
}

static HRESULT WINAPI HTMLOptionElement_put_text(IHTMLOptionElement *iface, BSTR v)
{
    HTMLOptionElement *This = impl_from_IHTMLOptionElement(iface);
    nsIDOMText *text_node;
    nsAString text_str;
    nsIDOMNode *tmp;
    nsresult nsres;

    TRACE("(%p)->(%s)\n", This, debugstr_w(v));

    if(!This->element.node.doc->nsdoc) {
        WARN("NULL nsdoc\n");
        return E_UNEXPECTED;
    }

    while(1) {
        nsIDOMNode *child;

        nsres = nsIDOMHTMLOptionElement_GetFirstChild(This->nsoption, &child);
        if(NS_FAILED(nsres) || !child)
            break;

        nsres = nsIDOMHTMLOptionElement_RemoveChild(This->nsoption, child, &tmp);
        nsIDOMNode_Release(child);
        if(NS_SUCCEEDED(nsres)) {
            nsIDOMNode_Release(tmp);
        }else {
            ERR("RemoveChild failed: %08x\n", nsres);
            break;
        }
    }

    nsAString_InitDepend(&text_str, v);
    nsres = nsIDOMHTMLDocument_CreateTextNode(This->element.node.doc->nsdoc, &text_str, &text_node);
    nsAString_Finish(&text_str);
    if(NS_FAILED(nsres)) {
        ERR("CreateTextNode failed: %08x\n", nsres);
        return E_FAIL;
    }

    nsres = nsIDOMHTMLOptionElement_AppendChild(This->nsoption, (nsIDOMNode*)text_node, &tmp);
    if(NS_SUCCEEDED(nsres))
        nsIDOMNode_Release(tmp);
    else
        ERR("AppendChild failed: %08x\n", nsres);

    return S_OK;
}

static HRESULT WINAPI HTMLOptionElement_get_text(IHTMLOptionElement *iface, BSTR *p)
{
    HTMLOptionElement *This = impl_from_IHTMLOptionElement(iface);
    nsAString text_str;
    nsresult nsres;

    TRACE("(%p)->(%p)\n", This, p);

    nsAString_Init(&text_str, NULL);
    nsres = nsIDOMHTMLOptionElement_GetText(This->nsoption, &text_str);
    return return_nsstr(nsres, &text_str, p);
}

static HRESULT WINAPI HTMLOptionElement_get_form(IHTMLOptionElement *iface, IHTMLFormElement **p)
{
    HTMLOptionElement *This = impl_from_IHTMLOptionElement(iface);
    FIXME("(%p)->(%p)\n", This, p);
    return E_NOTIMPL;
}

static const IHTMLOptionElementVtbl HTMLOptionElementVtbl = {
    HTMLOptionElement_QueryInterface,
    HTMLOptionElement_AddRef,
    HTMLOptionElement_Release,
    HTMLOptionElement_GetTypeInfoCount,
    HTMLOptionElement_GetTypeInfo,
    HTMLOptionElement_GetIDsOfNames,
    HTMLOptionElement_Invoke,
    HTMLOptionElement_put_selected,
    HTMLOptionElement_get_selected,
    HTMLOptionElement_put_value,
    HTMLOptionElement_get_value,
    HTMLOptionElement_put_defaultSelected,
    HTMLOptionElement_get_defaultSelected,
    HTMLOptionElement_put_index,
    HTMLOptionElement_get_index,
    HTMLOptionElement_put_text,
    HTMLOptionElement_get_text,
    HTMLOptionElement_get_form
};

static inline HTMLOptionElement *impl_from_HTMLDOMNode(HTMLDOMNode *iface)
{
    return CONTAINING_RECORD(iface, HTMLOptionElement, element.node);
}

static HRESULT HTMLOptionElement_QI(HTMLDOMNode *iface, REFIID riid, void **ppv)
{
    HTMLOptionElement *This = impl_from_HTMLDOMNode(iface);

    *ppv = NULL;

    if(IsEqualGUID(&IID_IUnknown, riid)) {
        TRACE("(%p)->(IID_IUnknown %p)\n", This, ppv);
        *ppv = &This->IHTMLOptionElement_iface;
    }else if(IsEqualGUID(&IID_IDispatch, riid)) {
        TRACE("(%p)->(IID_IDispatch %p)\n", This, ppv);
        *ppv = &This->IHTMLOptionElement_iface;
    }else if(IsEqualGUID(&IID_IHTMLOptionElement, riid)) {
        TRACE("(%p)->(IID_IHTMLOptionElement %p)\n", This, ppv);
        *ppv = &This->IHTMLOptionElement_iface;
    }

    if(*ppv) {
        IUnknown_AddRef((IUnknown*)*ppv);
        return S_OK;
    }

    return HTMLElement_QI(&This->element.node, riid, ppv);
}

static const NodeImplVtbl HTMLOptionElementImplVtbl = {
    HTMLOptionElement_QI,
    HTMLElement_destructor,
    HTMLElement_cpc,
    HTMLElement_clone,
    HTMLElement_handle_event,
    HTMLElement_get_attr_col
};

static const tid_t HTMLOptionElement_iface_tids[] = {
    HTMLELEMENT_TIDS,
    IHTMLOptionElement_tid,
    0
};
static dispex_static_data_t HTMLOptionElement_dispex = {
    NULL,
    DispHTMLOptionElement_tid,
    NULL,
    HTMLOptionElement_iface_tids
};

HRESULT HTMLOptionElement_Create(HTMLDocumentNode *doc, nsIDOMHTMLElement *nselem, HTMLElement **elem)
{
    HTMLOptionElement *ret;
    nsresult nsres;

    ret = heap_alloc_zero(sizeof(HTMLOptionElement));
    if(!ret)
        return E_OUTOFMEMORY;

    ret->IHTMLOptionElement_iface.lpVtbl = &HTMLOptionElementVtbl;
    ret->element.node.vtbl = &HTMLOptionElementImplVtbl;

    HTMLElement_Init(&ret->element, doc, nselem, &HTMLOptionElement_dispex);

    nsres = nsIDOMHTMLElement_QueryInterface(nselem, &IID_nsIDOMHTMLOptionElement, (void**)&ret->nsoption);

    /* Share nsoption reference with nsnode */
    assert(nsres == NS_OK && (nsIDOMNode*)ret->nsoption == ret->element.node.nsnode);
    nsIDOMNode_Release(ret->element.node.nsnode);

    *elem = &ret->element;
    return S_OK;
}

static inline HTMLOptionElementFactory *impl_from_IHTMLOptionElementFactory(IHTMLOptionElementFactory *iface)
{
    return CONTAINING_RECORD(iface, HTMLOptionElementFactory, IHTMLOptionElementFactory_iface);
}

static HRESULT WINAPI HTMLOptionElementFactory_QueryInterface(IHTMLOptionElementFactory *iface,
                                                              REFIID riid, void **ppv)
{
    HTMLOptionElementFactory *This = impl_from_IHTMLOptionElementFactory(iface);

    *ppv = NULL;

    if(IsEqualGUID(&IID_IUnknown, riid)) {
        TRACE("(%p)->(IID_IUnknown %p)\n", This, ppv);
        *ppv = &This->IHTMLOptionElementFactory_iface;
    }else if(IsEqualGUID(&IID_IDispatch, riid)) {
        TRACE("(%p)->(IID_IDispatch %p)\n", This, ppv);
        *ppv = &This->IHTMLOptionElementFactory_iface;
    }else if(IsEqualGUID(&IID_IHTMLOptionElementFactory, riid)) {
        TRACE("(%p)->(IID_IHTMLOptionElementFactory %p)\n", This, ppv);
        *ppv = &This->IHTMLOptionElementFactory_iface;
    }

    if(*ppv) {
        IUnknown_AddRef((IUnknown*)*ppv);
        return S_OK;
    }

    WARN("(%p)->(%s %p)\n", This, debugstr_guid(riid), ppv);
    return E_NOINTERFACE;
}

static ULONG WINAPI HTMLOptionElementFactory_AddRef(IHTMLOptionElementFactory *iface)
{
    HTMLOptionElementFactory *This = impl_from_IHTMLOptionElementFactory(iface);
    LONG ref = InterlockedIncrement(&This->ref);

    TRACE("(%p) ref=%d\n", This, ref);

    return ref;
}

static ULONG WINAPI HTMLOptionElementFactory_Release(IHTMLOptionElementFactory *iface)
{
    HTMLOptionElementFactory *This = impl_from_IHTMLOptionElementFactory(iface);
    LONG ref = InterlockedDecrement(&This->ref);

    TRACE("(%p) ref=%d\n", This, ref);

    if(!ref)
        heap_free(This);

    return ref;
}

static HRESULT WINAPI HTMLOptionElementFactory_GetTypeInfoCount(IHTMLOptionElementFactory *iface, UINT *pctinfo)
{
    HTMLOptionElementFactory *This = impl_from_IHTMLOptionElementFactory(iface);
    FIXME("(%p)->(%p)\n", This, pctinfo);
    return E_NOTIMPL;
}

static HRESULT WINAPI HTMLOptionElementFactory_GetTypeInfo(IHTMLOptionElementFactory *iface, UINT iTInfo,
                                              LCID lcid, ITypeInfo **ppTInfo)
{
    HTMLOptionElementFactory *This = impl_from_IHTMLOptionElementFactory(iface);
    FIXME("(%p)->(%u %u %p)\n", This, iTInfo, lcid, ppTInfo);
    return E_NOTIMPL;
}

static HRESULT WINAPI HTMLOptionElementFactory_GetIDsOfNames(IHTMLOptionElementFactory *iface, REFIID riid,
                                                LPOLESTR *rgszNames, UINT cNames,
                                                LCID lcid, DISPID *rgDispId)
{
    HTMLOptionElementFactory *This = impl_from_IHTMLOptionElementFactory(iface);
    FIXME("(%p)->(%s %p %u %u %p)\n", This, debugstr_guid(riid), rgszNames, cNames,
                                        lcid, rgDispId);
    return E_NOTIMPL;
}

static HRESULT WINAPI HTMLOptionElementFactory_Invoke(IHTMLOptionElementFactory *iface, DISPID dispIdMember,
                            REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS *pDispParams,
                            VARIANT *pVarResult, EXCEPINFO *pExcepInfo, UINT *puArgErr)
{
    HTMLOptionElementFactory *This = impl_from_IHTMLOptionElementFactory(iface);
    FIXME("(%p)->(%d %s %d %d %p %p %p %p)\n", This, dispIdMember, debugstr_guid(riid),
            lcid, wFlags, pDispParams, pVarResult, pExcepInfo, puArgErr);
    return E_NOTIMPL;
}

static HRESULT WINAPI HTMLOptionElementFactory_create(IHTMLOptionElementFactory *iface,
        VARIANT text, VARIANT value, VARIANT defaultselected, VARIANT selected,
        IHTMLOptionElement **optelem)
{
    HTMLOptionElementFactory *This = impl_from_IHTMLOptionElementFactory(iface);
    nsIDOMHTMLElement *nselem;
    HTMLDOMNode *node;
    HRESULT hres;

    static const PRUnichar optionW[] = {'O','P','T','I','O','N',0};

    TRACE("(%p)->(%s %s %s %s %p)\n", This, debugstr_variant(&text), debugstr_variant(&value),
          debugstr_variant(&defaultselected), debugstr_variant(&selected), optelem);

    if(!This->window || !This->window->doc) {
        WARN("NULL doc\n");
        return E_UNEXPECTED;
    }

    *optelem = NULL;

    hres = create_nselem(This->window->doc, optionW, &nselem);
    if(FAILED(hres))
        return hres;

    hres = get_node(This->window->doc, (nsIDOMNode*)nselem, TRUE, &node);
    nsIDOMHTMLElement_Release(nselem);
    if(FAILED(hres))
        return hres;

    hres = IHTMLDOMNode_QueryInterface(&node->IHTMLDOMNode_iface,
            &IID_IHTMLOptionElement, (void**)optelem);
    node_release(node);

    if(V_VT(&text) == VT_BSTR)
        IHTMLOptionElement_put_text(*optelem, V_BSTR(&text));
    else if(V_VT(&text) != VT_EMPTY)
        FIXME("Unsupported text %s\n", debugstr_variant(&text));

    if(V_VT(&value) == VT_BSTR)
        IHTMLOptionElement_put_value(*optelem, V_BSTR(&value));
    else if(V_VT(&value) != VT_EMPTY)
        FIXME("Unsupported value %s\n", debugstr_variant(&value));

    if(V_VT(&defaultselected) != VT_EMPTY)
        FIXME("Unsupported defaultselected %s\n", debugstr_variant(&defaultselected));
    if(V_VT(&selected) != VT_EMPTY)
        FIXME("Unsupported selected %s\n", debugstr_variant(&selected));

    return S_OK;
}

static const IHTMLOptionElementFactoryVtbl HTMLOptionElementFactoryVtbl = {
    HTMLOptionElementFactory_QueryInterface,
    HTMLOptionElementFactory_AddRef,
    HTMLOptionElementFactory_Release,
    HTMLOptionElementFactory_GetTypeInfoCount,
    HTMLOptionElementFactory_GetTypeInfo,
    HTMLOptionElementFactory_GetIDsOfNames,
    HTMLOptionElementFactory_Invoke,
    HTMLOptionElementFactory_create
};

HRESULT HTMLOptionElementFactory_Create(HTMLInnerWindow *window, HTMLOptionElementFactory **ret_ptr)
{
    HTMLOptionElementFactory *ret;

    ret = heap_alloc(sizeof(*ret));
    if(!ret)
        return E_OUTOFMEMORY;

    ret->IHTMLOptionElementFactory_iface.lpVtbl = &HTMLOptionElementFactoryVtbl;
    ret->ref = 1;
    ret->window = window;

    *ret_ptr = ret;
    return S_OK;
}
