/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/* ***** BEGIN LICENSE BLOCK *****
 * Version: NPL 1.1/GPL 2.0/LGPL 2.1
 *
 * The contents of this file are subject to the Netscape Public License
 * Version 1.1 (the "License"); you may not use this file except in
 * compliance with the License. You may obtain a copy of the License at
 * http://www.mozilla.org/NPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is mozilla.org code.
 *
 * The Initial Developer of the Original Code is 
 * Netscape Communications Corporation.
 * Portions created by the Initial Developer are Copyright (C) 1998
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 *
 * Alternatively, the contents of this file may be used under the terms of
 * either the GNU General Public License Version 2 or later (the "GPL"), or 
 * the GNU Lesser General Public License Version 2.1 or later (the "LGPL"),
 * in which case the provisions of the GPL or the LGPL are applicable instead
 * of those above. If you wish to allow use of your version of this file only
 * under the terms of either the GPL or the LGPL, and not to allow others to
 * use your version of this file under the terms of the NPL, indicate your
 * decision by deleting the provisions above and replace them with the notice
 * and other provisions required by the GPL or the LGPL. If you do not delete
 * the provisions above, a recipient may use your version of this file under
 * the terms of any one of the NPL, the GPL or the LGPL.
 *
 * ***** END LICENSE BLOCK ***** */

#include "nscore.h"

class nsILocalFile;

/**
 * Initialize the XPCOM glue by dynamically linking against the XPCOM
 * shared library indicated by xpcomFile.
 */

extern "C"
nsresult XPCOMGlueStartup(const char* xpcomFile);


/**
 * Finish the XPCOM glue after it is no longer needed.
 */

extern "C"
nsresult XPCOMGlueShutdown();


/**
 * Locate the path of a compatible GRE.
 * 
 * @return string buffer pointing to the GRE path (without a trailing
 *         directory separator). Callers do no need to free this buffer.
 */

extern "C"
char const * GRE_GetGREPath();


/**
 * Locate the path of a compatible GRE. This is returned as an
 * nsILocalFile instead of a char*.
 *
 * @param _retval   Ordinary XPCOM getter, returns an addrefed interface.
 */

extern "C"
nsresult GRE_GetGREDirectory(nsILocalFile* *_retval);


/**
 * Locate the path of the XPCOM binary of a compatible GRE.
 * The result of this function is normally passed directly to
 * XPCOMGlueStartup.
 *
 * @return string buffer pointing to the XPCOM DLL path. Callers do
 *         not need to free this buffer.
 */

extern "C"
char const * GRE_GetXPCOMPath();


/**
 * Embedding applications which don't need a custom
 * directoryserviceprovider may use GRE_Startup to start the XPCOM
 * glue and initialize the GRE in one step.
 */

extern "C"
nsresult GRE_Startup();


/**
 * Shut down XPCOM and the XPCOM glue in one step.
 */

extern "C"
nsresult GRE_Shutdown();
