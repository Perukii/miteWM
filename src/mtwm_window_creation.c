


int mtwm_draw_client(mtwm_client * _client){

    unsigned int client_width = cairo_xlib_surface_get_width (_client->surface[MTWM_CLIENT_BOX]);
    unsigned int client_height= cairo_xlib_surface_get_height(_client->surface[MTWM_CLIENT_BOX]);
    
    double box_border = mtwm_config_box_border + mtwm_config_shadow_roughness;

    cairo_set_operator(_client->cr[MTWM_CLIENT_BOX], CAIRO_OPERATOR_SOURCE);
    cairo_set_line_width(_client->cr[MTWM_CLIENT_BOX], mtwm_config_shadow_roughness);

    for(double i=0; i<box_border; i += mtwm_config_shadow_roughness){
        cairo_set_source_rgba(_client->cr[MTWM_CLIENT_BOX], 0, 0, 0, 0.5/box_border*i);
        cairo_rectangle(_client->cr[MTWM_CLIENT_BOX], i,i, client_width-i*2, client_height-i*2);
        
        cairo_stroke(_client->cr[MTWM_CLIENT_BOX]);
    }
    cairo_surface_flush(_client->surface[MTWM_CLIENT_BOX]);
    
}

int mtwm_new_client(mtwm_client_table * _client_table, Window _targ_window){

    if(_targ_window == None) return 0;
    if(mtwm_client_table_find(_client_table, _targ_window).window[MTWM_CLIENT_BOX] != 0) return 0;

    XWindowAttributes targ_attributes;
    XGetWindowAttributes(mtwm_display, _targ_window, &targ_attributes);

    mtwm_client client;
    client.window[MTWM_CLIENT_APP] = _targ_window;

    // ---------------
    // ===== BOX =====
    // ---------------

    client.border_width  =
        mtwm_width_diff  + targ_attributes.border_width*2;
    client.border_height =
        mtwm_height_diff + targ_attributes.border_width*2;

    unsigned int box_width = client.border_width + targ_attributes.width;
    unsigned int box_height = client.border_height + targ_attributes.height;

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
                          mtwm_config_box_border, mtwm_config_titlebar_height);
    XDefineCursor        (mtwm_display, client.window[MTWM_CLIENT_BOX],
                          XCreateFontCursor(mtwm_display, XC_plus));
    XSelectInput         (mtwm_display, client.window[MTWM_CLIENT_BOX], SubstructureNotifyMask);
    XMapWindow           (mtwm_display, client.window[MTWM_CLIENT_BOX]);

    // ウインドウのcairoコンテキストを作成

    client.surface[MTWM_CLIENT_BOX] =
            cairo_xlib_surface_create(
                    mtwm_display,
                    client.window[MTWM_CLIENT_BOX],
                    box_visual_info.visual,
                    box_width,
                    box_height);
    client.cr[MTWM_CLIENT_BOX] = cairo_create(client.surface[MTWM_CLIENT_BOX]);



    // ----------------
    // ===== EXIT =====
    // ----------------

    /*
    client.window[MTWM_CLIENT_BOX]
    client.window[MTWM_CLIENT_APP]
    client.window[MTWM_CLIENT_TITLEBAR]
    client.window[MTWM_CLIENT_EXIT]
    */

    // コンテキストを更新
    mtwm_draw_client(&client);
    mtwm_client_table_add(_client_table, client);


}
