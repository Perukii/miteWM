
void mtwm_resize_window(mtwm_client * _client,
                        int _x,
                        int _y,
                        int _width,
                        int _height,
                        int _x_diff,
                        int _y_diff,
                        Bool _x_move,
                        Bool _y_move){

    if(_x_move) _x_diff *= -1;
    if(_y_move) _y_diff *= -1;
    
    int f_width  = _width  + _x_diff;
    int f_height = _height + _y_diff;
    const int f_width_max  = _client->local_border_width+1;
    const int f_height_max = _client->local_border_height+1;

    if(f_width  <=  f_width_max){
        _x_diff += (f_width_max-f_width);
        f_width  =  f_width_max;
    }
    
    if(f_height <=  f_height_max){
        _y_diff += (f_height_max-f_height);
        f_height =  f_height_max;
    }
    
    XResizeWindow(mtwm_display, _client->window[MTWM_CLIENT_BOX],
                f_width,
                f_height);
    
    XResizeWindow(mtwm_display, _client->window[MTWM_CLIENT_APP],
                f_width  - _client->local_border_width,
                f_height - _client->local_border_height);

    XMoveWindow(mtwm_display, _client->window[MTWM_CLIENT_BOX],
                _x+_x_diff*(-_x_move),
                _y+_y_diff*(-_y_move));

    // TODO : サイズを0未満に変えようとするとフリーズ

    cairo_xlib_surface_set_size( _client->surface[MTWM_CLIENT_BOX],
                f_width,
                f_height);
                

/*
    XMoveWindow(mtwm_display, _client->window[MTWM_CLIENT_EXIT],
                _width + _client->local_border_width/2 - mtwm_config_titlebar_width_margin,
                mtwm_config_box_border);
*/
}