
void mtwm_get_size_hints(mtwm_client * _client, XSizeHints * hints){
    long supplied;
    XGetWMNormalHints(mtwm_display, _client->window[MTWM_CLIENT_APP], hints, &supplied);
}

void mtwm_get_class_hint(mtwm_client * _client, XClassHint * hint){
    XGetClassHint(mtwm_display, _client->window[MTWM_CLIENT_APP], hint);
}

int mtwm_new_client(mtwm_client_table * _client_table,
                    Window _targ_window){

    if(_targ_window == None) return 0;
    if(mtwm_client_table_find(_client_table, _targ_window)!= NULL) return 0;

    XWindowAttributes targ_attributes;
    XGetWindowAttributes(mtwm_display, _targ_window, &targ_attributes);

    // ---------------
    // ===== APP =====
    // ---------------

    mtwm_client client;
    client.window[MTWM_CLIENT_APP] = _targ_window;

    // ---------------
    // ===== BOX =====
    // ---------------

    client.local_border_width  =
        mtwm_width_diff  + targ_attributes.border_width*2;
    client.local_border_height =
        mtwm_height_diff + targ_attributes.border_width*2;

    unsigned int box_width = client.local_border_width + targ_attributes.width;
    unsigned int box_height = client.local_border_height + targ_attributes.height;

    XVisualInfo box_visual_info;
    XMatchVisualInfo(mtwm_display, DefaultScreen(mtwm_display), 32, TrueColor, &box_visual_info);

    XSetWindowAttributes box_attributes;
    box_attributes.colormap = XCreateColormap(mtwm_display, mtwm_root_window, box_visual_info.visual, AllocNone);
    box_attributes.border_pixel = 0;
    box_attributes.background_pixel = 0;
    box_attributes.override_redirect = 1;

    // BOXを作成
    client.window[MTWM_CLIENT_BOX] = XCreateWindow(
        mtwm_display,
        mtwm_root_window,
        targ_attributes.x,
        targ_attributes.y,
        box_width,
        box_height,
        0, box_visual_info.depth,
        InputOutput, box_visual_info.visual,
        CWColormap | CWBorderPixel | CWBackPixel | CWOverrideRedirect, &box_attributes
    );

    // ウインドウの設定
    XReparentWindow      (mtwm_display, client.window[MTWM_CLIENT_APP], client.window[MTWM_CLIENT_BOX],
                          mtwm_config_box_border, mtwm_config_titlebar_height + mtwm_config_box_border);
    XDefineCursor        (mtwm_display, client.window[MTWM_CLIENT_BOX],
                          XCreateFontCursor(mtwm_display, XC_plus));
    XMapWindow           (mtwm_display, client.window[MTWM_CLIENT_BOX]);
    XSelectInput         (mtwm_display, client.window[MTWM_CLIENT_BOX], SubstructureNotifyMask);

    // ウインドウのcairoコンテキストを作成

    client.surface[MTWM_CLIENT_BOX] =
            cairo_xlib_surface_create(
                    mtwm_display,
                    client.window[MTWM_CLIENT_BOX],
                    box_visual_info.visual,
                    box_width,
                    box_height);
    client.cr[MTWM_CLIENT_BOX] = cairo_create(client.surface[MTWM_CLIENT_BOX]);
    
    XClassHint hint;
    mtwm_get_class_hint(&client, &hint);
    client.title = hint.res_class;

    cairo_set_operator  (client.cr[MTWM_CLIENT_BOX], CAIRO_OPERATOR_SOURCE);
    cairo_set_antialias (client.cr[MTWM_CLIENT_BOX], CAIRO_ANTIALIAS_SUBPIXEL);
    cairo_set_line_width(client.cr[MTWM_CLIENT_BOX], mtwm_config_shadow_roughness);

    // コンテキストを更新
    mtwm_draw_client(&client);
    mtwm_client_table_add(_client_table, client);


}
