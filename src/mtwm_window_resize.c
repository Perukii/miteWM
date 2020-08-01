
void mtwm_resize_window(mtwm_client * _client, int _width, int _height){

    if(_width < 1 || _height < 1) return;

    XResizeWindow(mtwm_display, _client->window[MTWM_CLIENT_BOX],
                _width + _client->border_width,
                _height+ _client->border_height);
    
    XResizeWindow(mtwm_display, _client->window[MTWM_CLIENT_APP], _width, _height);

    cairo_xlib_surface_set_size( *(_client->surface), _width + _client->border_width, _height+ _client->border_height);
}