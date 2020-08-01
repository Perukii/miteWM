
// クライアント
typedef struct{
    Window           window [MTWM_CLIENT_WIDGETS];
    cairo_surface_t *surface[MTWM_CLIENT_WIDGETS-1];
    cairo_t         *cr     [MTWM_CLIENT_WIDGETS-1];
    unsigned int local_border_width;
    unsigned int local_border_height;
} mtwm_client;


int mtwm_draw_client(mtwm_client * _client){

    unsigned int client_width = cairo_xlib_surface_get_width (_client->surface[MTWM_CLIENT_BOX]);
    unsigned int client_height= cairo_xlib_surface_get_height(_client->surface[MTWM_CLIENT_BOX]);
    
    double box_border = mtwm_config_box_border + mtwm_config_shadow_roughness;

    for(double i=0; i<box_border; i += mtwm_config_shadow_roughness){
        cairo_set_source_rgba(_client->cr[MTWM_CLIENT_BOX], 0, 0, 0, 0.5/box_border*i);
        cairo_rectangle(_client->cr[MTWM_CLIENT_BOX], i,i, client_width-i*2, client_height-i*2);
        
        cairo_stroke(_client->cr[MTWM_CLIENT_BOX]);
    }
    cairo_surface_flush(_client->surface[MTWM_CLIENT_BOX]);
    
    cairo_set_source_rgb(_client->cr[MTWM_CLIENT_EXIT], 1, 0.5, 0.5);
    cairo_paint(_client->cr[MTWM_CLIENT_EXIT]);
    cairo_surface_flush(_client->surface[MTWM_CLIENT_EXIT]);
}