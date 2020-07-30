

#define MTWM_CLIENT_WIDGETS    4
    #define MTWM_CLIENT_BOX      0
    #define MTWM_CLIENT_APP      1
    #define MTWM_CLIENT_TITLEBAR 2
    #define MTWM_CLIENT_EXIT     3

typedef struct{
    Window window[MTWM_CLIENT_WIDGETS];
} mtwm_client;

typedef struct{
    mtwm_client client;
    Bool is_enable;
    size_t code_forward, code_backward;
} mtwm_client_table_index;

typedef struct{
    mtwm_client_table_index * hasharray;
    size_t current_size, capacity_size, hashcode_max;
} mtwm_client_table;

mtwm_client mtwm_null_client(){
    mtwm_client res_client;
    for(unsigned int i=0; i<MTWM_CLIENT_WIDGETS; i++) res_client.window[i] = None;
    return res_client;
}

mtwm_client_table_index mtwm_null_client_index(){
    mtwm_client_table_index res_client;
    res_client.client = mtwm_null_client();
    res_client.is_enable = False;
    res_client.code_forward = __SIZE_MAX__;
    res_client.code_backward= __SIZE_MAX__;
    return res_client;
}

int mtwm_client_table_init(mtwm_client_table * _table, size_t _hashcode_max){

    _table->hasharray = malloc(sizeof(mtwm_client_table_index) * _hashcode_max*2);

    for(size_t i=0; i<_hashcode_max*2; i++){
        _table->hasharray[i] = mtwm_null_client_index();
    }

    _table->hashcode_max = _hashcode_max;
    _table->current_size = _hashcode_max;
    _table->capacity_size= _hashcode_max*2;

    return 1;
}

int mtwm_client_table_add(mtwm_client_table * _table, mtwm_client _client){

    size_t adress = _client.window[MTWM_CLIENT_BOX] % _table->hashcode_max;

    mtwm_client_table_index index;
    index.client = _client;
    index.code_forward = __SIZE_MAX__;
    index.code_backward= __SIZE_MAX__;
    index.is_enable = True;

    while(_table->hasharray[adress].client.window[0] != None){

        if(_table->hasharray[adress].code_forward != __SIZE_MAX__)
            adress = _table->hasharray[adress].code_forward;
        else{

            size_t pre_adress = _table->current_size;

            _table->current_size++;

            if(_table->current_size == _table->capacity_size){
                _table->capacity_size *= 2;
                _table->hasharray =
                    realloc(_table->hasharray,
                            sizeof(mtwm_client_table_index) * _table->capacity_size);
                for(size_t i=_table->capacity_size/2; i<_table->capacity_size; i++){
                    _table->hasharray[i] = mtwm_null_client_index();
                }
            }

            _table->hasharray[adress].code_forward = pre_adress;
            index.code_backward = adress;

            adress = pre_adress;
        }
    }

    _table->hasharray[adress] = index;
    return 1;
}

size_t mtwm_client_table_window_exists(mtwm_client_table * _table, Window _target){

    size_t adress = _target % _table->hashcode_max;

    while(1){

        if(_table->hasharray[adress].client.window[MTWM_CLIENT_BOX] == _target){
            if(_table->hasharray[adress].is_enable == True)
                return adress;
            else
                return __SIZE_MAX__;
        }

        if(_table->hasharray[adress].code_forward != __SIZE_MAX__)
            adress = _table->hasharray[adress].code_forward;
        else
            return __SIZE_MAX__;

    }
    return __SIZE_MAX__;
}

mtwm_client mtwm_client_table_find(mtwm_client_table * _table, Window _target){
    int address = mtwm_client_table_window_exists(_table, _target);
    if(address != __SIZE_MAX__)
        return _table->hasharray[address].client;
    else 
        return mtwm_null_client();
}

int mtwm_client_table_delete(mtwm_client_table * _table, Window _target){
    int address = mtwm_client_table_window_exists(_table, _target);
    if(address != __SIZE_MAX__){
        _table->hasharray[address].is_enable = False;
        return 1;
    }
    else
        return 0;
}

int mtwm_client_table_free(mtwm_client_table * _table){

    free(_table->hasharray);

    return 1;
}

