
// TODO : 各要素の初期化
struct{
    Window           window;
    cairo_surface_t *surface;
    cairo_surface_t *image;
    cairo_t         *cr;
    double           image_scale;
} mtwm_background;


void mtwm_set_background(const char * _file){

    XWindowAttributes root_attributes;
    XGetWindowAttributes(mtwm_display, mtwm_root_window, &root_attributes);

    mtwm_background.window = XCreateSimpleWindow(
        mtwm_display, mtwm_root_window,
        root_attributes.x, root_attributes.y, root_attributes.width, root_attributes.height,
        0, 0, BlackPixel(mtwm_display, 0)
    );

    mtwm_background.surface = cairo_xlib_surface_create(mtwm_display, mtwm_background.window,
                              DefaultVisual(mtwm_display, DefaultScreen(mtwm_display)), root_attributes.width, root_attributes.height);

    mtwm_background.cr = cairo_create(mtwm_background.surface);
    mtwm_background.image = cairo_image_surface_create_from_png(_file);

    const double image_width  = cairo_image_surface_get_width (mtwm_background.image);
    const double image_height = cairo_image_surface_get_height(mtwm_background.image);

    mtwm_background.image_scale =
                root_attributes.width/image_width > root_attributes.height/image_height ?
                root_attributes.width/image_width : root_attributes.height/image_height;

    cairo_set_antialias(mtwm_background.cr, CAIRO_ANTIALIAS_SUBPIXEL);
    
    XMapWindow(mtwm_display, mtwm_background.window);
    
}

void mtwm_draw_background(){

    if(mtwm_background.image == NULL) return;

    cairo_save(mtwm_background.cr);

    cairo_scale(mtwm_background.cr,
                mtwm_background.image_scale, mtwm_background.image_scale);

    cairo_set_source_surface(mtwm_background.cr, mtwm_background.image, 0, 0);

    cairo_paint(mtwm_background.cr);

    cairo_restore(mtwm_background.cr);
    cairo_surface_flush(mtwm_background.surface);
}
