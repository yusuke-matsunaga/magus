
/// @file Scope.cc
/// @brief Scope の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "Scope.h"
#include "IrHandle.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス Scope
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parent_scope 親のスコープ
// @param[in] global_scope グローバルスコープ
// @param[in] name 名前
Scope::Scope(Scope* parent_scope,
	     Scope* global_scope,
	     ShString name) :
  mParentScope(parent_scope),
  mGlobalScope(global_scope),
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
  // IrHandleのメモリは IrMgr が管理している．
  // のでここで開放する必要はない．

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

// @brief グローバルスコープを返す．
Scope*
Scope::global_scope() const
{
  return mGlobalScope;
}

// @brief 要素を追加する．
// @param[in] item 追加する要素
void
Scope::add(IrHandle* item)
{
  if ( mHashNum >= mNextLimit ) {
    alloc_table(mHashSize << 1);
  }

  _put(item);

  if ( item->handle_type() == IrHandle::kVar ) {
    const IrVar* var = item->var();
    mVarList.push_back(var);
  }
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
  if ( h != NULL ) {
    return h;
  }

  if ( mParentScope != NULL ) {
    h = mParentScope->find(name);
    if ( h != NULL ) {
      return h;
    }
  }

  if ( mGlobalScope != NULL ) {
    h = mGlobalScope->find(name);
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
