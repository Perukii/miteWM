
// クライアント
typedef struct{
    Window           window [MTWM_CLIENT_WIDGETS];
    cairo_surface_t *surface[MTWM_CLIENT_WIDGETS-1];
    cairo_t         *cr     [MTWM_CLIENT_WIDGETS-1];
    char * title;
    int local_border_width;
    int local_border_height;
} mtwm_client;


void mtwm_draw_client(mtwm_client * _client){

    const int overall_width  = cairo_xlib_surface_get_width (_client->surface[MTWM_CLIENT_BOX]);
    const int overall_height = cairo_xlib_surface_get_height(_client->surface[MTWM_CLIENT_BOX]);

    const int client_width =
        overall_width  - mtwm_config_empty_box_border*2;
    const int client_height=
        overall_height - mtwm_config_empty_box_border*2;
    
    const double shade_box_border = mtwm_config_box_border - mtwm_config_empty_box_border + mtwm_config_shadow_roughness;

    for(double i=0; i<shade_box_border; i += mtwm_config_shadow_roughness){
        cairo_set_source_rgba(_client->cr[MTWM_CLIENT_BOX], 0, 0, 0, 0.3/shade_box_border*i);
        cairo_rectangle(_client->cr[MTWM_CLIENT_BOX],
                        i+mtwm_config_empty_box_border,
                        i+mtwm_config_empty_box_border,
                        client_width-i*2, client_height-i*2);
        cairo_stroke(_client->cr[MTWM_CLIENT_BOX]);
    }

    // TITLEBARの描画。

    cairo_pattern_t * titlebar_pattern;

    const double titlebar_width = overall_width - _client->local_border_width;

    titlebar_pattern = cairo_pattern_create_linear (0.0,
                                                    0.0,
                                                    titlebar_width*0.0,
                                                    mtwm_config_titlebar_height*1.0);

    cairo_pattern_add_color_stop_rgba (titlebar_pattern, 1, 1.0, 1.0, 1.0, 1.0);
    cairo_pattern_add_color_stop_rgba (titlebar_pattern, 0, 1.0, 1.0, 1.0, 0.5);
    cairo_set_source(_client->cr[MTWM_CLIENT_BOX], titlebar_pattern);

    cairo_rectangle(_client->cr[MTWM_CLIENT_BOX],
                    _client->local_border_width/2,
                    mtwm_config_box_border ,
                    titlebar_width,
                    mtwm_config_titlebar_height);
    cairo_fill(_client->cr[MTWM_CLIENT_BOX]);

    cairo_set_source_rgb(_client->cr[MTWM_CLIENT_BOX], 0.4, 0.4, 0.4);

    cairo_select_font_face(_client->cr[MTWM_CLIENT_BOX], "Arial",
        CAIRO_FONT_SLANT_NORMAL,
        CAIRO_FONT_WEIGHT_BOLD);

    cairo_set_font_size(_client->cr[MTWM_CLIENT_BOX], 20);
    cairo_move_to(_client->cr[MTWM_CLIENT_BOX], _client->local_border_width/2, mtwm_config_titlebar_height + mtwm_config_box_border);
    cairo_show_text(_client->cr[MTWM_CLIENT_BOX], _client->title); 

    // EXITボタンの描画

    const double exit_x = overall_width - mtwm_config_titlebar_width_margin - _client->local_border_width/2;
    double margin_width = 4;
    double shadow_width = 2;
    
    cairo_set_source_rgb(_client->cr[MTWM_CLIENT_BOX], 1.0, 0.3, 0.3);
    cairo_rectangle(_client->cr[MTWM_CLIENT_BOX],
                    exit_x                            + margin_width,
                    mtwm_config_box_border            + margin_width,
                    mtwm_config_titlebar_width_margin - margin_width*2,
                    mtwm_config_titlebar_height       - margin_width*2);
    cairo_fill_preserve(_client->cr[MTWM_CLIENT_BOX]);

    cairo_set_source_rgb(_client->cr[MTWM_CLIENT_BOX], 0.7, 0.2, 0.2);
    cairo_set_line_width(_client->cr[MTWM_CLIENT_BOX], shadow_width);
    cairo_stroke(_client->cr[MTWM_CLIENT_BOX]);

    //cairo_surface_flush(_client->surface[MTWM_CLIENT_BOX]);
    
}

void mtwm_set_button_event_info(mtwm_client * _client,
                                int pointer_x,
                                int pointer_y,
                                int window_width,
                                int window_height,
                                unsigned int * _event_property){
    (*_event_property) = 0;

    int start  = _client->local_border_width/2 ;
    int top    = mtwm_config_box_border;
    int end    = window_width  - _client->local_border_width/2 ;
    int bottom = window_height - mtwm_config_box_border;

    if(
        pointer_x >= end - mtwm_config_titlebar_width_margin &&
        pointer_y <= top + mtwm_config_titlebar_height &&
        pointer_x <= end &&
        pointer_y >= top
    ){
        (*_event_property) += 1<<MTWM_EXIT_PRESSED;
        return;
    }

    if(pointer_x < start)  (*_event_property)+= 1<<MTWM_RESIZE_ANGLE_START;
    if(pointer_y < top)    (*_event_property)+= 1<<MTWM_RESIZE_ANGLE_TOP;
    if(pointer_x > end)    (*_event_property)+= 1<<MTWM_RESIZE_ANGLE_END;
    if(pointer_y > bottom) (*_event_property)+= 1<<MTWM_RESIZE_ANGLE_BOTTOM;

}