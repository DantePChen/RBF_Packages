//-
// ==========================================================================
// Copyright 1995,2006,2008 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk
// license agreement provided at the time of installation or download,
// or which otherwise accompanies this software in either electronic
// or hard copy form.
// ==========================================================================
//+

// MELCommand.cpp : Implementation of CMELCommand

#include "stdafx.h"
#include "MELCommand.h"
#include "Common.h"

/*
 * Sends a command to the Maya command port. Connects to the command port
 * if not already connected. The result returned by Maya is stored as a
 * string in the m_strReply member variable, to be potentially processed
 * later by the get_Result() member function.
 *
 * Parameters
 *   Command [in] Null-terminated string with the command.
 *
 * Return Value
 *   Returns S_OK if successful, or an error value otherwise.
 *
 * Remarks
 *   The return value from this method only indicates whether the command
 *   was successfully sent to the Maya command port. It does not indicate
 *   if the command was successfully executed by Maya.
 */
STDMETHODIMP CMELCommand::Execute(BSTR Command)
{
	m_strResult.Empty();

	HRESULT hr = Connect();
	if (FAILED(hr))
		return hr;

	// convert BSTR to ANSI
	CAtlString strCommand(Command);
	CW2A pszCommand(strCommand);

	// send command to Maya command port
	int err = send(m_socket, pszCommand, (int)strlen(pszCommand), 0);
	if (err == SOCKET_ERROR)
	{
		Disconnect();
		return AtlReportError(GetObjectCLSID(),
			L"Unable to send command to command port",
			IID_IMELCommand);
	}

	// wait for reply, store result as raw string
	char* recvBuf = new char[4096+1];
	int numBytes = recv(m_socket, recvBuf, 4096, 0);
	if (numBytes == SOCKET_ERROR)
	{
		delete[] recvBuf;
		Disconnect();
		return AtlReportError(GetObjectCLSID(),
			L"Unable to get reply from command port",
			IID_IMELCommand);
	}
	recvBuf[numBytes] = '\0'; // practising safe string usage
	CA2W pszRecv(recvBuf); // convert ANSI to Unicode
	m_strResult = pszRecv;
	delete[] recvBuf;
	m_strResult.TrimLeft(L" \r\n");  // trim junk from beginning
	m_strResult.TrimRight(L" \r\n"); // ...and end

	return S_OK;
}

/*
 * The Result property specifies the result returned by Maya for the most
 * recent invocation of the Execute method. This property is read-only.
 *
 * Parameters
 *   pVal [out] Returns the Result property as a reference to a VARIANT.
 *
 * Remarks
 *   The Result property can be a LONG (VT_I4), DOUBLE (VT_R8),
 *   BSTR (VT_BSTR), or an array (VT_ARRAY) of those types.
 */
STDMETHODIMP CMELCommand::get_Result(VARIANT* pVal)
{
	return MakeVariant(m_strResult).Detach(pVal);
}


/*
 * The PortName property specifies the name of the Maya command port.
 *
 * Parameters
 *   pVal   [out] Returns the PortName property as a reference to a BSTR.
 *   newVal [in]  Sets the value of the PortName property to the value of the BSTR.
 *
 * Remarks
 *   Changing the PortName property while a connection is established will
 *   close that connection.
 *   The default/initial value of the PortName property is "mayaCommand".
 */
STDMETHODIMP CMELCommand::get_PortName(BSTR* pVal)
{
	*pVal = m_strPortName.AllocSysString();
	return S_OK;
}
STDMETHODIMP CMELCommand::put_PortName(BSTR newVal)
{
	CAtlString strPortName(newVal);
	if (m_strPortName != strPortName)
	{
		Disconnect();
		m_strPortName = strPortName;
	}
	return S_OK;
}

/*
 * Connects to the Maya command port, if not already connected.
 *
 * Return Value
 *   Returns S_OK if successful or already connected,
 *   or an error value otherwise.
 */
STDMETHODIMP CMELCommand::Connect(void)
{
	if (m_socket != INVALID_SOCKET)
		// already connected!
		return S_OK;

	// initialize Windows Sockets 2
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 0), &wsaData) == 0)
	{
		CW2A pszPortName(m_strPortName);
		m_socket = ConnectToMayaCommandPortByName(pszPortName);
		if (m_socket != INVALID_SOCKET)
			return S_OK;
	}
	WSACleanup();

	CAtlString strError = L"Unable to connect to command port: ";
	strError += m_strPortName;
	return AtlReportError(GetObjectCLSID(), strError, IID_IMELCommand);
}

/*
 * Closes the connection to the Maya command port;
 * does nothing if not connected.
 *
 * Return Value
 *   Returns S_OK.
 */
STDMETHODIMP CMELCommand::Disconnect(void)
{
	if (m_socket != INVALID_SOCKET)
	{
		DisconnectFromMayaCommandPort(m_socket);
		m_socket = INVALID_SOCKET;
		WSACleanup();
	}
	return S_OK;
}

/*
 * The Connected property specifies whether there currently is a connection
 * established with the Maya command port. This property is read-only.
 *
 * Parameters
 *   pVal [out] Returns the whether a connection currently exists.
 */
STDMETHODIMP CMELCommand::get_Connected(VARIANT_BOOL* pVal)
{
	*pVal = (m_socket == INVALID_SOCKET) ? FALSE : TRUE;
	return S_OK;
}

/*
 * Converts the specified (reply) string into a Variant value.
 * If the string contains tab characters ('\t), the string is
 * considered to contain an array of values.
 */
CComVariant CMELCommand::MakeVariant(const CAtlString& strText)
{
	int tabCount = 0;
	int tabPos = 0;
	while (tabPos < strText.GetLength()) {
		tabPos = strText.Find(L'\t', tabPos);
		if (tabPos < 0)
			break;
		++tabCount;
		++tabPos;
	}
	if (!tabCount) {
		return MakeVariantNotArray(strText);
	}
	CComSafeArray<VARIANT> safeArray;
	safeArray.Create(tabCount + 1);
	int curPos = 0;
	int index = 0;
	tabPos = strText.Find(L'\t');
	for (;;) {
		CAtlString strElement;
		if (tabPos > curPos) {
			strElement = strText.Mid(curPos, tabPos - curPos).Trim();
		}
		safeArray[index++] = MakeVariantNotArray(strElement);
		curPos = tabPos + 1;
		if (curPos == strText.GetLength()) {
			safeArray[index] = CComVariant(L"");
			break;
		}
		tabPos = strText.Find(L'\t', curPos);
		if (tabPos < 0) {
			safeArray[index] = MakeVariant(strText.Mid(curPos).Trim());
			break;
		}
	}
	return CComVariant(safeArray);
}

/*
 * Converts the specified (sub-)string into a Variant value.
 */
CComVariant CMELCommand::MakeVariantNotArray(const CAtlString& strText)
{
	CComVariant variant(strText);
	if (strText.SpanExcluding(L"0123456789.-+e").IsEmpty()) {
		if (strText.Find(L".") < 0) {
			variant.ChangeType(VT_I4);
		} else {
			variant.ChangeType(VT_R8);
		}
	}
	return variant;
}

