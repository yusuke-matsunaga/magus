
/// @file YmslLabel.cc
/// @brief YmslLabel の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmslLabel.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス YmslLabel
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] name 名前
YmslLabel::YmslLabel(ShString name) :
  mName(name)
{
  mPlaced = false;
}

// @brief デストラクタ
YmslLabel::~YmslLabel()
{
}

// @brief 名前を返す．
ShString
YmslLabel::name() const
{
  return mName;
}

// @brief 配置済みか調べる．
bool
YmslLabel::is_placed() const
{
  return mPlaced;
}

// @brief 配置する
// @param[in] addr アドレス
void
YmslLabel::place(Ymsl_INT addr)
{
  ASSERT_COND( !mPlaced );
  mPlaced = true;
  mAddr = addr;
  for (vector<Ymsl_INT*>::iterator p = mRefList.begin();
       p != mRefList.end(); ++ p) {
    Ymsl_INT* p_ref = *p;
    *p_ref = addr;
  }
}

// @brief 参照する．
// @param[in] p_ref アドレスを書き込む位置
void
YmslLabel::refer(Ymsl_INT* p_ref)
{
  if ( is_placed() ) {
    *p_ref = mAddr;
  }
  else {
    mRefList.push_back(p_ref);
  }
}

END_NAMESPACE_YM_YMSL
