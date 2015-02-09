
/// @file Scope.cc
/// @brief Scope の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "Scope.h"
#include "ConstHandle.h"
#include "FuncHandle.h"
#include "LabelHandle.h"
#include "ScopeHandle.h"
#include "TypeHandle.h"
#include "VarHandle.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス Scope
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parent 親のスコープ
// @param[in] name 名前
Scope::Scope(Scope* parent,
	     ShString name) :
  mParent(parent),
  mName(name),
  mHashSize(0),
  mHashTable(NULL),
  mHashNum(0)
{
  alloc_table(16);
}

// @brief デストラクタ
Scope::~Scope()
{
  for (ymuint i = 0; i < mHashSize; ++ i) {
    for (IrHandle* h = mHashTable[i]; h; ) {
      IrHandle* n = h->mLink;
      delete h;
      h = n;
    }
  }
  delete [] mHashTable;
}

// @brief 自身の名前を返す．
//
// ShString() (空文字列)の場合もある．
ShString
Scope::name() const
{
  return mName;
}

// @brief ラベルを追加する．
// @param[in] name 名前
// @param[in] item 追加する要素
void
Scope::add_label(ShString name,
		 IrNode* item)
{
  IrHandle* handle = new LabelHandle(name, item);
  put(handle);
}

// @brief 関数を追加する．
// @param[in] item 追加する要素
void
Scope::add_function(const Function* item)
{
  IrHandle* handle = new FuncHandle(item);
  put(handle);
}

// @brief 変数を追加する．
// @param[in] item 追加する要素
void
Scope::add_var(const Var* item)
{
  IrHandle* handle = new VarHandle(item);
  put(handle);
}

// @brief 型を追加する．
// @param[in] item 追加する要素
// @param[in] scope item のスコープ
void
Scope::add_named_type(const Type* item,
		      Scope* scope)
{
  IrHandle* handle = new TypeHandle(item, scope);
  put(handle);
}

// @brief 定数ノードを追加する．
// @param[in] name 名前
// @param[in] item 追加する要素
void
Scope::add_constant(ShString name,
		    IrNode* item)
{
  IrHandle* handle = new ConstHandle(name, item);
  put(handle);
}

// @brief スコープを追加する．
// @param[in] item 追加する要素
void
Scope::add_scope(Scope* item)
{
  IrHandle* handle = new ScopeHandle(item);
  put(handle);
}

// @brief ハッシュ表を確保する．
void
Scope::alloc_table(ymuint req_size)
{
  ymuint old_size = mHashSize;
  IrHandle** old_table = mHashTable;
  mHashSize = req_size;
  mNextLimit = static_cast<ymuint>(mHashSize * 1.8);
  mHashTable = new IrHandle*[mHashSize];
  for (ymuint i = 0; i < mHashSize; ++ i) {
    mHashTable[i] = NULL;
  }
  for (ymuint i = 0; i < old_size; ++ i) {
    for (IrHandle* handle = old_table[i];
	 handle != NULL; ) {
      IrHandle* next = handle->mLink;
      _put(handle);
      handle = next;
    }
  }
  delete [] old_table;
}

// @brief 名前からハンドルを探す．
// @param[in] name 名前
IrHandle*
Scope::find(ShString name) const
{
  IrHandle* h = find_local(name);
  if ( h == NULL && mParent != NULL ) {
    h = mParent->find(name);
  }
  return h;
}

// @brief 名前からハンドルを探す．
// @param[in] name 名前
//
// こちらはこのスコープのみ探す．
IrHandle*
Scope::find_local(ShString name) const
{
  ymuint pos = name.hash() % mHashSize;
  for (IrHandle* handle = mHashTable[pos];
       handle != NULL; handle = handle->mLink) {
    if ( handle->name() == name ) {
      return handle;
    }
  }
  return NULL;
}

// @brief ハンドルを登録する．
void
Scope::put(IrHandle* handle)
{
  if ( mHashNum >= mNextLimit ) {
    alloc_table(mHashSize << 1);
  }

  _put(handle);
}

// @brief ハンドルを登録する．
//
// こちらはサイズチェックなし
void
Scope::_put(IrHandle* handle)
{
  ymuint pos = handle->name().hash() % mHashSize;
  handle->mLink = mHashTable[pos];
  mHashTable[pos] = handle;
  ++ mHashNum;
}

END_NAMESPACE_YM_YMSL
