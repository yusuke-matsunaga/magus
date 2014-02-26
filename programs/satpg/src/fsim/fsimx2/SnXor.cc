
/// @file SnXor.cc
/// @brief SnXor の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012-2014 Yusuke Matsunaga
/// All rights reserved.


#include "SnXor.h"


BEGIN_NAMESPACE_YM_SATPG_FSIMX2

//////////////////////////////////////////////////////////////////////
// SnXor
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SnXor::SnXor(ymuint32 id,
	     const vector<SimNode*>& inputs) :
  SnGate(id, inputs)
{
}

// @brief デストラクタ
SnXor::~SnXor()
{
}

// @brief 正常値の計算を行う．(3値版)
void
SnXor::_calc_gval3()
{
  PackedVal val0 = mFanins[0]->gval_0();
  PackedVal val1 = mFanins[0]->gval_1();
  for (ymuint i = 1; i < mNfi; ++ i) {
    PackedVal tmp_val0 = mFanins[i]->gval_0();
    PackedVal tmp_val1 = mFanins[i]->gval_1();

    PackedVal a_val0 = (val0 | tmp_val1);
    PackedVal a_val1 = (val1 & tmp_val0);
    PackedVal b_val0 = (val1 | tmp_val0);
    PackedVal b_val1 = (val0 & tmp_val1);

    val0 = a_val0 & b_val0;
    val1 = a_val1 | b_val1;
  }
  mGval0 = val0;
  mGval1 = val1;
}

// @brief 故障値の計算を行う．(3値版)
// @param[in] mask マスク
// @note 結果は mFval0, mFval1 に格納される．
void
SnXor::_calc_fval3(PackedVal mask)
{
  PackedVal val0 = mFanins[0]->fval_0();
  PackedVal val1 = mFanins[0]->fval_1();
  for (ymuint i = 1; i < mNfi; ++ i) {
    PackedVal tmp_val0 = mFanins[i]->fval_0();
    PackedVal tmp_val1 = mFanins[i]->fval_1();

    PackedVal a_val0 = (val0 | tmp_val1);
    PackedVal a_val1 = (val1 & tmp_val0);
    PackedVal b_val0 = (val1 | tmp_val0);
    PackedVal b_val1 = (val0 & tmp_val1);

    val0 = a_val0 & b_val0;
    val1 = a_val1 | b_val1;
  }
  mFval0 &= ~mask;
  mFval0 |= val0 & mask;
  mFval1 &= ~mask;
  mFval1 |= val1 & mask;
}

// @brief ゲートの入力から出力までの可観測性を計算する．(3値版)
PackedVal
SnXor::calc_gobs3(ymuint ipos)
{
  return kPvAll1;
}

// @brief 内容をダンプする．
void
SnXor::dump(ostream& s) const
{
  s << "XOR(" << mFanins[0]->id();
  for (ymuint i = 1; i < mNfi; ++ i) {
    s << ", " << mFanins[i]->id();
  }
  s << ")" << endl;
}


//////////////////////////////////////////////////////////////////////
// SnXor2
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SnXor2::SnXor2(ymuint32 id,
	       const vector<SimNode*>& inputs) :
  SnGate2(id, inputs)
{
}

// @brief デストラクタ
SnXor2::~SnXor2()
{
}

// @brief 正常値の計算を行う．(3値版)
void
SnXor2::_calc_gval3()
{
  PackedVal tmp0_0 = mFanins[0]->gval_0() | mFanins[1]->gval_1();
  PackedVal tmp0_1 = mFanins[0]->gval_1() & mFanins[1]->gval_0();

  PackedVal tmp1_0 = mFanins[0]->gval_1() | mFanins[1]->gval_0();
  PackedVal tmp1_1 = mFanins[0]->gval_0() & mFanins[1]->gval_1();

  mGval0 = tmp0_0 & tmp1_0;
  mGval1 = tmp0_1 | tmp1_1;
}

// @brief 故障値の計算を行う．(3値版)
// @param[in] mask マスク
// @note 結果は mFval0, mFval1 に格納される．
void
SnXor2::_calc_fval3(PackedVal mask)
{
  PackedVal tmp0_0 = mFanins[0]->fval_0() | mFanins[1]->fval_1();
  PackedVal tmp0_1 = mFanins[0]->fval_1() & mFanins[1]->fval_0();

  PackedVal tmp1_0 = mFanins[0]->fval_1() | mFanins[1]->fval_0();
  PackedVal tmp1_1 = mFanins[0]->fval_0() & mFanins[1]->fval_1();

  mFval0 &= ~mask;
  mFval0 |= (tmp0_0 & tmp1_0) & mask;
  mFval1 &= ~mask;
  mFval1 |= (tmp0_1 | tmp1_1) & mask;
}

// @brief ゲートの入力から出力までの可観測性を計算する．(3値版)
PackedVal
SnXor2::calc_gobs3(ymuint ipos)
{
  return kPvAll1;
}

// @brief 内容をダンプする．
void
SnXor2::dump(ostream& s) const
{
  s << "XOR2(" << mFanins[0]->id();
  s << ", " << mFanins[1]->id();
  s << ")" << endl;
}


//////////////////////////////////////////////////////////////////////
// SnXnor
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SnXnor::SnXnor(ymuint32 id,
	       const vector<SimNode*>& inputs) :
  SnXor(id, inputs)
{
}

// @brief デストラクタ
SnXnor::~SnXnor()
{
}

// @brief 正常値の計算を行う．(3値版)
void
SnXnor::_calc_gval3()
{
  PackedVal val0 = mFanins[0]->gval_0();
  PackedVal val1 = mFanins[0]->gval_1();
  for (ymuint i = 1; i < mNfi; ++ i) {
    PackedVal tmp_val0 = mFanins[i]->gval_0();
    PackedVal tmp_val1 = mFanins[i]->gval_1();

    PackedVal a_val0 = (val0 | tmp_val1);
    PackedVal a_val1 = (val1 & tmp_val0);
    PackedVal b_val0 = (val1 | tmp_val0);
    PackedVal b_val1 = (val0 & tmp_val1);

    val0 = a_val0 & b_val0;
    val1 = a_val1 | b_val1;
  }
  mGval0 = val1;
  mGval1 = val0;
}

// @brief 故障値の計算を行う．(3値版)
// @param[in] mask マスク
// @note 結果は mFval0, mFval1 に格納される．
void
SnXnor::_calc_fval3(PackedVal mask)
{
  PackedVal val0 = mFanins[0]->fval_0();
  PackedVal val1 = mFanins[0]->fval_1();
  for (ymuint i = 1; i < mNfi; ++ i) {
    PackedVal tmp_val0 = mFanins[i]->fval_0();
    PackedVal tmp_val1 = mFanins[i]->fval_1();

    PackedVal a_val0 = (val0 | tmp_val1);
    PackedVal a_val1 = (val1 & tmp_val0);
    PackedVal b_val0 = (val1 | tmp_val0);
    PackedVal b_val1 = (val0 & tmp_val1);

    val0 = a_val0 & b_val0;
    val1 = a_val1 | b_val1;
  }
  mFval0 &= ~mask;
  mFval0 |= val1 & mask;
  mFval1 &= ~mask;
  mFval1 |= val0 & mask;
}

// @brief 内容をダンプする．
void
SnXnor::dump(ostream& s) const
{
  s << "XNOR(" << mFanins[0]->id();
  for (ymuint i = 1; i < mNfi; ++ i) {
    s << ", " << mFanins[i]->id();
  }
  s << ")" << endl;
}


//////////////////////////////////////////////////////////////////////
// SnXnor2
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SnXnor2::SnXnor2(ymuint32 id,
		 const vector<SimNode*>& inputs) :
  SnXor2(id, inputs)
{
}

// @brief デストラクタ
SnXnor2::~SnXnor2()
{
}

// @brief 正常値の計算を行う．(3値版)
void
SnXnor2::_calc_gval3()
{
  PackedVal tmp0_0 = mFanins[0]->gval_0() | mFanins[1]->gval_1();
  PackedVal tmp0_1 = mFanins[0]->gval_1() & mFanins[1]->gval_0();

  PackedVal tmp1_0 = mFanins[0]->gval_1() | mFanins[1]->gval_0();
  PackedVal tmp1_1 = mFanins[0]->gval_0() & mFanins[1]->gval_1();

  mGval1 = tmp0_0 & tmp1_0;
  mGval0 = tmp0_1 | tmp1_1;
}

// @brief 故障値の計算を行う．(3値版)
// @param[in] mask マスク
// @note 結果は mFval0, mFval1 に格納される．
void
SnXnor2::_calc_fval3(PackedVal mask)
{
  PackedVal tmp0_0 = mFanins[0]->fval_0() | mFanins[1]->fval_1();
  PackedVal tmp0_1 = mFanins[0]->fval_1() & mFanins[1]->fval_0();

  PackedVal tmp1_0 = mFanins[0]->fval_1() | mFanins[1]->fval_0();
  PackedVal tmp1_1 = mFanins[0]->fval_0() & mFanins[1]->fval_1();

  mFval1 &= ~mask;
  mFval1 |= (tmp0_0 & tmp1_0) & mask;
  mFval0 &= ~mask;
  mFval0 |= (tmp0_1 | tmp1_1) & mask;
}

// @brief 内容をダンプする．
void
SnXnor2::dump(ostream& s) const
{
  s << "XNOR2(" << mFanins[0]->id();
  s << ", " << mFanins[1]->id();
  s << ")" << endl;
}

END_NAMESPACE_YM_SATPG_FSIMX2
