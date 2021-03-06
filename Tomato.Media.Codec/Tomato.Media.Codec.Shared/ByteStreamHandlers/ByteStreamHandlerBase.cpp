﻿//
// Tomato Media Codec
// Media Foundation ByteStream Handler 基类
// 
// 作者：SunnyCase
// 创建时间：2015-03-17
#include "pch.h"
#include "ByteStreamHandlerBase.h"

using namespace NS_MEDIA;
using namespace NS_MEDIA_CODEC;
using namespace WRL;
using namespace concurrency;

ByteStreamHandlerBase::ByteStreamHandlerBase()
{
	
}

ByteStreamHandlerBase::~ByteStreamHandlerBase()
{

}

//-------------------------------------------------------------------
// IMediaExtension methods
//-------------------------------------------------------------------

#if (NTDDI_VERSION >= NTDDI_WIN8)
//-------------------------------------------------------------------
// SetProperties
// Sets the configuration of the media byte stream handler
//-------------------------------------------------------------------
IFACEMETHODIMP ByteStreamHandlerBase::SetProperties(
	ABI::Windows::Foundation::Collections::IPropertySet *pConfiguration)
{
	return S_OK;
}
#endif

//-------------------------------------------------------------------
// IMFByteStreamHandler methods
//-------------------------------------------------------------------

//-------------------------------------------------------------------
// BeginCreateObject
// 开始创建 MediaSource
//-------------------------------------------------------------------

HRESULT ByteStreamHandlerBase::BeginCreateObject(
	/* [in] */ IMFByteStream *pByteStream,
	/* [in] */ LPCWSTR pwszURL,
	/* [in] */ DWORD dwFlags,
	/* [in] */ IPropertyStore *pProps,
	/* [out] */ IUnknown **ppIUnknownCancelCookie,  // Can be nullptr
	/* [in] */ IMFAsyncCallback *pCallback,
	/* [in] */ IUnknown *punkState                  // Can be nullptr
	)
{
	if (pByteStream == nullptr || pCallback == nullptr)
		return E_POINTER;
	try
	{
		// 创建 MediaSource
		if (dwFlags & MF_RESOLUTION_MEDIASOURCE)
			OnCreateMediaSource(pByteStream, pCallback, punkState);
		//不支持其他用途
		else
			ThrowIfFailed(E_INVALIDARG);

		if (ppIUnknownCancelCookie)
		{
			*ppIUnknownCancelCookie = nullptr;
		}
	}
	CATCH_ALL();

	return S_OK;
}

//-------------------------------------------------------------------
// EndCreateObject
// Completes the BeginCreateObject operation.
//-------------------------------------------------------------------

HRESULT ByteStreamHandlerBase::EndCreateObject(
	/* [in] */ IMFAsyncResult *pResult,
	/* [out] */ MF_OBJECT_TYPE *pObjectType,
	/* [out] */ IUnknown **ppObject)
{
	if (pResult == nullptr || pObjectType == nullptr || ppObject == nullptr)
	{
		return E_POINTER;
	}

	HRESULT hr = S_OK;

	*pObjectType = MF_OBJECT_INVALID;
	*ppObject = nullptr;

	hr = pResult->GetStatus();

	if (SUCCEEDED(hr))
	{
		ComPtr<IUnknown> punkSource;
		hr = pResult->GetObject(&punkSource);
		if (SUCCEEDED(hr))
		{
			*pObjectType = MF_OBJECT_MEDIASOURCE;
			*ppObject = punkSource.Detach();
		}
	}

	return hr;
}


HRESULT ByteStreamHandlerBase::CancelObjectCreation(IUnknown *pIUnknownCancelCookie)
{
	return E_NOTIMPL;
}

HRESULT ByteStreamHandlerBase::GetMaxNumberOfBytesRequiredForResolution(QWORD* pqwBytes)
{
	return E_NOTIMPL;
}