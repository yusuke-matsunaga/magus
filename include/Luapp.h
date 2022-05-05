#ifndef LUAPP_H
#define LUAPP_H

/// @file Luapp.h
/// @brief Luapp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2020, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "magus.h"
#include <lua.hpp>


BEGIN_NAMESPACE_MAGUS

//////////////////////////////////////////////////////////////////////
/// @class Luapp Luapp.h "Luapp.h"
/// @brief lua の C++ ラッパクラス
/// @ingroup LuappGroup
///
/// <lua.h>, <luaL.h> で提供されている API を C++ 風にラップしたもの
///
/// オリジナルの C-API では lua_State という構造体を第1引数に取る
/// 関数群を提供している．
/// このクラス(Luapp)はその lua_State を C++ 的に拡張したもの．
/// ほとんどのAPIはLuappのメンバ関数である．
///
/// 以下は公式APIリファレンスの要約
///
/// Lua インタプリタでは Lua スクリプトと C/C++ 間の情報の受け渡しに
/// 仮想スタックを用いる．
/// Lua の仮想スタックは任意のデータ型を格納することができ，
/// スタックの上方向に成長する．
/// 実際には任意の位置の要素にアクセス可能である．
///
/// n個の要素を持つスタックの場合，底の要素のインデックスは1，
/// 先頭のインデックスはnとなる．
/// インデックスが負の場合，先頭からの相対位置を表す．
/// -1 が先頭，-n が底を表す．
/// 負のインデックスを正のインデックスに変換するには absindex() を用いる．
///
/// スタックに要素を追加するときに十分なスタックサイズが用意されているか
/// 注意することはユーザーの責任となる．
/// 最小のスタックサイズはマクロ定数 LUA_MINSTACK で定義されており，
/// 通常は 20 である．
/// スタックサイズのチェックと確保には chesk_stack() を用いる．
///
/// 通常のインデックス以外に擬似インデックスと呼ばれるインデックス値がある．
/// これはレジストリと upvalue の参照のために用いられる．
/// これらの値は実際にはスタック上には存在しない．
///
/// Cタイプの文字列ポインタ(char*)を値として持つ場合，
/// その値が Lua のスタックに存在する間はポインタで指されている文字列領域
/// が存在していることが保障される．
/// スタックから取り除かれた場合には保障されない．
///
/// C言語の関数を呼び出す際に「クロージャ」を持たせることができる．
/// クロージャは Lua スタックの upvalue として実装される．
/// Lua 上で C++ のクラスのような動作を行いたい場合にはこのクロージャに
/// オブジェクト固有の値を持たせて関数を呼び出せば，
/// それがあたかもメンバ関数の様な働きをさせることができる．
///
/// レジストリを使うことで固有のテーブルを表すことが出来る．
/// ただしレジストリのキーが他のモジュールと衝突しないように注意する必要がある．
/// よくある使い方は light userdata を用いるものである．
/// 詳細は Lua の公式マニュアルなどを参照のこと．
//////////////////////////////////////////////////////////////////////
class Luapp
{
public:

  /// @brief get_XXX の返す型
  enum RetType {
    OK,       ///< 正常終了
    ERROR,    ///< エラー
    NOT_FOUND ///< 見つからなかった．
  };


public:
  //////////////////////////////////////////////////////////////////////
  /// @name コンストラクタ/デストラクタ
  /// @{

  /// @brief 空のコンストラクタ
  ///
  /// デフォルトの状態で初期化される．<br>
  /// このオブジェクトとともに Lua インタプリタも破壊される．
  Luapp(
  ) : mOwner{true},
      mState{luaL_newstate()}
  {
  }

  /// @brief 詳細なパラメータを指定したコンストラクタ
  ///
  /// このオブジェクトとともに Lua インタプリタも破壊される．
  Luapp(
    lua_Alloc f, ///< [in] メモリアロケータ
    void* ud     ///< [in] ユーザーデータ
  ) : mOwner{true},
      mState{lua_newstate(f, ud)}
  {
  }

  /// @brief すでに生成済みのインタプリタを用いるコンストラクタ
  ///
  /// このオブジェクトが破壊されても Lua インタプリタは破壊されない．
  Luapp(
    lua_State* L ///< [in] lua インタプリタ
  ) : mOwner{false},
      mState{L}
  {
  }

  /// @brief デストラクタ
  ///
  /// 自前で Lua インタプリタを生成した場合には破壊される．
  ~Luapp()
  {
    if ( mOwner ) {
      lua_close(mState);
    }
  }

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name basic stack manipulation
  /// @{
  //////////////////////////////////////////////////////////////////////

  /// @brief インデックスを絶対値に直す．
  ///
  /// idx は acceptance index である必要がある．<br>
  /// acceptance index に関しては公式マニュアルを参照のこと．
  int
  absindex(
    int idx ///< [in] インデックス
  )
  {
    return lua_absindex(mState, idx);
  }

  /// @brief スタックの先頭のインデックスを返す．
  ///
  /// この関数はスタックサイズを取得するためにも使われる．
  int
  get_top()
  {
    return lua_gettop(mState);
  }

  /// @brief スタックの先頭のインデックスを設定する．
  ///
  /// 実際のスタックサイズよりも大きなインデックスを設定した場合，
  /// 挿入される要素は nil で初期化される．
  ///
  /// 実際のスタックサイズよりも小さなインデックスを設定した場合，
  /// インデックスを越えた要素はスタックから取り除かれる．
  ///
  /// 0を設定した場合には全ての要素が取り除かれる．
  ///
  /// 負の値を指定した場合には先頭からの位置を表す．
  /// 現在の先頭の要素を一つポップする場合には set_top(-2)
  /// となるので注意すること．
  /// 実際には pop() を用いた方がわかりやすい．
  void
  set_top(
    int idx ///< [in] インデックス
  )
  {
    lua_settop(mState, idx);
  }

  /// @brief スタックから要素を取り出す．
  ///
  /// 取り出された要素は捨てられる．
  void
  pop(
    int n ///< [in] 取り出す要素数
  )
  {
    lua_pop(mState, n);
  }

  /// @brief スタックに要素をプッシュする．
  ///
  /// idx で指定された要素のコピーをスタックの先頭にプッシュする．
  void
  push_value(
    int idx ///< [in] プッシュする要素のインデックス
  )
  {
    lua_pushvalue(mState, idx);
  }

  /// @brief スタックの様を回転させる．
  ///
  /// idx で指示される位置から先頭までの要素を n だけずらす．
  /// 実際には rotate なのではみ出た要素は反対側に移動する．<br>
  /// n が正の場合にはスタックの先頭の方向に移動する．<br>
  /// n が負の場合にはスタックの底の方向に移動する．
  void
  rotate(
    int idx, ///< [in] 対象のインデックス
    int n    ///< [in] ずらす要素数
  )
  {
    lua_rotate(mState, idx, n);
  }

  /// @brief スタック上の要素をコピーする．
  ///
  /// コピー元の要素は影響を受けない．スタックサイズも変化しない．
  void
  copy(
    int fromidx, ///< [in] コピー元のインデックス
    int toidx    ///< [in] コピー先のインデックス
  )
  {
    lua_copy(mState, fromidx, toidx);
  }

  /// @brief スタックに要素を追加する．
  ///
  /// 現在の先頭の要素を idx の位置に移動し，
  /// idx より上の要素を一つずつ上に移動する．
  ///
  /// スタックサイズは変化しない．
  /// プッシュとは動作が異なることに注意．
  void
  insert(
    int idx ///< [in] インデックス
  )
  {
    lua_insert(mState, idx);
  }

  /// @brief スタックから要素を取り除く
  ///
  /// idx の位置の要素を取り除き，
  /// idx より上の要素を一つずつ下に移動する．
  void
  remove(
    int idx ///< [in] インデックス
  )
  {
    lua_remove(mState, idx);
  }

  /// @brief スタックの先頭の要素を移動する．
  ///
  /// 先頭の要素を idx に移動する．
  /// idx の元の値は取り除かれる．
  /// スタックサイズは1つ減少する．
  void
  replace(
    int idx ///< [in] インデックス
  )
  {
    lua_replace(mState, idx);
  }

  /// @brief スタックサイズを調べる．
  /// @return 十分なサイズがあれば true を返す．
  ///
  /// 現在の先頭からさらに n 個要素を挿入できるか調べる．
  bool
  check_stack(
    int n ///< [in] 必要な要素数
  )
  {
    return lua_checkstack(mState, n);
  }

  /// @brief 他のオブジェクトに値を移動する．
  ///
  /// 異なるスレッド間で要素の移動を行なう．
  /// 先頭から n 個の要素をポップし，to にプッシュする．
  void
  xmove(
    Luapp& to, ///< [in] 移動先のオブジェクト
    int n      ///< [in] 移動する要素数
  )
  {
    lua_xmove(mState, to.mState, n);
  }

  /// @}
  //////////////////////////////////////////////////////////////////////

  /// @brief upvalue のインデックスを得る．
  static
  int
  upvalueindex(
    int idx ///< [in] インデックス
  )
  {
    return lua_upvalueindex(idx);
  }


public:
  //////////////////////////////////////////////////////////////////////
  /// @name access functions (stack -> C)
  /// @{
  //////////////////////////////////////////////////////////////////////

  /// @brief 対象が浮動小数点数のとき true を返す．
  bool
  is_number(
    int idx ///< [in] インデックス
  )
  {
    return lua_isnumber(mState, idx);
  }

  /// @brief 対象が文字列のとき true を返す．
  bool
  is_string(
    int idx ///< [in] インデックス
  )
  {
    return lua_isstring(mState, idx);
  }

  /// @brief 対象がCの関数のとき true を返す．
  bool
  is_cfuction(
    int idx ///< [in] インデックス
  )
  {
    return lua_iscfunction(mState, idx);
  }

  /// @brief 対象が整数のとき true を返す．
  bool
  is_integer(
    int idx ///< [in] インデックス
  )
  {
    return lua_isinteger(mState, idx);
  }

  /// @brief 対象がユーザーデータのとき true を返す．
  bool
  is_userdata(
    int idx ///< [in] インデックス
  )
  {
    return lua_isuserdata(mState, idx);
  }

  /// @brief 対象がluaの関数のとき true を返す．
  bool
  is_function(
    int idx ///< [in] インデックス
  )
  {
    return lua_isfunction(mState, idx);
  }

  /// @brief 対象がluaのテーブルのとき true を返す．
  bool
  is_table(
    int idx ///< [in] インデックス
  )
  {
    return lua_istable(mState, idx);
  }

  /// @brief 対象がlightuserdataのとき true を返す．
  bool
  is_lightuserdata(
    int idx ///< [in] インデックス
  )
  {
    return lua_islightuserdata(mState, idx);
  }

  /// @brief 対象がluaのnilのとき true を返す．
  bool
  is_nil(
    int idx ///< [in] インデックス
  )
  {
    return lua_isnil(mState, idx);
  }

  /// @brief 対象がブール値のとき true を返す．
  bool
  is_boolean(
    int idx ///< [in] インデックス
  )
  {
    return lua_isboolean(mState, idx);
  }

  /// @brief 対象がluaのスレッドのとき true を返す．
  bool
  is_thread(
    int idx ///< [in] インデックス
  )
  {
    return lua_isthread(mState, idx);
  }

  /// @brief 対象のインデックスが範囲外のとき true を返す．
  bool
  is_none(
    int idx ///< [in] インデックス
  )
  {
    return lua_isnone(mState, idx);
  }

  /// @brief 対象が範囲外かnilのとき true を返す．
  bool
  is_none_or_nil(
    int idx ///< [in] インデックス
  )
  {
    return lua_isnoneornil(mState, idx);
  }

  /// @brief 対象の型を表す値を返す．
  int
  type(
    int idx ///< [in] インデックス
  )
  {
    return lua_type(mState, idx);
  }

  /// @brief 型を表す文字列を返す．
  const char*
  type_name(
    int tp ///< [in] 型を表す値
  )
  {
    return lua_typename(mState, tp);
  }

  /// @brief 長さを返す．
  /// @return 対象の長さを返す．
  ///
  /// 長さの定義は対象の型によって異なる．
  /// - 文字列: 文字列の長さ
  /// - テーブル: テーブルの要素数
  /// - ユーザーデータ: サイズ(バイト数)
  /// - その他: 0
  size_t
  rawlen(
    int idx ///< [in] インデックス
  )
  {
    return lua_rawlen(mState, idx);
  }

  /// @brief 対象を浮動小数点数に変換する．
  /// @retval true, 変換結果 変換可能の場合
  /// @retval false, 0 変換できない場合
  tuple<bool, lua_Number>
  to_number(
    int idx ///< [in] インデックス
  )
  {
    int isnum;
    lua_Number ans = lua_tonumberx(mState, idx, &isnum);
    return {static_cast<bool>(isnum), ans};
  }

  /// @brief 対象を整数に変換する．
  /// @retval true, 変換結果 変換可能の場合
  /// @retval false, 0 変換できない場合
  tuple<bool, lua_Integer>
  to_integer(
    int idx ///< [in] インデックス
  )
  {
    int isnum;
    lua_Integer ans = lua_tointegerx(mState, idx, &isnum);
    return {static_cast<bool>(isnum), ans};
  }

  /// @brief 対象をブール値に変換する．
  ///
  /// この変換は失敗しない．
  bool
  to_boolean(
    int idx ///< [in] インデックス
  )
  {
    return lua_toboolean(mState, idx);
  }

  /// @brief 対象をC文字列に変換する．
  const char*
  to_string(
    int idx ///< [in] インデックス
  )
  {
    return lua_tostring(mState, idx);
  }

  /// @brief 対象を長さ付き文字列に変換する．
  tuple<size_t, const char*>
  to_lstring(
    int idx ///< [in] インデックス
  )
  {
    size_t len;
    const char* ans = lua_tolstring(mState, idx, &len);
    return {len, ans};
  }

  /// @brief 対象をCの関数に変換する．
  ///
  /// 変換できない時は nullptr を返す．
  lua_CFunction
  to_cfunction(
    int idx ///< [in] インデックス
  )
  {
    return lua_tocfunction(mState, idx);
  }

  /// @brief 対象をユーザーデータに変換する．
  ///
  /// 変換できない時は nullptr を返す．
  void*
  to_userdata(
    int idx ///< [in] インデックス
  )
  {
    return lua_touserdata(mState, idx);
  }

  /// @brief 対象をluaのスレッドに変換する．
  ///
  /// 変換できない時は nullptr を返す．
  lua_State*
  to_thread(
    int idx ///< [in] インデックス
  )
  {
    return lua_tothread(mState, idx);
  }

  /// @brief 対象をCのポインタに変換する．
  ///
  /// 変換できない時は nullptr を返す．
  const void*
  to_pointer(
    int idx ///< [in] インデックス
  )
  {
    return lua_topointer(mState, idx);
  }

  /// @}
  //////////////////////////////////////////////////////////////////////

public:
  //////////////////////////////////////////////////////////////////////
  /// @name comparison and arithmetic functions
  /// @{
  //////////////////////////////////////////////////////////////////////

  /// @brief 数値演算を行う．
  ///
  /// 引数はスタックの先頭から取り出され，結果はスタックに追加される．
  /// op の値は以下の通り
  /// - LUA_OPADD: performs addition (+)
  /// - LUA_OPSUB: performs subtraction (-)
  /// - LUA_OPMUL: performs multiplication (*)
  /// - LUA_OPDIV: performs float division (/)
  /// - LUA_OPIDIV: performs floor division (//)
  /// - LUA_OPMOD: performs modulo (%)
  /// - LUA_OPPOW: performs exponentiation (^)
  /// - LUA_OPUNM: performs mathematical negation (unary -)
  /// - LUA_OPBNOT: performs bitwise NOT (~)
  /// - LUA_OPBAND: performs bitwise AND (&)
  /// - LUA_OPBOR: performs bitwise OR (|)
  /// - LUA_OPBXOR: performs bitwise exclusive OR (~)
  /// - LUA_OPSHL: performs left shift (<<)
  /// - LUA_OPSHR: performs right shift (>>)
  void
  arith(
    int op ///< [in] 演算コード
  )
  {
    return lua_arith(mState, op);
  }

  /// @brief 2つの値の単純な等価比較を行う．
  ///
  /// どちらか一方でも範囲外の場合は false を返す．
  bool
  rawequal(
    int idx1, ///< [in] 第1オペランドのインデックス
    int idx2  ///< [in] 第2オペランドのインデックス
  )
  {
    return lua_rawequal(mState, idx1, idx2);
  }

  /// @brief 2つの値の比較を行う．
  ///
  /// op の値は以下の通り．
  /// - LUA_OPEQ: compares for equality (==)
  /// - LUA_OPLT: compares for less than (<)
  /// - LUA_OPLE: compares for less or equal (<=)
  /// この関係が成り立った時 true を返す．
  /// それ以外，およびインデックスが範囲外の時は false を返す．
  bool
  compare(
    int idx1, ///< [in] 第1オペランドのインデックス
    int idx2, ///< [in] 第2オペランドのインデックス
    int op    ///< [in] 演算コード
  )
  {
    return lua_compare(mState, idx1, idx2, op);
  }

  /// @}
  //////////////////////////////////////////////////////////////////////

public:
  //////////////////////////////////////////////////////////////////////
  /// @name push functions (C -> stack)
  /// @{
  //////////////////////////////////////////////////////////////////////

  /// @brief スタックにluaのnilをプッシュする．
  void
  push_nil()
  {
    lua_pushnil(mState);
  }

  /// @brief スタックに数値をプッシュする．
  void
  push_number(
    lua_Number n ///< [in] 数値
  )
  {
    lua_pushnumber(mState, n);
  }

  /// @brief スタックに整数値をプッシュする．
  void
  push_integer(
    lua_Integer n ///< [in] 数値
  )
  {
    lua_pushinteger(mState, n);
  }

  /// @brief スタックに長さ付き文字列をプッシュする．
  const char*
  push_lstring(
    const char* s, ///< [in] 文字列
    size_t len     ///< [in] 長さ
  )
  {
    return lua_pushlstring(mState, s, len);
  }

  /// @brief スタックににC文字列をプッシュする．
  const char*
  push_string(
    const char* s ///< [in] C文字列('\0'で終端する)
  )
  {
    return lua_pushstring(mState, s);
  }

  /// @brief スタックにCの関数をプッシュする．
  void
  push_cfunction(
    lua_CFunction fn ///< [in] C関数
  )
  {
    lua_pushcfunction(mState, fn);
  }

  /// @brief スタックにCのクロージャをプッシュする．
  ///
  /// クロージャの変数の値はスタックの現在のトップの上に n 個入っている．
  void
  push_cclosure(
    lua_CFunction fn, ///< [in] C関数
    int n             ///< [in] クロージャに含まれる関数の数
  )
  {
    lua_pushcclosure(mState, fn, n);
  }

  /// @brief スタックにブール値をプッシュする．
  void
  push_boolean(
    bool b ///< [in] ブール値
  )
  {
    lua_pushboolean(mState, b);
  }

  /// @brief スタックに light userdata をプッシュする．
  void
  push_lightuserdata(
    void* p ///< [in] データ
  )
  {
    lua_pushlightuserdata(mState, p);
  }

  /// @brief スタックにluaのスレッドをプッシュする．
  int
  push_thread()
  {
    return lua_pushthread(mState);
  }

  /// @}
  //////////////////////////////////////////////////////////////////////

public:
  //////////////////////////////////////////////////////////////////////
  /// @name get functions (Lua -> stack)
  /// @{
  //////////////////////////////////////////////////////////////////////

  /// @brief グローバル変数をスタックに積む．
  /// @return 変数の型
  int
  get_global(
    const char* name ///< [in] 変数名
  )
  {
    return lua_getglobal(mState, name);
  }

  /// @brief テーブルの値をスタックに積む．
  /// @return 取り出された値の型
  ///
  /// 対象のインデックスはスタックの先頭から取り出される．
  int
  get_table(
    int idx ///< [in] テーブルを指すインデックス
  )
  {
    return lua_gettable(mState, idx);
  }

  /// @brief フィールドの値をスタックに積む．
  /// @return 取り出された値の型
  int
  get_field(
    int idx,      ///< [in] テーブルを指すインデックス
    const char* k ///< [in] フィールド名
  )
  {
    return lua_getfield(mState, idx, k);
  }

  /// @brief テーブルのi番目の要素をスタックに積む．
  /// @return 取り出された値の型
  int
  get_table(
    int idx,      ///< [in] テーブルを指すインデックス
    lua_Integer i ///< [in] 対象のインデックス
  )
  {
    return lua_geti(mState, idx, i);
  }

  /// @brief テーブルの値をスタックに積む．
  /// @return 取り出された値の型
  ///
  /// 対象のインデックスはスタックの先頭から取り出される．<br>
  /// get_table() と似ているがこちらは metamethod を呼ばない．
  int
  rawget(
    int idx ///< [in] テーブルを指すインデックス
  )
  {
    return lua_rawget(mState, idx);
  }

  /// @brief テーブルのi番目の要素をスタックに積む．
  /// @return 取り出された値の型
  ///
  /// geti() と似ているがこちらは metamethod を呼ばない．
  int
  rawget(
    int idx,      ///< [in] テーブルを指すインデックス
    lua_Integer n ///< [in] 対象のインデックス
  )
  {
    return lua_rawgeti(mState, idx, n);
  }

  /// @brief テーブルの値をスタックに積む．
  /// @return 取り出された値の型
  int
  rawget(
    int idx,      ///< [in] テーブルを指すインデックス
    const void* p ///< [in] キーを表す light userdata
  )
  {
    return lua_rawgetp(mState, idx, p);
  }

  /// @brief 新しいテーブルを作る．
  ///
  /// 生成されたテーブルはスタックにプッシュされる．
  void
  create_table(
    int narr = 0, ///< [in] 配列要素数のヒント
    int nrec = 0  ///< [in] テーブル要素数のヒント
  )
  {
    lua_createtable(mState, narr, nrec);
  }

  /// @brief ユーザーデータを作る．
  ///
  /// 生成されたユーザーデータはスタックにプッシュされる．
  void*
  new_userdata(
    size_t sz ///< [in] ユーザーデータのサイズ
  )
  {
    return lua_newuserdata(mState, sz);
  }

  /// @brief メタテーブルを取り出しスタックに積む．
  /// @retval true 対象がメタテーブルを持っていた．
  /// @retval false 対象がメタテーブルを持っていなかった．
  ///
  /// 失敗した場合にはスタックは変化しない．
  bool
  get_metatable(
    int objindex ///< [in] 対象のインデックス
  )
  {
    return lua_getmetatable(mState, objindex);
  }

  /// @brief ユーザーデータの値をスタックに積む．
  /// @return 取り出された値の型
  int
  get_uservalue(
    int idx ///< [in] 対象のインデックス
  )
  {
    return lua_getuservalue(mState, idx);
  }

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name set functions (stack -> Lua)e
  /// @{
  //////////////////////////////////////////////////////////////////////

  /// @brief グローバル変数に値をセットする．
  ///
  /// スタックから値を取り出してセットする．
  void
  set_global(
    const char* name ///< [in] 変数名
  )
  {
    lua_setglobal(mState, name);
  }

  /// @brief テーブルに値をセットする．
  ///
  /// 設定する値はスタックの先頭に，
  /// テーブル上のインデックスはその次に入っているものとする．
  /// これら2つの要素はポップされる．
  void
  set_table(
    int idx ///< [in] テーブルを指すインデックス
  )
  {
    lua_settable(mState, idx);
  }

  /// @brief フィールドに値をセットする．
  ///
  /// 設定する値はスタックの先頭に入っているものとする．
  /// この要素はポップされる．
  void
  set_field(
    int idx,      ///< [in] テーブルを指すインデックス
    const char* k ///< [in] フィールド名
  )
  {
    lua_setfield(mState, idx, k);
  }

  /// @brief テーブルに値をセットする．
  ///
  /// 設定する値はスタックの先頭に入っているものとする．
  /// この要素はポップされる．
  void
  set_table(
    int idx,      ///< [in] テーブルを指すインデックス
    lua_Integer n ///< [in] テーブル上のインデックス
  )
  {
    lua_seti(mState, idx, n);
  }

  /// @brief テーブルに値をセットする．
  ///
  /// 設定する値はスタックの先頭に，
  /// テーブル上のインデックスはその次に入っているものとする．
  /// これら2つの要素はポップされる．
  /// set_table()との違いは __index metamethod を呼ばないこと．
  void
  rawset(
    int idx ///< [in] テーブルを指すインデックス
  )
  {
    lua_rawset(mState, idx);
  }

  /// @brief テーブルに値をセットする．
  ///
  /// 設定する値はスタックの先頭に入っているものとする．<br>
  /// この要素はポップされる．<br>
  /// set_table()との違いは __index metamethod を呼ばないこと．
  void
  rawset(
    int idx,      ///< [in] テーブルを指すインデックス
    lua_Integer n ///< [in] テーブル上のインデックス
  )
  {
    lua_rawseti(mState, idx, n);
  }

  /// @brief フィールドに値をセットする．
  ///
  /// 設定する値はスタックの先頭に入っているものとする．<br>
  /// この要素はポップされる．
  void
  rawset(
    int idx,      ///< [in] テーブルを指すインデックス
    const void* p ///< [in] フィールドを表す light userdata
  )
  {
    lua_rawsetp(mState, idx, p);
  }

  /// @brief メタテーブルをセットする．
  ///
  /// メタテーブルはスタックの先頭に入っているものとする．<br>
  /// この要素はポップされる．
  void
  set_metatable(
    int objindex ///< [in] 対象のインデックス
  )
  {
    lua_setmetatable(mState, objindex);
  }

  /// @brief ユーザーデータの値をセットする．
  ///
  /// 設定する値はスタックの先頭に入っているものとする．<br>
  /// この要素はポップされる．
  void
  set_uservalue(
    int idx ///< [in] 対象のインデックス
  )
  {
    lua_setuservalue(mState, idx);
  }

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name 'load' and 'call' functions
  /// @{
  //////////////////////////////////////////////////////////////////////

  /// @brief 関数呼び出しを行う．
  ///
  /// スタックには，関数，引数1，...，引数nargs
  /// の順で値が積まれているものとする．<br>
  /// 結果はスタックに積まれる．
  void
  call(
    int nargs,   ///< [in] 引数の数
    int nresults ///< [in] 結果の要素数の数
  )
  {
    lua_call(mState, nargs, nresults);
  }

  /// @brief 関数呼び出しを行う(yield版)．
  ///
  /// スタックには，関数，引数1，...，引数nargs
  /// の順で値が積まれているものとする．<br>
  /// 結果はスタックに積まれる．
  void
  call(
    int nargs,        ///< [in] 引数の数
    int nresults,     ///< [in] 結果の要素数の数
    lua_KContext ctx, ///< [in] コンテキスト
    lua_KFunction k   ///< [in] continuation function
  )
  {
    lua_callk(mState, nargs, nresults, ctx, k);
  }

  /// @brief 保護モードで関数呼び出しを行う．
  /// @return エラーコード
  ///
  /// スタックには，関数，引数1，...，引数nargs
  /// の順で値が積まれているものとする．<br>
  /// 結果はスタックに積まれる．<br>
  /// エラーコードは以下の通り．
  /// - LUA_OK (0): success.
  /// - LUA_ERRRUN: a runtime error.
  /// - LUA_ERRMEM: memory allocation error. For such errors, Lua does not call the message handler.
  /// - LUA_ERRERR: error while running the message handler.
  /// - LUA_ERRGCMM: error while running a __gc metamethod. For such errors, Lua does not call the message handler (as this kind of error typically has no relation with the function being called).
  int
  pcall(
    int nargs,    ///< [in] 引数の数
    int nresults, ///< [in] 結果の要素数の数
    int errfunc   ///< [in] エラー関数のインデックス
  )
  {
    return lua_pcall(mState, nargs, nresults, errfunc);
  }

  /// @brief 保護モードで関数呼び出しを行う(yield版)．
  /// @return エラーコード
  ///
  /// スタックには，関数，引数1，...，引数nargs
  /// の順で値が積まれているものとする．<br>
  /// 結果はスタックに積まれる．<br>
  /// エラーコードは以下の通り．
  /// - LUA_OK (0): success.
  /// - LUA_ERRRUN: a runtime error.
  /// - LUA_ERRMEM: memory allocation error. For such errors, Lua does not call the message handler.
  /// - LUA_ERRERR: error while running the message handler.
  /// - LUA_ERRGCMM: error while running a __gc metamethod. For such errors, Lua does not call the message handler (as this kind of error typically has no relation with the function being called).
  int
  pcall(
    int nargs,        ///< [in] 引数の数
    int nresults,     ///< [in] 結果の要素数の数
    int errfunc,      ///< [in] エラー関数のインデックス
    lua_KContext ctx, ///< [in] コンテキスト
    lua_KFunction k   ///< [in] continuation function
  )
  {
    return lua_pcallk(mState, nargs, nresults, errfunc, ctx, k);
  }

  /// @brief ロードを行う．
  /// @retval LUA_OK: no errors;
  /// @retval LUA_ERRSYNTAX: syntax error during precompilation;
  /// @retval LUA_ERRMEM: memory allocation (out-of-memory) error;
  /// @retval LUA_ERRGCMM: error while running a __gc metamethod. (This error has no relation with the chunk being loaded. It is generated by the garbage collector.)
  ///
  /// 読み出されたチャンクはスタックに積まれる．
  int
  load(
    lua_Reader reader,     ///< [in] 読み込み関数
    void* data,            ///< [in] reader に渡されるデータ
    const char* chunkname, ///< [in] チャンク名
    const char* mode       ///< [in] モード
  )
  {
    return lua_load(mState, reader, data, chunkname, mode);
  }

  /// ダンプを行う．
  int
  dump(
    lua_Writer writer, ///< [in] 書き出し関数
    void* data,        ///< [in] writer に渡されるデータ
    bool strip         ///< [in] ストリップフラグ
  )
  {
    return lua_dump(mState, writer, data, strip);
  }

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name corutine functionse
  /// @{
  //////////////////////////////////////////////////////////////////////

  /// @brief ???
  int
  yield(
    int nresults,
    lua_KContext ctx,
    lua_KFunction k
  )
  {
    return lua_yieldk(mState, nresults, ctx, k);
  }

  int
  resume(
    Luapp& from,
    int narg,
    int* nresults
  )
  {
    return lua_resume(mState, from.mState, narg, nresults);
  }

  int
  status()
  {
    return lua_status(mState);
  }

  bool
  is_yieldable()
  {
    return lua_isyieldable(mState);
  }

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name garbage-collection function and options
  /// @{
  //////////////////////////////////////////////////////////////////////

  int
  gc(
    int what,
    int data
  )
  {
    return lua_gc(mState, what, data);
  }

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name miscellaneous function
  /// @{
  //////////////////////////////////////////////////////////////////////

  int
  error()
  {
    return lua_error(mState);
  }

  int
  next(
    int idx
  )
  {
    return lua_next(mState, idx);
  }

  void
  concat(
    int n
  )
  {
    lua_concat(mState, n);
  }

  void
  len(
    int idx
  )
  {
    lua_len(mState, idx);
  }

  size_t
  string_to_number(
    const char* s
  )
  {
    return lua_stringtonumber(mState, s);
  }

  tuple<lua_Alloc, void*>
  get_allocf()
  {
    void* ud;
    lua_Alloc af = lua_getallocf(mState, &ud);
    return {af, ud};
  }

  void
  set_allocf(
    lua_Alloc f,
    void* ud
  )
  {
    lua_setallocf(mState, f, ud);
  }

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name lualib, lauxlib の関数
  /// @{
  //////////////////////////////////////////////////////////////////////

  void
  L_openlibs()
  {
    luaL_openlibs(mState);
  }

  void
  L_checkversion_(
    lua_Number ver,
    size_t sz
  )
  {
    luaL_checkversion_(mState, ver, sz);
  }

  void
  L_checkversion()
  {
    luaL_checkversion(mState);
  }

  int
  L_getmetafield(
    int obj,
    const char *e
  )
  {
    return luaL_getmetafield(mState, obj, e);
  }

  int
  L_callmeta(
    int obj,
    const char *e
  )
  {
    return luaL_callmeta(mState, obj, e);
  }

  const char*
  L_tolstring(
    int idx,
    size_t *len
  )
  {
    return luaL_tolstring(mState, idx, len);
  }

  int
  L_argerror(
    int arg,
    const char *extramsg
  )
  {
    return luaL_argerror(mState, arg, extramsg);
  }

  const char*
  L_checklstring(
    int arg,
    size_t *l
  )
  {
    return luaL_checklstring(mState, arg, l);
  }

  const char*
  L_optlstring(
    int arg,
    const char *def,
    size_t *l
  )
  {
    return luaL_optlstring(mState, arg, def, l);
  }

  lua_Number
  L_checknumber(
    int arg
  )
  {
    return luaL_checknumber(mState, arg);
  }

  lua_Number
  L_optnumber(
    int arg,
    lua_Number def
  )
  {
    return luaL_optnumber(mState, arg, def);
  }

  lua_Integer
  L_checkinteger(
    int arg
  )
  {
    return luaL_checkinteger(mState, arg);
  }

  lua_Integer
  L_optinteger(
    int arg,
    lua_Integer def
  )
  {
    return luaL_optinteger(mState, arg, def);
  }

  void
  L_checkstack(
    int sz,
    const char *msg
  )
  {
    luaL_checkstack(mState, sz, msg);
  }

  void
  L_checktype(
    int arg,
    int t
  )
  {
    luaL_checktype(mState, arg, t);
  }

  void
  L_checkany(
    int arg
  )
  {
    luaL_checkany(mState, arg);
  }

  int
  L_newmetatable(
    const char *tname
  )
  {
    return luaL_newmetatable(mState, tname);
  }

  void
  L_setmetatable(
    const char *tname
  )
  {
    luaL_setmetatable(mState, tname);
  }

  void*
  L_testudata(
    int ud,
    const char *tname
  )
  {
    return luaL_testudata(mState, ud, tname);
  }

  void*
  L_checkudata(
    int ud,
    const char *tname
  )
  {
    return luaL_checkudata(mState, ud, tname);
  }

  void
  L_where(
    int lvl
  )
  {
    luaL_where(mState, lvl);
  }

  int
  L_error(
    const char *fmt,
    ...
  );

  int
  L_checkoption(
    int arg,
    const char *def,
    const char *const lst[]
  )
  {
    return luaL_checkoption(mState, arg, def, lst);
  }

  int
  L_fileresult(
    int stat,
    const char *fname
  )
  {
    return luaL_fileresult(mState, stat, fname);
  }

  int
  L_execresult(
    int stat
  )
  {
    return luaL_execresult(mState, stat);
  }

  int
  L_ref(
    int t
  )
  {
    return luaL_ref(mState, t);
  }

  void
  L_unref(
    int t,
    int ref
  )
  {
    luaL_unref(mState, t, ref);
  }

  int
  L_loadfilex(
    const char *filename,
    const char *mode
  )
  {
    return luaL_loadfilex(mState, filename, mode);
  }

  int
  L_loadfile(
    const char* filename
  )
  {
    return luaL_loadfile(mState, filename);
  }

  int
  L_loadbufferx(
    const char *buff,
    size_t sz,
    const char *name,
    const char *mode
  )
  {
    return luaL_loadbufferx(mState, buff, sz, name, mode);
  }

  int
  L_loadstring(
    const char *s
  )
  {
    return luaL_loadstring(mState, s);
  }

  lua_Integer
  L_len(
    int idx
  )
  {
    return luaL_len(mState, idx);
  }

  const char*
  L_gsub(
    const char *s,
    const char *p,
    const char *r
  )
  {
    return luaL_gsub(mState, s, p, r);
  }

  void
  L_setfuncs(
    const luaL_Reg *l,
    int nup
  )
  {
    luaL_setfuncs(mState, l, nup);
  }

  int
  L_getsubtable(
    int idx,
    const char *fname
  )
  {
    return luaL_getsubtable(mState, idx, fname);
  }

  void
  L_traceback(
    Luapp& l1,
    const char *msg,
    int level
  )
  {
    luaL_traceback(mState, l1.mState, msg, level);
  }

  void
  L_requiref(
    const char *modname,
    lua_CFunction openf,
    int glb
  )
  {
    luaL_requiref(mState, modname, openf, glb);
  }

#if 0
  void
  L_newlibtable(
    const luaL_Reg* l
  )
  {
    // もとはマクロなので sizeof(l) で元の配列のサイズを得ることができるが
    // ポインタにしたらわからない．
    // インターフェイスを変更する必要がある．
    int nrec = static_cast<int>(sizeof(l) / sizeof(l[0]) - 1);
    create_table(0, nrec);
  }

  void
  L_newlib(
    const luaL_Reg* l
  )
  {
    L_checkversion();
    L_newlibtable(l);
    L_setfuncs(l, 0);
  }
#endif

  int
  L_dofile(
    const char* filename
  )
  {
    return luaL_dofile(mState, filename);
  }

  int
  L_dostring(
    const char* chunk
  )
  {
    return luaL_dostring(mState, chunk);
  }

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name スタック上のテーブルにアクセスする関数
  /// @{
  //////////////////////////////////////////////////////////////////////

  /// @brief 指定したフィールドを整数に変換する．
  /// @retval OK 成功した．
  /// @retval ERROR エラーが起こった．
  /// @retval NOT_FOUND 見つからなかった
  ///
  /// スタックは変化しない．
  RetType
  get_int_field(
    int index,       ///< [in]  テーブルのスタック上の位置
    const char* key, ///< [in]  フィールド名
    lua_Integer& val ///< [out] 変換結果を収める変数
  )
  {
    RetType ret = ERROR;
    int type = get_field(index, key);
    if ( type == LUA_TNIL ) {
      // 存在しなかった．
      ret = NOT_FOUND;
    }
    else {
      bool isnum;
      tie(isnum, val) = to_integer(-1);
      if ( isnum ) {
	ret = OK;
      }
      else {
	// 異なる型だった．
	ret = ERROR;
	cerr << "Error: '" << key << "' should be an integer." << endl;
      }
    }

    // get_field() で積まれた要素を捨てる．
    pop(1);

    return ret;
  }

  /// @brief 指定したフィールドをブール値に変換する．
  /// @retval true 成功した．
  /// @retval false エラーが起こった．
  /// @retval OK 成功した．
  /// @retval ERROR エラーが起こった．
  /// @retval NOT_FOUND 見つからなかった
  ///
  /// スタックは変化しない．
  RetType
  get_boolean_field(
    int index,       ///< [in]  テーブルのスタック上の位置
    const char* key, ///< [in]  フィールド名
    bool& val        ///< [out] 変換結果を収める変数
  )
  {
    RetType ret = ERROR;
    int type = get_field(index, key);
    if ( type == LUA_TNIL ) {
      // 存在しなかった．
      ret = NOT_FOUND;
    }
    else {
      // 全ての型は boolean に変換可能
      val = to_boolean(-1);
      ret = OK;
    }

    // get_field() で積まれた要素を捨てる．
    pop(1);

    return ret;
  }

  /// @brief 指定したフィールドを浮動小数点に変換する．
  /// @retval OK 成功した．
  /// @retval ERROR エラーが起こった．
  /// @retval NOT_FOUND 見つからなかった
  ///
  /// スタックは変化しない．
  RetType
  get_float_field(
    int index,       ///< [in]  テーブルのスタック上の位置
    const char* key, ///< [in]  フィールド名
    lua_Number& val  ///< [out] 変換結果を収める変数
  )
  {
    RetType ret = ERROR;
    int type = get_field(index, key);
    if ( type == LUA_TNIL ) {
      // 存在しなかった．
      ret = NOT_FOUND;
    }
    else {
      bool isnum;
      tie(isnum, val) = to_number(-1);
      if ( isnum ) {
	ret = OK;
      }
      else {
	// 異なる型だった．
	ret = ERROR;
	cerr << "Error: '" << key << "' should be a float number." << endl;
      }
    }

    // get_field() で積まれた要素を捨てる．
    pop(1);

    return ret;
  }

  /// @brief 指定したフィールドを文字列に変換する．
  /// @retval OK 成功した．
  /// @retval ERROR エラーが起こった．
  /// @retval NOT_FOUND 見つからなかった
  ///
  /// スタックは変化しない．
  RetType
  get_string_field(
    int index,       ///< [in]  テーブルのスタック上の位置
    const char* key, ///< [in]  フィールド名
    string& val      ///< [out] 変換結果を収める変数
  )
  {
    RetType ret = ERROR;
    int type = get_field(index, key);
    if ( type == LUA_TNIL ) {
      // 存在しなかった．
      ret = NOT_FOUND;
    }
    else {
      if ( type == LUA_TSTRING ) {
	val = string(to_string(-1));
	ret = OK;
      }
      else {
	ret = ERROR;
	cerr << "Error: '" << key << "' should be a string." << endl;
      }
    }

    // get_field() で積まれた要素を捨てる．
    pop(1);

    return ret;
  }

  /// @brief 指定したフィールドを vector<int> に変換する．
  /// @retval OK 成功した．
  /// @retval ERROR エラーが起こった．
  /// @retval NOT_FOUND 見つからなかった
  ///
  /// スタックは変化しない．
  RetType
  get_int_list_field(
    int index,        ///< [in]  テーブルのスタック上の位置
    const char* key,  ///< [in]  フィールド名
    vector<int>& val  ///< [out] 変換結果を収める変数
  )
  {
    RetType ret = ERROR;
    int type = get_field(index, key);
    val.clear();
    if ( type == LUA_TNIL ) {
      // 存在しなかった．
      ret = NOT_FOUND;
    }
    else if ( type == LUA_TTABLE ) {
      int n = L_len(-1);
      ret = OK;
      for ( int i = 1; i <= n; ++ i ) {
	get_table(-1, i);
	bool isnum;
	int val1;
	tie(isnum, val1) = to_integer(-1);
	if ( isnum ) {
	  val.push_back(val1);
	}
	else {
	  cerr << "Error: illegal data in table '" << key << "'" << endl;
	  ret = ERROR;
	}
	// get_table() で積んだ値を捨てる．
	pop(1);
      }
    }
    else {
      // エラー
      cerr << "Error: illegal type in '" << key << "'" << endl;
    }

    // get_field() で積まれた要素を捨てる．
    pop(1);

    return ret;
  }

  /// @brief 指定したフィールドを vector<string> に変換する．
  /// @retval OK 成功した．
  /// @retval ERROR エラーが起こった．
  /// @retval NOT_FOUND 見つからなかった
  ///
  /// スタックは変化しない．
  RetType
  get_string_list_field(
    int index,          ///< [in]  テーブルのスタック上の位置
    const char* key,    ///< [in]  フィールド名
    vector<string>& val ///< [out] 変換結果を収める変数
  )
  {
    RetType ret = ERROR;
    int type = get_field(index, key);
    val.clear();
    if ( type == LUA_TNIL ) {
      // 存在しなかった．
      ret = NOT_FOUND;
    }
    else if ( type == LUA_TTABLE ) {
      int n = L_len(-1);
      ret = OK;
      for ( int i = 1; i <= n; ++ i ) {
	get_table(-1, i);
	string val1 = to_string(-1);
	val.push_back(val1);
	// get_table() で積んだ値を捨てる．
	pop(1);
      }
    }
    else {
      // エラー
      ret = ERROR;
      cerr << "Error: illegal type in '" << key << "'" << endl;
    }

    // get_field() で積まれた要素を捨てる．
    pop(1);

    return ret;
  }

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name スタック上のテーブルにアクセスする関数(その2)
  /// @{
  //////////////////////////////////////////////////////////////////////

  /// @brief 指定したフィールドを整数に変換してセッター関数を呼ぶ．
  /// @retval OK 成功した．
  /// @retval ERROR エラーが起こった．
  /// @retval NOT_FOUND 見つからなかった
  ///
  /// スタックは変化しない．
  RetType
  get_int_field(
    int index,                      ///< [in] テーブルのスタック上の位置
    const char* key,                ///< [in] フィールド名
    std::function<bool(int)> setter ///< [in] セッター関数
  )
  {
    RetType ret = ERROR;
    int type = get_field(index, key);
    if ( type == LUA_TNIL ) {
      // 存在しなかった．
      ret = NOT_FOUND;
    }
    else {
      bool isnum;
      lua_Integer tmp;
      tie(isnum, tmp) = to_integer(-1);
      if ( isnum ) {
	if ( setter(tmp) ) {
	  ret = OK;
	}
      }
      else {
	// 異なる型だった．
	cerr << "Error: '" << key << "' should be an integer." << endl;
      }
    }

    // get_field() で積まれた要素を捨てる．
    pop(1);

    return ret;
  }

  /// @brief 指定したフィールドをブール値に変換してセッター関数を呼ぶ．
  /// @retval OK 成功した．
  /// @retval ERROR エラーが起こった．
  /// @retval NOT_FOUND 見つからなかった
  ///
  /// スタックは変化しない．
  RetType
  get_boolean_field(
    int index,                       ///< [in] テーブルのスタック上の位置
    const char* key,                 ///< [in] フィールド名
    std::function<bool(bool)> setter ///< [in] セッター関数
  )
  {
    RetType ret = ERROR;
    int type = get_field(index, key);
    if ( type == LUA_TNIL ) {
      // 存在しなかった．
      ret = NOT_FOUND;
    }
    else {
      // 全ての型は boolean に変換可能
      if ( setter(to_boolean(-1)) ) {
	ret = OK;
      }
    }

    // get_field() で積まれた要素を捨てる．
    pop(1);

    return ret;
  }

  /// @brief 指定したフィールドを文字列に変換してセッター関数を呼ぶ．
  /// @retval OK 成功した．
  /// @retval ERROR エラーが起こった．
  /// @retval NOT_FOUND 見つからなかった
  ///
  /// スタックは変化しない．
  RetType
  get_string_field(
    int index,                                ///< [in] テーブルのスタック上の位置
    const char* key,                          ///< [in] フィールド名
    std::function<bool(const string&)> setter ///< [in] セッター関数
  )
  {
    RetType ret = ERROR;
    int type = get_field(index, key);
    if ( type == LUA_TNIL ) {
      // 存在しなかった．
      ret = NOT_FOUND;
    }
    else if ( type == LUA_TSTRING ) {
      if ( setter(string(to_string(-1))) ) {
	ret = OK;
      }
    }
    else {
      cerr << "Error: '" << key << "' should be a string." << endl;
    }

    // get_field() で積まれた要素を捨てる．
    pop(1);

    return ret;
  }

  /// @brief 指定したフィールドを vector<int> に変換してセッター関数を呼ぶ．
  /// @retval OK 成功した．
  /// @retval ERROR エラーが起こった．
  /// @retval NOT_FOUND 見つからなかった
  ///
  /// スタックは変化しない．
  RetType
  get_int_list_field(
    int index,                                     ///< [in] テーブルのスタック上の位置
    const char* key,                               ///< [in] フィールド名
    std::function<bool(const vector<int>&)> setter ///< [in] セッター関数
  )
  {
    RetType ret = ERROR;
    int type = get_field(index, key);
    if ( type == LUA_TNIL ) {
      // 存在しなかった．
      ret = NOT_FOUND;
    }
    else if ( type == LUA_TTABLE ) {
      int n = L_len(-1);
      vector<int> val;
      val.reserve(n);
      bool break_flag = false;
      for ( int i = 1; i <= n; ++ i ) {
	get_table(-1, i);
	bool isnum;
	int val1;
	tie(isnum, val1) = to_integer(-1);
	if ( isnum ) {
	  val.push_back(val1);
	}
	else {
	  cerr << "Error: illegal data in table '" << key << "'" << endl;
	  break_flag = true;
	  break;
	}
	// get_table() で積んだ値を捨てる．
	pop(1);
      }
      if ( !break_flag && setter(val) ) {
	ret = OK;
      }
    }
    else {
      // エラー
      cerr << "Error: illegal type in '" << key << "'" << endl;
    }

    // get_field() で積まれた要素を捨てる．
    pop(1);

    return ret;
  }

  /// @brief 指定したフィールドを vector<string> に変換してセッター関数を呼ぶ．
  /// @retval OK 成功した．
  /// @retval ERROR エラーが起こった．
  /// @retval NOT_FOUND 見つからなかった
  ///
  /// スタックは変化しない．
  RetType
  get_string_list_field(
    int index,                                        ///< [in] テーブルのスタック上の位置
    const char* key,                                  ///< [in] フィールド名
    std::function<bool(const vector<string>&)> setter ///< [in] セッター関数
  )
  {
    RetType ret = ERROR;
    int type = get_field(index, key);
    if ( type == LUA_TNIL ) {
      // 存在しなかった．
      ret = NOT_FOUND;
    }
    else if ( type == LUA_TTABLE ) {
      int n = L_len(-1);
      vector<string> val;
      val.reserve(n);
      for ( int i = 1; i <= n; ++ i ) {
	get_table(-1, i);
	string val1 = to_string(-1);
	val.push_back(val1);
	// get_table() で積んだ値を捨てる．
	pop(1);
      }
      if ( setter(val) ) {
	ret = OK;
      }
    }
    else {
      // エラー
      cerr << "Error: illegal type in '" << key << "'" << endl;
    }

    // get_field() で積まれた要素を捨てる．
    pop(1);

    return ret;
  }

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name C++ のメンバ関数を呼び出すためのインターフェイス
  /// @{
  //////////////////////////////////////////////////////////////////////

  /// @brief C++ のオブジェクトを upvalue に持つクロージャ関数を登録する．
  void
  set_cppclosure(
    const char* fname, ///< [in] lua 上の関数名
    void* objp,        ///< [in] オブジェクトのポインタ
    lua_CFunction func ///< [in] メンバ関数を呼び出す関数
  )
  {
    push_lightuserdata(objp);
    push_cclosure(func, 1);
    set_global(fname);
  }

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name raw 関数
  /// @{
  //////////////////////////////////////////////////////////////////////

  /// @brief バージョンを返す．
  lua_Number
  version()
  {
    return lua_version(mState);
  }

  // lua インタプリタを取り出す．
  lua_State*
  lua_state()
  {
    return mState;
  }

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name エラーメッセージ出力
  /// @{
  //////////////////////////////////////////////////////////////////////

  /// @brief エラーメッセージを出力する．
  int
  error_end(
    const string& msg ///< [in] エラーメッセージ
  )
  {
    push_boolean(false);
    push_string(msg.c_str());
    return 2;
  }

  /// @}
  //////////////////////////////////////////////////////////////////////

private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // mState を所有している時 true にするフラグ
  bool mOwner;

  // luaインタプリタの状態
  lua_State* mState;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

END_NAMESPACE_MAGUS

#endif // LUAPP_H
