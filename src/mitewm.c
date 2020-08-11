/*
GNU General Public License (version 3)

MiteWM
Copyright (C) 2020  Tada Perukii (Tada Teruki, 多田 瑛貴)

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <X11/Xlib.h>
#include <X11/cursorfont.h>
#include <X11/Xutil.h>
#include <cairo/cairo-xlib.h>
#include <stdlib.h>
#include <stdio.h>

// クライアントの構成ウインドウ識別番号
#define MTWM_CLIENT_WIDGETS    2  // 構成ウインドウの数
    #define MTWM_CLIENT_BOX    0  // Box
    #define MTWM_CLIENT_APP    1  // Application

#define MTWM_RESIZE_ANGLE_TOP    0
#define MTWM_RESIZE_ANGLE_BOTTOM 1
#define MTWM_RESIZE_ANGLE_START  2
#define MTWM_RESIZE_ANGLE_END    3
#define MTWM_EXIT_PRESSED        4

#define MTWM_MAX(A, B) ( (A) < (B) ? (B) : (A) )

// ディスプレイ
Display *mtwm_display;

// 根ウインドウ
Window   mtwm_root_window;

// ---------------------
// ===== configure =====
// ---------------------

// クライアントの枠線の幅。
const unsigned int mtwm_config_box_border            = 12;
// クライアントの枠線の幅のうち、影を描画しない外側の部分。
const unsigned int mtwm_config_empty_box_border      = 6;
// クライアントのTITLEBARの右端とAPPの右端のX座標の差 = EXITの幅
const unsigned int mtwm_config_titlebar_width_margin = 25;
// TITLEBARの高さ
const unsigned int mtwm_config_titlebar_height       = 25;
// 背景画像のファイル
const char * background_file = "/home/tada/Documents/ICONS/SagiBack.png";

// クライアントの影の粗さ。必ず0より大きい値に！
// 1.5~2.5 ぐらいがいい感じ。粗すぎると変な見た目になるし、逆に
const double mtwm_config_shadow_roughness            = 2.0;

const unsigned int mtwm_width_diff  = mtwm_config_box_border*2;
const unsigned int mtwm_height_diff = mtwm_config_titlebar_height + mtwm_config_box_border*2;

#include "mtwm_test.c"
#include "mtwm_background.c"
#include "mtwm_client.c"
#include "mtwm_client_table.c"
#include "mtwm_client_creation.c"
#include "mtwm_client_resize.c"

#include "mtwm_main.c"

