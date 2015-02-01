
/// @file Label.cc
/// @brief Label の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "Label.h"
#include "IrNode.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス Label
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] name 名前
Label::Label(ShString name) :
  mName(name)
{
  mNextNode = NULL;
}

// @brief デストラクタ
Label::~Label()
{
}

// @brief 名前を返す．
ShString
Label::name() const
{
  return mName;
}

// @brief 配置済みか調べる．
bool
Label::is_placed() const
{
  return mNextNode != NULL;
}

// @brief 配置する
// @param[in] next_node 直後のノード
void
Label::place(IrNode* next_node)
{
  ASSERT_COND( !is_placed() );

  mNextNode = next_node;
  for (vector<IrNode*>::iterator p = mRefList.begin();
       p != mRefList.end(); ++ p) {
    IrNode* node = *p;
    node->set_addr(mNextNode);
  }
  mRefList.clear();
}

// @brief 参照する．
// @param[in] rerer_node 参照しているノード
void
Label::add_refer(IrNode* refer_node)
{
  if ( is_placed() ) {
    refer_node->set_addr(mNextNode);
  }
  else {
    mRefList.push_back(refer_node);
  }
}

END_NAMESPACE_YM_YMSL
