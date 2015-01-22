/******************************************************************************
 * Copyright (C) 2013      Advanced Digital Chips Inc. 
 *						http://www.adc.co.kr
 * Author : Software Team.
 ******************************************************************************/

#pragma once
#if	defined(__cplusplus)||defined(__GNUG__)// The GNU C++ compiler defines this. Testing it is equivalent to testing (__GNUC__ && __cplusplus). 
extern "C" {
#endif
    typedef enum
    {
        LIST_CHANGED = 0,
        LIST_MAX,
    }
    LIST_EVENT;

    typedef struct _tagListItem
    {
        BOOL selected;
        EGL_HANDLE label;
        void *itemdata;
        struct _tagListItem *prev;
        struct _tagListItem *next;
    } ListItem;


    typedef struct _tag_list_data
    {
        EGL_HANDLE scrollbar;
        int m_scrollbar_width;
        ListItem* listhead;
        ListItem* listtail;
        int m_listcount;
        int m_cursel;			// single mode
        int m_selcnt;			// multiple mode
        BOOL m_bMultiple;
        int m_x;
        int m_y;
        int m_w;
        int m_h;
        int m_item_x;
        int m_next_item_y;
        int m_item_w;
        int m_item_h;
        EGL_COLOR m_bgcolor;
        EGL_COLOR m_textcolor;
        EGL_COLOR m_seltextcolor;
        EGL_COLOR m_selbgcolor;
        BOOL m_selfdraw;
        int m_viewStartIndex;
    } LIST_DATA;

    EGL_HANDLE egl_create_listbox( int x, int y, int w, int h, BOOL bMultiple );
    BOOL egl_listbox_callback( EGL_HANDLE hObj, EVENT_CALLBACK cb );
    void egl_listbox_additem( EGL_HANDLE hObj, const char* text );
    void egl_listbox_delitem( EGL_HANDLE hObj );
    BOOL egl_listbox_delitem_text( EGL_HANDLE hObj, const char* text );
    void egl_listbox_alldelitem( EGL_HANDLE hObj );
    const char ** egl_listbox_get_all_itemlist( EGL_HANDLE hObj, int *itemcnt );
    const char* egl_listbox_get_sel_item( EGL_HANDLE hObj, int *index );
    const char ** egl_listbox_get_multiple_sel_itemlist( EGL_HANDLE hObj, int *selcnt );
    void egl_listbox_set_textalign( EGL_HANDLE hObj, int align );
    void egl_listbox_set_bgcolor( EGL_HANDLE hObj, unsigned char r, unsigned char g, unsigned char b );
    void egl_listbox_set_textcolor( EGL_HANDLE hObj, unsigned char r, unsigned char g, unsigned char b );
    void egl_listbox_set_selbgcolor( EGL_HANDLE hObj, unsigned char r, unsigned char g, unsigned char b );
    void egl_listbox_set_seltextcolor( EGL_HANDLE hObj, unsigned char r, unsigned char g, unsigned char b );
    void egl_listbox_set_scrollwidth( EGL_HANDLE hObj, int width );


#pragma once
#if	defined(__cplusplus)||defined(__GNUG__)// The GNU C++ compiler defines this. Testing it is equivalent to testing (__GNUC__ && __cplusplus). 
}
#endif
