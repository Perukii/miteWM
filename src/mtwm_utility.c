
mtwm_client * mtwm_client_table_find_from_app(mtwm_client_table * _client_table, Window _window){
    Window parent, root, *child;
    unsigned int child_num;
    XQueryTree(mtwm_display, _window, &root, &parent, &child, &child_num);
    return mtwm_client_table_find(_client_table, parent);
}

