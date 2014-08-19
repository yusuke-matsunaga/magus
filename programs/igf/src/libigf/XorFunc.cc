
/// @file XorFunc.cc
/// @brief XorFunc の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "XorFunc.h"
#include "RegVect.h"


BEGIN_NAMESPACE_YM_IGF

//////////////////////////////////////////////////////////////////////
// クラス XorFunc
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] vars_list 変数のリストのリスト
XorFunc::XorFunc(const vector<vector<ymuint32> >& vars_list) :
  mVarsList(vars_list)
{
}

// @brief デストラクタ
XorFunc::~XorFunc()
{
}

// @brief 出力のビット幅を返す．
ymuint
XorFunc::output_width() const
{
  return mVarsList.size();
}

// @brief 関数値を求める．
// @param[in] rv 登録ベクタ
ymuint32
XorFunc::eval(const RegVect* rv) const
{
  ymuint32 v = 0U;
  ymuint n = mVarsList.size();
  for (ymuint i = 0; i < n; ++ i) {
    const vector<ymuint32>& vars = mVarsList[i];
    ymuint nv = vars.size();
    bool iv = false;
    for (ymuint j = 0; j < nv; ++ j) {
      ymuint pos = vars[j];
      if ( rv->val(pos) ) {
	iv ^= true;
      }
    }
    if ( iv ) {
      v |= (1U << i);
    }
  }
  return v;
}

// @brief 内容を表示する．
// @param[in] s 出力先のストリーム
void
XorFunc::dump(ostream& s) const
{
  ymuint n = mVarsList.size();
  s << "{";
  const char* comma = "";
  for (ymuint i = 0; i < n; ++ i) {
    const vector<ymuint32>& vars = mVarsList[i];
    ymuint nv = vars.size();
    s << comma << "{";
    comma = ", ";
    for (ymuint j = 0; j < nv; ++ j) {
      s << " " << vars[j];
    }
    s << "}";
  }
  s << "}" << endl;
}

END_NAMESPACE_YM_IGF
