
/// @file SnOr.cc
/// @brief SnOr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012-2014 Yusuke Matsunaga
/// All rights reserved.


#include "SnOr.h"


BEGIN_NAMESPACE_YM_SATPG_FSIM

//////////////////////////////////////////////////////////////////////
// SnOr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SnOr::SnOr(ymuint32 id,
	   const vector<SimNode*>& inputs) :
  SnGate(id, inputs)
{
}

// @brief デストラクタ
SnOr::~SnOr()
{
}

// @brief 正常値の計算を行う．(3値版)
void
SnOr::_calc_gval3()
{
  PackedVal val0 = mFanins[0]->gval_0();
  PackedVal val1 = mFanins[0]->gval_1();
  for (ymuint i = 1; i < mNfi; ++ i) {
    val0 &= mFanins[i]->gval_0();
    val1 |= mFanins[i]->gval_1();
  }
  mGval0 = val0;
  mGval1 = val1;
}

// @brief 故障値の計算を行う．(3値版)
// @param[in] mask マスク
// @note 結果は mFval0, mFval1 に格納される．
void
SnOr::_calc_fval3(PackedVal mask)
{
  PackedVal val0 = mFanins[0]->fval_0();
  PackedVal val1 = mFanins[0]->fval_1();
  for (ymuint i = 1; i < mNfi; ++ i) {
    val0 &= mFanins[i]->fval_0();
    val1 |= mFanins[i]->fval_1();
  }
  mFval0 &= ~mask;
  mFval0 |= val0 & mask;
  mFval1 &= ~mask;
  mFval1 |= val1 & mask;
}

// @brief ゲートの入力から出力までの可観測性を計算する．(3値版)
PackedVal
SnOr::calc_gobs3(ymuint ipos)
{
  PackedVal obs = kPvAll1;
  for (ymuint i = 0; i < ipos; ++ i) {
    obs &= mFanins[i]->gval_0();
  }
  for (ymuint i = ipos + 1; i < mNfi; ++ i) {
    obs &= mFanins[i]->gval_0();
  }
  return obs;
}

// @brief 内容をダンプする．
void
SnOr::dump(ostream& s) const
{
  ymuint n = mNfi;
  s << "OR(" << mFanins[0]->id();
  for (ymuint i = 1; i < n; ++ i) {
    s << ", " << mFanins[i]->id();
  }
  s << ")" << endl;
}


//////////////////////////////////////////////////////////////////////
// SnOr2
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SnOr2::SnOr2(ymuint32 id,
	     const vector<SimNode*>& inputs) :
  SnGate2(id, inputs)
{
}

// @brief デストラクタ
SnOr2::~SnOr2()
{
}

// @brief 正常値の計算を行う．(3値版)
void
SnOr2::_calc_gval3()
{
  mGval0 = mFanins[0]->gval_0() & mFanins[1]->gval_0();
  mGval1 = mFanins[0]->gval_1() | mFanins[1]->gval_1();
}

// @brief 故障値の計算を行う．(3値版)
// @param[in] mask マスク
// @note 結果は mFval0, mFval1 に格納される．
void
SnOr2::_calc_fval3(PackedVal mask)
{
  mFval0 &= ~mask;
  mFval0 |= (mFanins[0]->fval_0() & mFanins[1]->fval_0()) & mask;
  mFval1 &= ~mask;
  mFval1 |= (mFanins[0]->fval_1() | mFanins[1]->fval_1()) & mask;
}

// @brief ゲートの入力から出力までの可観測性を計算する．(3値版)
PackedVal
SnOr2::calc_gobs3(ymuint ipos)
{
  return mFanins[ipos ^ 1]->gval_0();
}

// @brief 内容をダンプする．
void
SnOr2::dump(ostream& s) const
{
  s << "OR2(" << mFanins[0]->id();
  s << ", " << mFanins[1]->id();
  s << ")" << endl;
}


//////////////////////////////////////////////////////////////////////
// SnOr3
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SnOr3::SnOr3(ymuint32 id,
	     const vector<SimNode*>& inputs) :
  SnGate3(id, inputs)
{
}

// @brief デストラクタ
SnOr3::~SnOr3()
{
}

// @brief 正常値の計算を行う．(3値版)
void
SnOr3::_calc_gval3()
{
  mGval0 = mFanins[0]->gval_0() & mFanins[1]->gval_0() & mFanins[2]->gval_0();
  mGval1 = mFanins[0]->gval_1() | mFanins[1]->gval_1() | mFanins[2]->gval_1();
}

// @brief 故障値の計算を行う．(3値版)
// @param[in] mask マスク
// @note 結果は mFval0, mFval1 に格納される．
void
SnOr3::_calc_fval3(PackedVal mask)
{
  mFval0 &= ~mask;
  mFval0 |= (mFanins[0]->fval_0() & mFanins[1]->fval_0() & mFanins[2]->fval_0()) & mask;
  mFval1 &= ~mask;
  mFval1 |= (mFanins[0]->fval_1() | mFanins[1]->fval_1() | mFanins[2]->fval_1()) & mask;
}

// @brief ゲートの入力から出力までの可観測性を計算する．(3値版)
PackedVal
SnOr3::calc_gobs3(ymuint ipos)
{
  ymuint idx1 = 0;
  ymuint idx2 = 0;
  switch ( ipos ) {
  case 0:
    idx1 = 1;
    idx2 = 2;
    break;

  case 1:
    idx1 = 0;
    idx2 = 2;
    break;

  case 2:
    idx1 = 0;
    idx2 = 1;
    break;

  default:
    assert_not_reached(__FILE__, __LINE__);
    break;
  }
  PackedVal pat0 = mFanins[idx1]->gval_0();
  PackedVal pat1 = mFanins[idx2]->gval_0();
  return pat0 & pat1;
}

// @brief 内容をダンプする．
void
SnOr3::dump(ostream& s) const
{
  s << "OR3(" << mFanins[0]->id();
  s << ", " << mFanins[1]->id();
  s << ", " << mFanins[2]->id();
  s << ")" << endl;
}


//////////////////////////////////////////////////////////////////////
// SnOr4
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SnOr4::SnOr4(ymuint32 id,
	     const vector<SimNode*>& inputs) :
  SnGate4(id, inputs)
{
}

// @brief デストラクタ
SnOr4::~SnOr4()
{
}

// @brief 正常値の計算を行う．(3値版)
void
SnOr4::_calc_gval3()
{
  mGval0 = mFanins[0]->gval_0() & mFanins[1]->gval_0() & mFanins[2]->gval_0() & mFanins[3]->gval_0();
  mGval1 = mFanins[0]->gval_1() | mFanins[1]->gval_1() | mFanins[2]->gval_1() | mFanins[3]->gval_1();
}

// @brief 故障値の計算を行う．(3値版)
// @param[in] mask マスク
// @note 結果は mFval0, mFval1 に格納される．
void
SnOr4::_calc_fval3(PackedVal mask)
{
  mFval0 &= ~mask;
  mFval0 |= (mFanins[0]->fval_0() & mFanins[1]->fval_0() & mFanins[2]->fval_0() & mFanins[3]->fval_0()) & mask;
  mFval1 &= ~mask;
  mFval1 |= (mFanins[0]->fval_1() | mFanins[1]->fval_1() | mFanins[2]->fval_1() | mFanins[3]->fval_1()) & mask;
}

// @brief ゲートの入力から出力までの可観測性を計算する．(3値版)
PackedVal
SnOr4::calc_gobs3(ymuint ipos)
{
  ymuint idx1 = 0;
  ymuint idx2 = 0;
  ymuint idx3 = 0;
  switch ( ipos ) {
  case 0:
    idx1 = 1;
    idx2 = 2;
    idx3 = 3;
    break;

  case 1:
    idx1 = 0;
    idx2 = 2;
    idx3 = 3;
    break;

  case 2:
    idx1 = 0;
    idx2 = 1;
    idx3 = 3;
    break;

  case 3:
    idx1 = 0;
    idx2 = 1;
    idx3 = 2;
    break;

  default:
    assert_not_reached(__FILE__, __LINE__);
    break;
  }
  PackedVal pat0 = mFanins[idx1]->gval_0();
  PackedVal pat1 = mFanins[idx2]->gval_0();
  PackedVal pat2 = mFanins[idx3]->gval_0();
  return pat0 & pat1 & pat2;
}

// @brief 内容をダンプする．
void
SnOr4::dump(ostream& s) const
{
  s << "OR4(" << mFanins[0]->id();
  s << ", " << mFanins[1]->id();
  s << ", " << mFanins[2]->id();
  s << ", " << mFanins[3]->id();
  s << ")" << endl;
}


//////////////////////////////////////////////////////////////////////
// SnNor
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SnNor::SnNor(ymuint32 id,
	     const vector<SimNode*>& inputs) :
  SnOr(id, inputs)
{
}

// @brief デストラクタ
SnNor::~SnNor()
{
}

// @brief 正常値の計算を行う．(3値版)
void
SnNor::_calc_gval3()
{
  PackedVal val0 = mFanins[0]->gval_0();
  PackedVal val1 = mFanins[0]->gval_1();
  for (ymuint i = 1; i < mNfi; ++ i) {
    val0 &= mFanins[i]->gval_0();
    val1 |= mFanins[i]->gval_1();
  }
  mGval0 = val1;
  mGval1 = val0;
}

// @brief 故障値の計算を行う．(3値版)
// @param[in] mask マスク
// @note 結果は mFval0, mFval1 に格納される．
void
SnNor::_calc_fval3(PackedVal mask)
{
  PackedVal val0 = mFanins[0]->fval_0();
  PackedVal val1 = mFanins[0]->fval_1();
  for (ymuint i = 1; i < mNfi; ++ i) {
    val0 &= mFanins[i]->fval_0();
    val1 |= mFanins[i]->fval_1();
  }
  mFval0 &= ~mask;
  mFval0 |= val1 & mask;
  mFval1 &= ~mask;
  mFval1 |= val0 & mask;
}

// @brief 内容をダンプする．
void
SnNor::dump(ostream& s) const
{
  s << "NOR(" << mFanins[0]->id();
  for (ymuint i = 1; i < mNfi; ++ i) {
    s << ", " << mFanins[i]->id();
  }
  s << ")" << endl;
}


//////////////////////////////////////////////////////////////////////
// SnNor2
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SnNor2::SnNor2(ymuint32 id,
	       const vector<SimNode*>& inputs) :
  SnOr2(id, inputs)
{
}

// @brief デストラクタ
SnNor2::~SnNor2()
{
}

// @brief 正常値の計算を行う．(3値版)
void
SnNor2::_calc_gval3()
{
  mGval1 = mFanins[0]->gval_0() & mFanins[1]->gval_0();
  mGval0 = mFanins[0]->gval_1() | mFanins[1]->gval_1();
}

// @brief 故障値の計算を行う．(3値版)
// @param[in] mask マスク
// @note 結果は mFval0, mFval1 に格納される．
void
SnNor2::_calc_fval3(PackedVal mask)
{
  mFval1 &= ~mask;
  mFval1 |= (mFanins[0]->fval_0() & mFanins[1]->fval_0()) & mask;
  mFval0 &= ~mask;
  mFval0 |= (mFanins[0]->fval_1() | mFanins[1]->fval_1()) & mask;
}

// @brief 内容をダンプする．
void
SnNor2::dump(ostream& s) const
{
  s << "NOR2(" << mFanins[0]->id();
  s << ", " << mFanins[1]->id();
  s << ")" << endl;
}


//////////////////////////////////////////////////////////////////////
// SnNor3
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SnNor3::SnNor3(ymuint32 id,
	       const vector<SimNode*>& inputs) :
  SnOr3(id, inputs)
{
}

// @brief デストラクタ
SnNor3::~SnNor3()
{
}

// @brief 正常値の計算を行う．(3値版)
void
SnNor3::_calc_gval3()
{
  mGval1 = mFanins[0]->gval_0() & mFanins[1]->gval_0() & mFanins[2]->gval_0();
  mGval0 = mFanins[0]->gval_1() | mFanins[1]->gval_1() | mFanins[2]->gval_1();
}

// @brief 故障値の計算を行う．(3値版)
// @param[in] mask マスク
// @note 結果は mFval0, mFval1 に格納される．
void
SnNor3::_calc_fval3(PackedVal mask)
{
  mFval1 &= ~mask;
  mFval1 |= (mFanins[0]->fval_0() & mFanins[1]->fval_0() & mFanins[2]->fval_0()) & mask;
  mFval0 &= ~mask;
  mFval0 |= (mFanins[0]->fval_1() | mFanins[1]->fval_1() | mFanins[2]->fval_1()) & mask;
}

// @brief 内容をダンプする．
void
SnNor3::dump(ostream& s) const
{
  s << "NOR3(" << mFanins[0]->id();
  s << ", " << mFanins[1]->id();
  s << ", " << mFanins[2]->id();
  s << ")" << endl;
}


//////////////////////////////////////////////////////////////////////
// SnNor4
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SnNor4::SnNor4(ymuint32 id,
	       const vector<SimNode*>& inputs) :
  SnOr4(id, inputs)
{
}

// @brief デストラクタ
SnNor4::~SnNor4()
{
}

// @brief 正常値の計算を行う．(3値版)
void
SnNor4::_calc_gval3()
{
  mGval1 = mFanins[0]->gval_0() & mFanins[1]->gval_0() & mFanins[2]->gval_0() & mFanins[3]->gval_0();
  mGval0 = mFanins[0]->gval_1() | mFanins[1]->gval_1() | mFanins[2]->gval_1() | mFanins[3]->gval_1();
}

// @brief 故障値の計算を行う．(3値版)
// @param[in] mask マスク
// @note 結果は mFval0, mFval1 に格納される．
void
SnNor4::_calc_fval3(PackedVal mask)
{
  mFval1 &= ~mask;
  mFval1 |= (mFanins[0]->fval_0() & mFanins[1]->fval_0() & mFanins[2]->fval_0() & mFanins[3]->fval_0()) & mask;
  mFval0 &= ~mask;
  mFval0 |= (mFanins[0]->fval_1() | mFanins[1]->fval_1() | mFanins[2]->fval_1() | mFanins[3]->fval_1()) & mask;
}

// @brief 内容をダンプする．
void
SnNor4::dump(ostream& s) const
{
  s << "NOR4(" << mFanins[0]->id();
  s << ", " << mFanins[1]->id();
  s << ", " << mFanins[2]->id();
  s << ", " << mFanins[3]->id();
  s << ")" << endl;
}

END_NAMESPACE_YM_SATPG_FSIM
