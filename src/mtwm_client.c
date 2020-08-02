
// クライアント
typedef struct{
    Window           window [MTWM_CLIENT_WIDGETS];
    cairo_surface_t *surface[MTWM_CLIENT_WIDGETS-1];
    cairo_t         *cr     [MTWM_CLIENT_WIDGETS-1];
    int local_border_width;
    int local_border_height;
} mtwm_client;


void mtwm_draw_client(mtwm_client * _client){

    unsigned int client_width = cairo_xlib_surface_get_width (_client->surface[MTWM_CLIENT_BOX]);
    unsigned int client_height= cairo_xlib_surface_get_height(_client->surface[MTWM_CLIENT_BOX]);
    
    double shade_box_border = mtwm_config_box_border + mtwm_config_shadow_roughness;

    for(double i=0; i<shade_box_border; i += mtwm_config_shadow_roughness){
        cairo_set_source_rgba(_client->cr[MTWM_CLIENT_BOX], 0, 0, 0, 0.5/shade_box_border*i);
        cairo_rectangle(_client->cr[MTWM_CLIENT_BOX], i,i, client_width-i*2, client_height-i*2);
        cairo_stroke(_client->cr[MTWM_CLIENT_BOX]);
    }

    cairo_set_source_rgb(_client->cr[MTWM_CLIENT_BOX], 0.3, 0.3, 0.3);

    cairo_rectangle(_client->cr[MTWM_CLIENT_BOX],
                    _client->local_border_width/2,
                    mtwm_config_box_border ,
                    client_width - _client->local_border_width, mtwm_config_titlebar_height);
    cairo_fill(_client->cr[MTWM_CLIENT_BOX]);

    cairo_set_source_rgb(_client->cr[MTWM_CLIENT_BOX], 1.0, 0.3, 0.3);

    cairo_rectangle(_client->cr[MTWM_CLIENT_BOX],
                    client_width - mtwm_config_titlebar_width_margin - _client->local_border_width/2,
                    mtwm_config_box_border,
                    mtwm_config_titlebar_width_margin, mtwm_config_titlebar_height);
    cairo_fill(_client->cr[MTWM_CLIENT_BOX]);

    cairo_surface_flush(_client->surface[MTWM_CLIENT_BOX]);
    
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