/******************************************************************************
 * Copyright (C) 2013      Advanced Digital Chips Inc. 
 *						http://www.adc.co.kr
 * Author : Software Team.
 ******************************************************************************/

#pragma once

#if	defined(__cplusplus)||defined(__GNUG__)// The GNU C++ compiler defines this. Testing it is equivalent to testing (__GNUC__ && __cplusplus). 
extern "C" {
#endif

#define MB_OK			0
#define MB_OKCANCEL		1
#define MB_YESNO		4
#define MB_YESNOCANCEL	3
    /*
    return value
    */
    enum
    {
        IDOK = 0	,
        IDCANCEL	,
        IDABORT		,
        IDRETRY		,
        IDIGNORE	,
        IDYES		,
        IDNO		,
        IDCONTINUE	,
    };

    int  egl_show_messagebox( const char* text, int flags, BOOL ( *user_input_fuction )( EGL_MSG* pMsg ) );
#if	defined(__cplusplus)||defined(__GNUG__)// The GNU C++ compiler defines this. Testing it is equivalent to testing (__GNUC__ && __cplusplus). 
}
#endif
