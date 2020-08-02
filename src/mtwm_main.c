

int main(){
    
    mtwm_display    = XOpenDisplay(0);
    if(mtwm_display == NULL) return 1;

    XEvent event;
    XEvent delete_event;
    Atom   delete_atom;
    mtwm_client_table client_table;
    mtwm_client_table_init(&client_table, 10);

    // ==== 根ウインドウ ====

    mtwm_root_window = XDefaultRootWindow(mtwm_display);

    XSelectInput(mtwm_display, mtwm_root_window,
                 ButtonPressMask   | ButtonReleaseMask |
                 PointerMotionMask | SubstructureNotifyMask
                );

    mtwm_set_background("/home/tada/Documents/Code/Dir2/MitsuWM/screen.png");

    struct{
        unsigned int button, event_property;
        int x_root, y_root;
        Window window;
        XWindowAttributes attributes;
    } grip_info;

    grip_info.button = 0;
    grip_info.window = None;
    grip_info.x_root = 0;
    grip_info.y_root = 0;
    grip_info.event_property = 0;

    while(1){

        if(XNextEvent(mtwm_display, &event) < 0)break;
        
        switch(event.type){

        /**/case MapNotify:

            if(event.xmap.override_redirect == True) break;

            if(event.xmap.window == mtwm_background.window){
                mtwm_draw_background();
                break;
            }
            mtwm_new_client(&client_table, event.xmap.window);
            
            break;

        /**/case ButtonPress:
            {
            if(event.xbutton.subwindow == None) break;
            if(event.xbutton.subwindow == mtwm_background.window) break;

            XGetWindowAttributes(mtwm_display, event.xbutton.subwindow, &grip_info.attributes);

            mtwm_client client = mtwm_client_table_find(&client_table, event.xbutton.subwindow);
            if(client.window[0] == None) break;

            grip_info.button = event.xbutton.button;
            grip_info.window = event.xbutton.subwindow;
            grip_info.x_root = event.xbutton.x_root;
            grip_info.y_root = event.xbutton.y_root;

            mtwm_set_button_event_info(
                &client,
                event.xmotion.x - grip_info.attributes.x,
                event.xmotion.y - grip_info.attributes.y,
                grip_info.attributes.width,
                grip_info.attributes.height,
                &grip_info.event_property);
              
            if((grip_info.event_property>>MTWM_EXIT_PRESSED)%2){

                delete_atom = XInternAtom(mtwm_display, "WM_DELETE_WINDOW", True);

                delete_event.xclient.type = ClientMessage;
                delete_event.xclient.message_type = XInternAtom(mtwm_display, "WM_PROTOCOLS", True);
                delete_event.xclient.format = 32;
                delete_event.xclient.data.l[0] = delete_atom;
                delete_event.xclient.data.l[1] = CurrentTime;
                //delete_event.xclient.data.l[2] = client.window[MTWM_CLIENT_BOX];
                delete_event.xclient.window = client.window[MTWM_CLIENT_APP];
                XSendEvent(mtwm_display, client.window[MTWM_CLIENT_APP], False, NoEventMask, &delete_event);
            }
            break;
            }

        /**/case ButtonRelease:
            grip_info.window = None;
            break;

        /**/case DestroyNotify:
            {

            if(mtwm_client_table_find(&client_table, event.xclient.window).window[0] != None){
                XDestroyWindow(mtwm_display, event.xclient.window);
            }
            break;
            }

        /**/case MotionNotify:
            {
            if(grip_info.window == None) break;

            mtwm_client client = mtwm_client_table_find(&client_table, grip_info.window);
            if(client.window[0] == None) break;

            int x_diff = event.xbutton.x_root - grip_info.x_root;
            int y_diff = event.xbutton.y_root - grip_info.y_root;

            if(grip_info.event_property == 0){
                XMoveWindow(mtwm_display, grip_info.window,
                            grip_info.attributes.x + x_diff,
                            grip_info.attributes.y + y_diff);
            }
            else{
                x_diff *= ( (grip_info.event_property>>MTWM_RESIZE_ANGLE_START)%2 || (grip_info.event_property>>MTWM_RESIZE_ANGLE_END   )%2);
                y_diff *= ( (grip_info.event_property>>MTWM_RESIZE_ANGLE_TOP  )%2 || (grip_info.event_property>>MTWM_RESIZE_ANGLE_BOTTOM)%2);

                mtwm_resize_window(&client,
                                   grip_info.attributes.x,
                                   grip_info.attributes.y,
                                   grip_info.attributes.width,
                                   grip_info.attributes.height,
                                   x_diff,
                                   y_diff,
                                   (grip_info.event_property>>MTWM_RESIZE_ANGLE_START)%2,
                                   (grip_info.event_property>>MTWM_RESIZE_ANGLE_TOP  )%2);
                

            }

            mtwm_draw_background();
            mtwm_draw_client(&client);
            XFlush(mtwm_display);
            break;
            }
        }
    }

    mtwm_client_table_free(&client_table);

    return 1;
}