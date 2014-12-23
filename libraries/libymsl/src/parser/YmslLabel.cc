
/// @file YmslLabel.cc
/// @brief YmslLabel の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmslLabel.h"
#include "YmslCodeList.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス YmslLabel
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] code_list 命令コードを収める配列
// @param[in] name 名前
YmslLabel::YmslLabel(YmslCodeList& code_list,
		     ShString name) :
  mCodeList(code_list),
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
  for (vector<Ymsl_INT>::iterator p = mRefList.begin();
       p != mRefList.end(); ++ p) {
    Ymsl_INT w_pos = *p;
    mCodeList.write_int(w_pos, addr);
  }
}

// @brief 参照する．
// @param[in] w_pos アドレスを書き込む位置
void
YmslLabel::refer(Ymsl_INT w_pos)
{
  if ( is_placed() ) {
    mCodeList.write_int(w_pos, mAddr);
  }
  else {
    mRefList.push_back(w_pos);
  }
}

END_NAMESPACE_YM_YMSL
