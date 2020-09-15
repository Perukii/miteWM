
void mtwm_resize_window(mtwm_client *, int, int, int, int, int, int, Bool, Bool);
void mtwm_config_notify(mtwm_client *, XEvent *);

void mtwm_resize_window(mtwm_client * _client,
                        int _x,
                        int _y,
                        int _width,
                        int _height,
                        int _x_diff,
                        int _y_diff,
                        Bool _x_move,
                        Bool _y_move){

    if(XEventsQueued(mtwm_display,QueuedAlready) >= 2) return;

    if(_x_move) _x_diff *= -1;
    if(_y_move) _y_diff *= -1;
    
    XSizeHints hints;
    mtwm_get_size_hints(_client, &hints);

    int f_width  = _width  + _x_diff;
    int f_height = _height + _y_diff;
    const int f_width_min  = _client->local_border_width + 1 + hints.min_width;
    const int f_height_min = _client->local_border_height+ 1 + hints.min_height;

    if(f_width  <=  f_width_min){
        _x_diff += (f_width_min-f_width);
        f_width  =  f_width_min;
    }
    
    if(f_height <=  f_height_min){
        _y_diff += (f_height_min-f_height);
        f_height =  f_height_min;
    }
    
    XMoveWindow(mtwm_display, _client->window[MTWM_CLIENT_BOX],
                _x+_x_diff*(-_x_move),
                _y+_y_diff*(-_y_move));

    XResizeWindow(mtwm_display, _client->window[MTWM_CLIENT_APP],
                f_width  - _client->local_border_width,
                f_height - _client->local_border_height);    

    // この後、mtwm_config_notify()が呼び出されるわけ
                
}

void mtwm_config_notify(mtwm_client * _client, XEvent * _event){
    
    if(_client == NULL) return;

    int f_width =  _event->xconfigure.width  + _client->local_border_width;
    int f_height = _event->xconfigure.height + _client->local_border_height;

    XResizeWindow(mtwm_display, _client->window[MTWM_CLIENT_BOX],
        f_width,
        f_height);

    cairo_xlib_surface_set_size( _client->surface[MTWM_CLIENT_BOX],
                f_width,
                f_height);

    // 描画を更新。
    mtwm_draw_client(_client);
}