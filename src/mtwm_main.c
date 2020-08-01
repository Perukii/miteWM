

int main(){
    
    mtwm_display    = XOpenDisplay(0);
    if(mtwm_display == NULL) return 1;

    XEvent event;
    mtwm_client_table client_table;
    mtwm_client_table_init(&client_table, 10);

    // ==== 根ウインドウ ====

    mtwm_root_window = XDefaultRootWindow(mtwm_display);

    XSelectInput(mtwm_display, mtwm_root_window,
                 ButtonPressMask   | ButtonReleaseMask |
                 PointerMotionMask | SubstructureNotifyMask
                );

    mtwm_set_background("/home/tada/Documents/Code/Dir2/MitsuWM/screen.png");

    struct{ unsigned int button, x_root, y_root; Window window; XWindowAttributes attributes; }
        grip_info;

    grip_info.button = 0;
    grip_info.window = None;
    grip_info.x_root = 0;
    grip_info.y_root = 0;

    while(1){

        if(XNextEvent(mtwm_display, &event) < 0)break;
        
        switch(event.type){

          case MapNotify:
            if(event.xmap.window == mtwm_background.window){
                mtwm_draw_background();
                break;
            }
            mtwm_new_client(&client_table, event.xmap.window);
            
            break;

          case ButtonPress:
            if(event.xbutton.subwindow == None) break;
            if(event.xbutton.subwindow == mtwm_background.window) break;
            XGetWindowAttributes(mtwm_display, event.xbutton.subwindow, &grip_info.attributes);
            grip_info.button = event.xbutton.button;
            grip_info.window = event.xbutton.subwindow;
            grip_info.x_root = event.xbutton.x_root;
            grip_info.y_root = event.xbutton.y_root;
            break;

          case ButtonRelease:
            grip_info.window = None;
            break;
          
          case MotionNotify:

            if(grip_info.window == None) break;

            mtwm_client client = mtwm_client_table_find(&client_table, grip_info.window);

            int x_diff = event.xbutton.x_root - grip_info.x_root;
            int y_diff = event.xbutton.y_root - grip_info.y_root;

            if(grip_info.button == 1){
                XMoveWindow(mtwm_display, grip_info.window,
                            grip_info.attributes.x + x_diff,
                            grip_info.attributes.y + y_diff);
            }
            else{
                mtwm_resize_window(&client, grip_info.attributes.width + x_diff, grip_info.attributes.height + y_diff);
            }

            mtwm_draw_background();
            mtwm_draw_client(&client);
            XFlush(mtwm_display);
            break;
        }
    }

    mtwm_client_table_free(&client_table);

    return 1;
}