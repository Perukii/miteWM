

int main(int argc, char ** argv){

    mtwm_display    = XOpenDisplay(0);
    if(mtwm_display == NULL) return 1;

    // 背景画像のファイル
    char * background_file = "";

    // JSONファイルを読み込む処理。
    struct json_object * json_obj;
    if(argc > 1){

        printf("%s\n",argv[1]);
        json_obj = json_object_from_file(argv[1]);

        if(json_obj != NULL){
            json_object_object_foreach(json_obj, key, val) {
                if( strcmp(key,"background_image_file") == 0 ){
                    background_file = (char*)json_object_get_string(val);
                }
            }
        }
    }



    // イベント、クライアントテーブル
    XEvent event;
    mtwm_client_table client_table;
    mtwm_client_table_init(&client_table, 10);

    // ==== 根ウインドウ ====

    mtwm_root_window = XDefaultRootWindow(mtwm_display);

    XSelectInput(mtwm_display, mtwm_root_window,
                 ButtonPressMask   | ButtonReleaseMask |
                 PointerMotionMask | SubstructureNotifyMask
                );

    if(background_file != ""){
        mtwm_set_background(background_file);
    }

    XDefineCursor(mtwm_display, mtwm_root_window,
                  XCreateFontCursor(mtwm_display, XC_left_ptr));
   

    // 掴まれているウインドウの情報。
    struct{
        unsigned int button, event_property;
        int x_root, y_root;
        Window window;
        XWindowAttributes attributes;
    } grip_info;

    // ...の、初期化。
    grip_info.button = 0;
    grip_info.event_property = 0;
    grip_info.window = None;
    grip_info.x_root = 0;
    grip_info.y_root = 0;

    // 最後にUngrabされた = 現在フォーカスされているウインドウ。
    Window last_ungrabbed_app = None;
    
    // メインループ。
    //
    while(1){
        
        if(XPending(mtwm_display) < 0) goto exit;

        // イベントを取得。
        if(XNextEvent(mtwm_display, &event) < 0) goto exit;
        
        switch(event.type){

        /**/case MapNotify:
            // override_redirectな奴に用は無い。（ポップアップメニューなど）
            if(event.xmap.override_redirect == True) break;

            // バックグラウンドウインドウなら、それを描画する。
            if(event.xmap.window == mtwm_background.window){
                mtwm_draw_background();
                break;
            }

            // それ以外は、基本的に新しいクライアントとして登録。
            mtwm_new_client(&client_table, event.xmap.window, &last_ungrabbed_app);
            
            break;

        /**/case ButtonPress:
            {

            // 無またはバックグラウンドウインドウを取得した場合
            if(event.xbutton.subwindow == None) break;
            if(event.xbutton.subwindow == mtwm_background.window) break;

            mtwm_client * client = mtwm_client_table_find(&client_table, event.xbutton.subwindow);

            if(client == NULL){
                client = mtwm_client_table_find_from_app(&client_table, event.xbutton.window);
                if(client == NULL) break;
            }
            else{
                // 掴まれているウインドウの情報を更新する作業。
                XGetWindowAttributes(mtwm_display, client->window[MTWM_CLIENT_BOX], &grip_info.attributes);

                grip_info.button = event.xbutton.button;
                grip_info.window = event.xbutton.subwindow;
                grip_info.x_root = event.xbutton.x_root;
                grip_info.y_root = event.xbutton.y_root;

                // マウスボタンイベント情報を手に入れる。
                // ここでgrip_info.event_propertyに格納された値は、MotionNotifyの項でも利用する。
                mtwm_set_button_event_info(
                    client,
                    event.xmotion.x - grip_info.attributes.x,
                    event.xmotion.y - grip_info.attributes.y,
                    grip_info.attributes.width,
                    grip_info.attributes.height,
                    &grip_info.event_property);
                
                // EXITボタンが押されている時
                if((grip_info.event_property>>MTWM_EXIT_PRESSED)%2){
                    
                    // APPが「自ら除去イベントを送信した」という状況を再現している。
                    // しかし、APPは実は本意ではなかったのかもしれない。
                    // 悲しい。

                    // 除去イベントを設定
                    XEvent delete_event;
                    delete_event.xclient.type = ClientMessage;
                    delete_event.xclient.message_type = XInternAtom(mtwm_display, "WM_PROTOCOLS", True);
                    delete_event.xclient.format = 32;
                    delete_event.xclient.data.l[0] = XInternAtom(mtwm_display, "WM_DELETE_WINDOW", True);
                    delete_event.xclient.data.l[1] = CurrentTime;
                    delete_event.xclient.window = client->window[MTWM_CLIENT_APP];

                    // 除去イベントを送信
                    XSendEvent(mtwm_display, client->window[MTWM_CLIENT_APP], False, NoEventMask, &delete_event);
                }
            }

            XRaiseWindow(mtwm_display, client->window[MTWM_CLIENT_BOX]);
            XSetInputFocus(mtwm_display, client->window[MTWM_CLIENT_APP], RevertToNone, CurrentTime);
            
            if(last_ungrabbed_app != None){
                XGrabButton(mtwm_display, AnyButton, AnyModifier, last_ungrabbed_app, False,
                    ButtonPressMask, GrabModeAsync, GrabModeAsync, None, None);
            }
            XUngrabButton(mtwm_display, AnyButton, AnyModifier, client->window[MTWM_CLIENT_APP]);
            last_ungrabbed_app = client->window[MTWM_CLIENT_APP];

            break;
            }

        /**/case ButtonRelease:
            // 掴んだウインドウを離す。
            grip_info.window = None;
            break;

        /**/case ConfigureNotify:
            {
            mtwm_client * client = mtwm_client_table_find_from_app(&client_table, event.xconfigure.window);
            if(client == NULL){
                break;
            }

            int f_width = event.xconfigure.width  + client->local_border_width;
            int f_height = event.xconfigure.height + client->local_border_height;

            XResizeWindow(mtwm_display, client->window[MTWM_CLIENT_BOX],
                f_width,
                f_height);  

            cairo_xlib_surface_set_size( client->surface[MTWM_CLIENT_BOX],
                        f_width,
                        f_height);

            // 描画を更新。
            mtwm_draw_client(client);
            
            break;
            }
            
        /**/case DestroyNotify:
            // 除去イベント。必ずAPPが除去されている時に送信されなければいけない。

            // BOXもろとも除去してしまう。
            if(event.xclient.window == None) break;
            if(event.xclient.window == mtwm_root_window) break;

            mtwm_client * client = mtwm_client_table_find(&client_table, event.xclient.window);

            if(client != NULL){
                if(last_ungrabbed_app == client->window[MTWM_CLIENT_APP]){
                    last_ungrabbed_app = None;
                }
                XDestroyWindow(mtwm_display, client->window[MTWM_CLIENT_BOX]);
                mtwm_client_table_delete(&client_table, client->window[MTWM_CLIENT_BOX]);
            }
            break;

        /**/case MotionNotify:
            {
            if(grip_info.window == None) break;

            mtwm_client * client = mtwm_client_table_find(&client_table, grip_info.window);
            if(client == NULL) break;

            int x_diff = event.xbutton.x_root - grip_info.x_root;
            int y_diff = event.xbutton.y_root - grip_info.y_root;

            // 掴んでいる部分がウインドウの端でなければ
            if(grip_info.event_property == 0){
                // ウインドウを動かす。
                XMoveWindow(mtwm_display, grip_info.window,
                            grip_info.attributes.x + x_diff,
                            grip_info.attributes.y + y_diff);
            }
            // 端であれば
            else{

                // 掴んでいる位置によって、x及びy方向へのresizeが適用されないこともある。
                x_diff *= ( (grip_info.event_property>>MTWM_RESIZE_ANGLE_START)%2 || (grip_info.event_property>>MTWM_RESIZE_ANGLE_END   )%2);
                y_diff *= ( (grip_info.event_property>>MTWM_RESIZE_ANGLE_TOP  )%2 || (grip_info.event_property>>MTWM_RESIZE_ANGLE_BOTTOM)%2);

                mtwm_resize_window(client,
                                   grip_info.attributes.x,
                                   grip_info.attributes.y,
                                   grip_info.attributes.width,
                                   grip_info.attributes.height,
                                   x_diff,
                                   y_diff,
                                   (grip_info.event_property>>MTWM_RESIZE_ANGLE_START)%2,
                                   (grip_info.event_property>>MTWM_RESIZE_ANGLE_TOP  )%2);
                

            }
            
            // 描画を更新。
            //mtwm_draw_background();
            //mtwm_draw_client(client);
            
            break;
            }

            default:break;
        }
    }

exit:
    mtwm_client_table_free(&client_table);

    return 1;
}