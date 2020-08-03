

typedef struct _mtwm_client_table_index mtwm_client_table_index;
typedef struct _mtwm_client_table       mtwm_client_table;

mtwm_client             mtwm_null_client();
mtwm_client_table_index mtwm_null_client_index();

int           mtwm_client_table_init          (mtwm_client_table *, size_t);
size_t        mtwm_client_table_add           (mtwm_client_table *, mtwm_client);
size_t        mtwm_client_table_window_exists (mtwm_client_table *, Window);
mtwm_client * mtwm_client_table_find          (mtwm_client_table *, Window);
int           mtwm_client_table_delete        (mtwm_client_table *, Window);
int           mtwm_client_table_defragment    (mtwm_client_table *);
int           mtwm_client_table_free          (mtwm_client_table *);


// クライアントのテーブル格納用構造体 (以下クライアントインデックス)
typedef struct _mtwm_client_table_index{
    mtwm_client client;                 // クライアント
    Bool exists;                        // クライアントは有効？
    size_t code_forward, code_backward; // テーブル内の前後要素のアドレス
} mtwm_client_table_index;

// クライアントテーブル
typedef struct _mtwm_client_table{
    mtwm_client_table_index * hasharray;
    size_t current_size;  // 現在のhasharrayのサイズ
    size_t capacity_size; // hasharrayのメモリ確保量
    size_t hashcode_max;  // 最大ハッシュコード
} mtwm_client_table;

// NULLクライアント
mtwm_client mtwm_null_client(){
    mtwm_client res_client;
    for(unsigned int i=0; i<MTWM_CLIENT_WIDGETS; i++) res_client.window[i] = None;

    res_client.local_border_width  = 0;
    res_client.local_border_height = 0;
    return res_client;
}


// NULLクライアントインデックス
mtwm_client_table_index mtwm_null_client_index(){
    mtwm_client_table_index res_client;
    res_client.client = mtwm_null_client();
    res_client.exists = False;
    res_client.code_forward = __SIZE_MAX__;
    res_client.code_backward= __SIZE_MAX__;
    return res_client;
}

// クライアントテーブルを初期化
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

// クライアントテーブルに値を追加
size_t mtwm_client_table_add(mtwm_client_table * _table, mtwm_client _client){

    //mtwm_client_table_defragment(_table);

    // ハッシュ関数
    size_t adress = _client.window[MTWM_CLIENT_BOX] % _table->hashcode_max;

    // 追加するクライアントに情報を付加
    mtwm_client_table_index index;
    index.client = _client;
    index.code_forward = __SIZE_MAX__;
    index.code_backward= __SIZE_MAX__;
    index.exists = True;

    // 探索対象のアドレスの中身がなくなるまでcode_forwardの指すアドレスをたどり続けるループ。
    while(_table->hasharray[adress].client.window[0] != None){

        if(_table->hasharray[adress].code_forward != __SIZE_MAX__)
            adress = _table->hasharray[adress].code_forward;
        else{

            size_t pre_adress = _table->current_size;

            _table->current_size++;

            // hasharrayが満杯になった時、新しいメモリを確保。
            // その時、hasharrayの占有メモリ量が2倍になる。
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
    // 挿入
    _table->hasharray[adress] = index;
    return adress;
}

// クライアントテーブル内に該当のウインドウをBOXにもつクライアントはあるか？
size_t mtwm_client_table_window_exists(mtwm_client_table * _table, Window _target){

    // ハッシュ関数
    size_t adress = _target % _table->hashcode_max;

    // 対象のWindowが発見されるか、リスト構造の末端に行き着くまで、code_forwardの指すアドレスをたどり続ける。
    while(1){

        // 見つかった時
        if(_table->hasharray[adress].client.window[MTWM_CLIENT_BOX] == _target){
            if(_table->hasharray[adress].exists == True)
                return adress;
            else
                return __SIZE_MAX__;
        }

        // リストの末端にたどり着いない場合、再ループ
        // たどり着いた場合、-1を返す
        if(_table->hasharray[adress].code_forward != __SIZE_MAX__)
            adress = _table->hasharray[adress].code_forward;
        else
            return __SIZE_MAX__;

    }
    return __SIZE_MAX__;
}

// 該当のウインドウをBOXにもつクライアントを探す
mtwm_client * mtwm_client_table_find(mtwm_client_table * _table, Window _target){
    int address = mtwm_client_table_window_exists(_table, _target);
    if(address != __SIZE_MAX__)
        return &_table->hasharray[address].client;
    else 
        return NULL;
}

// 該当のウインドウをBOXにもつクライアントを消す。
int mtwm_client_table_delete(mtwm_client_table * _table, Window _target){
    int address = mtwm_client_table_window_exists(_table, _target);
    if(address != __SIZE_MAX__){

        
        _table->hasharray[address].exists = False;
        
        return 1;
    }
    else
        return 0;
}


// テーブルのデフラグメントを行う。
int mtwm_client_table_defragment(mtwm_client_table * _table){

    size_t new_current_size = _table->hashcode_max;

    for(size_t i=_table->hashcode_max; i<_table->current_size; i++){

        if(_table->hasharray[i].exists == True && new_current_size != i){

            size_t forward = _table->hasharray[i].code_forward;
            if(forward != __SIZE_MAX__)
                _table->hasharray[forward].code_backward = new_current_size;

            size_t backward = _table->hasharray[i].code_backward;
            _table->hasharray[backward].code_forward = new_current_size;

            _table->hasharray[new_current_size] = _table->hasharray[i];

            new_current_size++;

            _table->hasharray[i].exists = False;
        }
    }

    if(_table->current_size == new_current_size) return 0;

    else{
        _table->current_size = new_current_size;
        return 1;
    }
}

// メモリ開放
int mtwm_client_table_free(mtwm_client_table * _table){

    free(_table->hasharray);

    return 1;
}

