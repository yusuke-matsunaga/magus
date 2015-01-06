
/// @file AstStmtList.cc
/// @brief AstStmtList の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "AstStmtList.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス AstStmtList
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
AstStmtList::AstStmtList()
{
  mSize = 0;
  mTop = NULL;
  mTail = NULL;
}

// @brief デストラクタ
AstStmtList::~AstStmtList()
{
}

// @brief 要素を追加する．
// @param[in] item 追加する要素．
void
AstStmtList::add(AstStatement* item)
{
  if ( mTop == NULL ) {
    ASSERT_COND( mTail == NULL );
    mTop = item;
    mTail = item;
  }
  else {
    ASSERT_COND( mTail != NULL );
    mTail->set_next(item);
    mTail = item;
  }
  ++ mSize;
}

// @brief 要素数を返す．
ymuint
AstStmtList::size() const
{
  return mSize;
}

// @brief 先頭の要素を取り出す．
AstStatement*
AstStmtList::top() const
{
  return mTop;
}

END_NAMESPACE_YM_YMSL
