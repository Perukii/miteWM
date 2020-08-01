
unsigned long mtwm_color(const char * _color) {

    XColor near_color, true_color;
    XAllocNamedColor(mtwm_display,
                     DefaultColormap(mtwm_display, 0), _color,
                     &near_color, &true_color);

    return BlackPixel(mtwm_display,0)^ near_color.pixel;

}

void mtwm_generate_test_window(){
    Window test = XCreateSimpleWindow(
        mtwm_display, mtwm_root_window, 300, 300, 500, 500, 0, 
        BlackPixel(mtwm_display,0), mtwm_color("orange") );

    XMapWindow(mtwm_display, test);
}