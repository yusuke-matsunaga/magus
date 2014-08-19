
/// @file VarFunc.cc
/// @brief VarFunc の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "VarFunc.h"
#include "RegVect.h"


BEGIN_NAMESPACE_YM_IGF

//////////////////////////////////////////////////////////////////////
// クラス VarFunc
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] var_list 変数のリスト
VarFunc::VarFunc(const vector<ymuint32>& var_list) :
  mVarList(var_list)
{
}

// @brief デストラクタ
VarFunc::~VarFunc()
{
}

// @brief 出力数を返す．
ymuint
VarFunc::output_num() const
{
  return mVarList.size();
}

// @brief 関数値を求める．
ymuint32
VarFunc::eval(const RegVect* rv) const
{
  ymuint32 v = 0U;
  ymuint n = mVarList.size();
  for (ymuint i = 0; i < n; ++ i) {
    ymuint pos = mVarList[i];
    if ( rv->val(pos) ) {
      v |= (1U << i);
    }
  }
  return v;
}

// @brief 内容を表示する．
void
VarFunc::dump(ostream& s) const
{
  ymuint n = mVarList.size();
  s << "{";
  for (ymuint i = 0; i < n; ++ i) {
    s << " " << mVarList[i];
  }
  s << "}" << endl;
}

END_NAMESPACE_YM_IGF
