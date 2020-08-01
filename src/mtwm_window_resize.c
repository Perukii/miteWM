
void mtwm_resize_window(mtwm_client * _client, int _width, int _height){

    if(_width  < 1) _width  = 1;
    if(_height < 1) _height = 1;

    XResizeWindow(mtwm_display, _client->window[MTWM_CLIENT_BOX],
                _width + _client->local_border_width,
                _height+ _client->local_border_height);
    
    XResizeWindow(mtwm_display, _client->window[MTWM_CLIENT_APP], _width, _height);

    XMoveWindow(mtwm_display, _client->window[MTWM_CLIENT_EXIT],
                _width + _client->local_border_width/2 - mtwm_config_titlebar_width_margin,
                mtwm_config_box_border);

    cairo_xlib_surface_set_size( *(_client->surface), _width + _client->local_border_width, _height+ _client->local_border_height);
}