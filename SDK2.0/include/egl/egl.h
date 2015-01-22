
#pragma once
#if	defined(__cplusplus)||defined(__GNUG__)// The GNU C++ compiler defines this. Testing it is equivalent to testing (__GNUC__ && __cplusplus). 
extern "C" {
#endif
#include "egl_typedef.h"
#include "egl_base.h"
#include "font/egl_font.h"
#include "font/textout.h"
#include "egl_window.h"
#include "egl_theme.h"
#include "surface.h"
#include "egl_port.h"

#include "font/bitfont.h"
#include "font/bitmapfont.h"

#include "image/loadbmp.h"
#include "image/loadjpg.h"
#include "image/loadpng.h"
#include "image/loadtga.h"

#include "primitives/draw_arc.h"
#include "primitives/draw_rect.h"
#include "primitives/draw_line.h"
#include "primitives/draw_thickline.h"
#include "primitives/draw_poly.h"
#include "primitives/draw_roundrect.h"
#include "primitives/draw_legacy.h"

#include "object/egl_animation.h"
#include "object/egl_label.h"
#include "object/egl_button.h"
#include "object/egl_image_button.h"
#include "object/egl_circle_gauge.h"
#include "object/egl_bar_gauge.h"
#include "object/egl_custom_object.h"
#include "object/egl_messagebox.h"
#include "object/egl_progressbar.h"
#include "object/egl_scrollbar.h"
#include "object/egl_listbox.h"
#include "object/egl_slider.h"
#include "object/egl_checkbutton.h"
#include "object/egl_picture.h"
#include "object/egl_toggle_image.h"

#include <stdio.h>
#include <stdlib.h>


#if	defined(__cplusplus)||defined(__GNUG__)// The GNU C++ compiler defines this. Testing it is equivalent to testing (__GNUC__ && __cplusplus). 
}
#endif
