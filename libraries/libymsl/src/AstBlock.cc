
/// @file AstBlock.cc
/// @brief AstBlock の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "AstBlock.h"
#include "AstFuncDecl.h"
#include "AstStatement.h"
#include "AstVarDecl.h"
#include "AstSymHandle.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス AstBlock
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parent 親のブロック
AstBlock::AstBlock(AstBlock* parent) :
  mParent(parent),
  mHashSize(0),
  mHashTable(NULL),
  mHashNum(0)
{
  alloc_table(16);
}

// @brief デストラクタ
AstBlock::~AstBlock()
{
}

// @brief statement を追加する．
void
AstBlock::add_statement(AstStatement* statement)
{
  mStatementList.push_back(statement);
  ShString label = statement->label();
  if ( label != ShString() ) {
    AstStmtHandle* handle = new AstStmtHandle(statement);
    put(handle);
  }
}

// @brief 関数定義を追加する．
// @param[in] item 追加する要素
void
AstBlock::add_funcdecl(AstFuncDecl* item)
{
  AstFuncHandle* handle = new AstFuncHandle(item);
  put(handle);
}

// @brief 変数定義を追加する．
// @param[in] item 追加する要素
void
AstBlock::add_vardecl(AstVarDecl* item)
{
  AstVarHandle* handle = new AstVarHandle(item);
  put(handle);
}

// @brief 名前からラベルステートメントを見つける．
// @param[in] name 名前
//
// ここになければ親のブロックを探す．
// それでもなければ NULL を返す．
AstStatement*
AstBlock::find_label(ShString name) const
{
  AstSymHandle* handle = find(name);
  if ( handle != NULL ) {
    return handle->statement();
  }
  return NULL;
}

// @brief 名前から関数宣言を見つける．
// @param[in] name 名前
//
// ここになければ親のブロックを探す．
// それでもなければ NULL を返す．
AstFuncDecl*
AstBlock::find_funcdecl(ShString name) const
{
  AstSymHandle* handle = find(name);
  if ( handle != NULL ) {
    return handle->funcdecl();
  }
  return NULL;
}

// @brief 名前から変数宣言を見つける．
// @param[in] name 名前
//
// ここになければ親のブロックを探す．
// それでもなければ NULL を返す．
AstVarDecl*
AstBlock::find_vardecl(ShString name) const
{
  AstSymHandle* handle = find(name);
  if ( handle != NULL ) {
    return handle->vardecl();
  }
  return NULL;
}

// @brief 内容を表示する．(デバッグ用)
// @param[in] s 出力ストリーム
// @param[in] indent インデントレベル
void
AstBlock::print(ostream& s,
		ymuint indent) const
{
#if 0
  YmslAstImpl::print_indent(s, indent);
  s << "{" << endl;

  ymuint n = mStatementList.size();
  for (ymuint i = 0; i < n; ++ i) {
    mStatementList[i]->print(s, indent + 1);
  }

  YmslAstImpl::print_indent(s, indent);
  s << "}" << endl;
#endif
}

// @brief ハッシュ表を確保する．
void
AstBlock::alloc_table(ymuint req_size)
{
  ymuint old_size = mHashSize;
  AstSymHandle** old_table = mHashTable;
  mHashSize = req_size;
  mNextLimit = static_cast<ymuint>(mHashSize * 1.8);
  mHashTable = new AstSymHandle*[mHashSize];
  for (ymuint i = 0; i < mHashSize; ++ i) {
    mHashTable[i] = NULL;
  }
  for (ymuint i = 0; i < old_size; ++ i) {
    for (AstSymHandle* handle = old_table[i];
	 handle != NULL; ) {
      AstSymHandle* next = handle->mLink;
      _put(handle);
      handle = next;
    }
  }
  delete [] old_table;
}

// @brief 名前からハンドルを探す．
// @param[in] name 名前
AstSymHandle*
AstBlock::find(ShString name) const
{
  ymuint pos = name.hash() % mHashSize;
  for (AstSymHandle* handle = mHashTable[pos];
       handle != NULL; handle = handle->mLink) {
    if ( handle->name() == name ) {
      return handle;
    }
  }
  if ( mParent != NULL ) {
    return mParent->find(name);
  }
  return NULL;
}

// @brief ハンドルを登録する．
void
AstBlock::put(AstSymHandle* handle)
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
AstBlock::_put(AstSymHandle* handle)
{
  ymuint pos = handle->name().hash() % mHashSize;
  handle->mLink = mHashTable[pos];
  mHashTable[pos] = handle;
  ++ mHashNum;
}


//////////////////////////////////////////////////////////////////////
// クラス AstSymHandle
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
AstSymHandle::AstSymHandle() :
  mLink(NULL)
{
}

// @brief デストラクタ
AstSymHandle::~AstSymHandle()
{
}

// @brief 変数宣言を返す．
//
// 他の要素の場合には NULL を返す．
AstVarDecl*
AstSymHandle::vardecl() const
{
  return NULL;
}

// @brief 関数宣言を返す．
//
// 他の要素の場合には NULL を返す．
AstFuncDecl*
AstSymHandle::funcdecl() const
{
  return NULL;
}

// @brief ラベルステートメントを返す．
//
// 他の要素の場合には NULL を返す．
AstStatement*
AstSymHandle::statement() const
{
  return NULL;
}


//////////////////////////////////////////////////////////////////////
// クラス AstVarHandle
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] vardecl 変数宣言
AstVarHandle::AstVarHandle(AstVarDecl* vardecl) :
  mVarDecl(vardecl)
{
}

// @brief デストラクタ
AstVarHandle::~AstVarHandle()
{
}

// @brief 名前を返す．
ShString
AstVarHandle::name() const
{
  return mVarDecl->name();
}

// @brief 変数宣言を返す．
//
// 他の要素の場合には NULL を返す．
AstVarDecl*
AstVarHandle::vardecl() const
{
  return mVarDecl;
}


//////////////////////////////////////////////////////////////////////
// クラス AstFuncHandle
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] funcdecl 関数宣言
AstFuncHandle::AstFuncHandle(AstFuncDecl* funcdecl) :
  mFuncDecl(funcdecl)
{
}

// @brief デストラクタ
AstFuncHandle::~AstFuncHandle()
{
}

// @brief 名前を返す．
ShString
AstFuncHandle::name() const
{
  return mFuncDecl->name();
}

// @brief 関数宣言を返す．
//
// 他の要素の場合には NULL を返す．
AstFuncDecl*
AstFuncHandle::funcdecl() const
{
  return mFuncDecl;
}


//////////////////////////////////////////////////////////////////////
// AstStmtHandle
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] statement ラベルすてーとめんと
AstStmtHandle::AstStmtHandle(AstStatement* statement) :
  mStatement(statement)
{
}

// @brief デストラクタ
AstStmtHandle::~AstStmtHandle()
{
}

// @brief 名前を返す．
ShString
AstStmtHandle::name() const
{
  return mStatement->label();
}

// @brief 関数宣言を返す．
//
// 他の要素の場合には NULL を返す．
AstStatement*
AstStmtHandle::statement() const
{
  return mStatement;
}

END_NAMESPACE_YM_YMSL
