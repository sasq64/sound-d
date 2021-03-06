/*
 * mptUUID.cpp
 * -----------
 * Purpose: UUID utility functions.
 * Notes  : (currently none)
 * Authors: OpenMPT Devs
 * The OpenMPT source code is released under the BSD license. Read LICENSE for more details.
 */


#include "stdafx.h"
#include "mptUUID.h"

#if MPT_OS_WINDOWS

#if defined(MODPLUG_TRACKER) || !defined(NO_DMO)
#include <windows.h>
#include <objbase.h>
#endif // MODPLUG_TRACKER || !NO_DMO


OPENMPT_NAMESPACE_BEGIN


namespace Util
{


#if defined(MODPLUG_TRACKER) || !defined(NO_DMO)


std::wstring CLSIDToString(CLSID clsid)
//-------------------------------------
{
	std::wstring str;
	LPOLESTR tmp = nullptr;
	::StringFromCLSID(clsid, &tmp);
	if(tmp)
	{
		str = tmp;
		::CoTaskMemFree(tmp);
		tmp = nullptr;
	}
	return str;
}


CLSID StringToCLSID(const std::wstring &str)
//------------------------------------------
{
	CLSID clsid = CLSID();
	std::vector<OLECHAR> tmp(str.c_str(), str.c_str() + str.length() + 1);
	if(::CLSIDFromString(&tmp[0], &clsid) != S_OK)
	{
		return CLSID();
	}
	return clsid;
}


bool VerifyStringToCLSID(const std::wstring &str, CLSID &clsid)
//-------------------------------------------------------------
{
	std::vector<OLECHAR> tmp(str.c_str(), str.c_str() + str.length() + 1);
	return (::CLSIDFromString(&tmp[0], &clsid) == S_OK);
}


bool IsCLSID(const std::wstring &str)
//-----------------------------------
{
	CLSID clsid = CLSID();
	std::vector<OLECHAR> tmp(str.c_str(), str.c_str() + str.length() + 1);
	return (::CLSIDFromString(&tmp[0], &clsid) == S_OK);
}


std::wstring IIDToString(IID iid)
//-------------------------------
{
	std::wstring str;
	LPOLESTR tmp = nullptr;
	::StringFromIID(iid, &tmp);
	if(tmp)
	{
		str = tmp;
		::CoTaskMemFree(tmp);
		tmp = nullptr;
	}
	return str;
}


IID StringToIID(const std::wstring &str)
//--------------------------------------
{
	IID iid = IID();
	std::vector<OLECHAR> tmp(str.c_str(), str.c_str() + str.length() + 1);
	::IIDFromString(&tmp[0], &iid);
	return iid;
}


std::wstring GUIDToString(GUID guid)
//----------------------------------
{
	std::vector<OLECHAR> tmp(256);
	::StringFromGUID2(guid, &tmp[0], static_cast<int>(tmp.size()));
	return &tmp[0];
}


GUID StringToGUID(const std::wstring &str)
//----------------------------------------
{
	return StringToIID(str);
}


GUID CreateGUID()
//---------------
{
	GUID guid = GUID();
	if(::CoCreateGuid(&guid) != S_OK)
	{
		return GUID();
	}
	return guid;
}


#endif // MODPLUG_TRACKER || !NO_DMO


#if defined(MODPLUG_TRACKER) || !defined(NO_DMO) || defined(MPT_ENABLE_TEMPFILE)


UUID StringToUUID(const mpt::ustring &str)
//----------------------------------------
{
	UUID uuid = UUID();
	std::wstring wstr = mpt::ToWide(str);
	std::vector<wchar_t> tmp(wstr.c_str(), wstr.c_str() + wstr.length() + 1);
	if(::UuidFromStringW((RPC_WSTR)(&(tmp[0])), &uuid) != RPC_S_OK)
	{
		return UUID();
	}
	return uuid;
}


mpt::ustring UUIDToString(UUID uuid)
//----------------------------------
{
	std::wstring wstr;
	RPC_WSTR tmp = nullptr;
	if(::UuidToStringW(&uuid, &tmp) != RPC_S_OK)
	{
		return mpt::ustring();
	}
	wstr = (wchar_t*)tmp;
	::RpcStringFreeW(&tmp);
	return mpt::ToUnicode(wstr);
}


bool IsValid(UUID uuid)
//---------------------
{
	return false
		|| uuid.Data1 != 0
		|| uuid.Data2 != 0
		|| uuid.Data3 != 0
		|| uuid.Data4[0] != 0
		|| uuid.Data4[1] != 0
		|| uuid.Data4[2] != 0
		|| uuid.Data4[3] != 0
		|| uuid.Data4[4] != 0
		|| uuid.Data4[5] != 0
		|| uuid.Data4[6] != 0
		|| uuid.Data4[7] != 0
		;
}


UUID CreateUUID()
//---------------
{
	UUID uuid = UUID();
	RPC_STATUS status = ::UuidCreate(&uuid);
	if(status != RPC_S_OK && status != RPC_S_UUID_LOCAL_ONLY)
	{
		return UUID();
	}
	return uuid;
}


UUID CreateLocalUUID()
//--------------------
{
	#if _WIN32_WINNT >= 0x0501
		// Available since Win2000, but we check for WinXP in order to not use this
		// function in Win32old builds. It is not available on some non-fully
		// patched Win98SE installs in the wild.
		UUID uuid = UUID();
		RPC_STATUS status = ::UuidCreateSequential(&uuid);
		if(status != RPC_S_OK && status != RPC_S_UUID_LOCAL_ONLY)
		{
			return UUID();
		}
		return uuid;
	#else
		// Fallback to ::UuidCreate is safe as ::UuidCreateSequential is only a
		// tiny performance optimization.
		return CreateUUID();
	#endif
}


#endif // MODPLUG_TRACKER || !NO_DMO || MPT_ENABLE_TEMPFILE


} // namespace Util


OPENMPT_NAMESPACE_END


#else // !MPT_OS_WINDOWS


OPENMPT_NAMESPACE_BEGIN


MPT_MSVC_WORKAROUND_LNK4221(mptUUID)


OPENMPT_NAMESPACE_END


#endif // MPT_OS_WINDOWS
