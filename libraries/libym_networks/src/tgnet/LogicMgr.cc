
/// @file LogicMgr.cc
/// @brief LogicMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013-2014 Yusuke Matsunaga
/// All rights reserved.


#include "LogicMgr.h"


BEGIN_NAMESPACE_YM_NETWORKS_TGNET

// ハッシュ表を拡大するときに使われる定数
const double LogicMgr::kHashCapacity = 1.8;

// @brief コンストラクタ
LogicMgr::LogicMgr()
{
  alloc_table(1024);
}

// @brief デストラクタ
LogicMgr::~LogicMgr()
{
  ymuint n = mCellArray.size();
  for (ymuint i = 0; i < n; ++ i) {
    delete mCellArray[i];
  }
  delete [] mHashTable;
}

// @brief 初期化する．
void
LogicMgr::clear()
{
  ymuint n = mCellArray.size();
  for (ymuint i = 0; i < n; ++ i) {
    delete mCellArray[i];
  }
  mCellArray.clear();

  for (ymuint i = 0; i < mHashSize; ++ i) {
    mHashTable[i] = NULL;
  }
}


BEGIN_NONAMESPACE

// 組み込み型のチェック
tTgGateType
check_builtin(const Expr& lexp,
	      ymuint ni,
	      const TvFunc& tmp_func)
{
  if ( lexp.is_zero() ) {
    return kTgGateConst0;
  }
  if ( lexp.is_one() ) {
    return kTgGateConst1;
  }
  if ( lexp.is_posiliteral() ) {
    ASSERT_COND(lexp.varid().val() == 0 );
    return kTgGateBuff;
  }
  if ( lexp.is_negaliteral() ) {
    ASSERT_COND(lexp.varid().val() == 0 );
    return kTgGateNot;
  }
  ymuint np = 1 << ni;

  bool and_flag = true;
  bool nand_flag = true;
  bool or_flag = true;
  bool nor_flag = true;
  bool xor_flag = true;
  bool xnor_flag = true;
  for (ymuint p = 0; p < np; ++ p) {
    bool bit = static_cast<bool>(tmp_func.value(p));
    if ( and_flag || nand_flag ) {
      if ( p < (np - 1) ) {
	and_flag &= !bit;
	nand_flag &= bit;
      }
      else {
	and_flag &= bit;
	nand_flag &= !bit;
      }
    }
    if ( or_flag || nor_flag ) {
      if ( p == 0UL ) {
	or_flag &= !bit;
	nor_flag &= bit;
      }
      else {
	or_flag &= bit;
	nor_flag &= !bit;
      }
    }
    if ( xor_flag || xnor_flag ) {
      ymuint n1 = 0;
      for (ymuint i = 0; i < ni; ++ i) {
	if ( p & (1UL << i) ) {
	  ++ n1;
	}
      }
      if ( n1 & 1 ) {
	xor_flag &= bit;
	xnor_flag &= !bit;
      }
      else {
	xor_flag &= !bit;
	xnor_flag &= bit;
      }
    }
  }

  if ( and_flag ) {
    return kTgGateAnd;
  }
  if ( nand_flag ) {
    return kTgGateNand;
  }
  if ( or_flag ) {
    return kTgGateOr;
  }
  if ( nor_flag ) {
    return kTgGateNor;
  }
  if ( xor_flag ) {
    return kTgGateXor;
  }
  if ( xnor_flag ) {
    return kTgGateXnor;
  }
  return kTgGateCplx;
}

END_NONAMESPACE


// @brief 新しい論理式を登録する．
// @param[in] lexp 論理式
// @param[out] id ID番号
// @return 論理関数の型を返す．
tTgGateType
LogicMgr::reg_logic(const Expr& lexp,
		    ymuint32& id)
{
  // 真理値ベクタに変換する．
  TvFunc tmp_func = lexp.make_tv();
  ymuint ni = lexp.input_size();

  // 組み込み型のチェック
  tTgGateType type = check_builtin(lexp, ni, tmp_func);

  if ( type != kTgGateCplx ) {
    return type;
  }

  // ハッシュ表から同一の関数がないか調べる．
  ymuint pos0 = tmp_func.hash();
  ymuint pos = pos0 % mHashSize;
  for (Cell* cell = mHashTable[pos]; cell; cell = cell->mLink) {
    if ( cell->mTvFunc == tmp_func ) {
      // 同じ関数が登録されていた．
      // もしもリテラル数が少なかったら論理式を変更する．
      if ( cell->mExpr.litnum() > lexp.litnum() ) {
	cell->mExpr = lexp;
      }
      id = cell->mId;
      return kTgGateCplx;
    }
  }

  if ( logic_num() > mNextLimit ) {
    // ハッシュ表を拡大する．
    expand();
    pos = pos0 % mHashSize;
  }

  Cell* new_cell = new Cell;
  new_cell->mId = logic_num();
  new_cell->mExpr = lexp;
  new_cell->mTvFunc = tmp_func;
  new_cell->mLink = mHashTable[pos];
  mCellArray.push_back(new_cell);
  mHashTable[pos] = new_cell;
  id = new_cell->mId;
  return kTgGateCplx;
}

// ハッシュ表を拡大して再ハッシュする．
void
LogicMgr::expand()
{
  ymuint old_size = mHashSize;
  Cell** old_table = mHashTable;
  alloc_table(mHashSize << 1);
  for (ymuint i = 0; i < old_size; ++ i) {
    Cell* next = NULL;
    for (Cell* cell = old_table[i]; cell; cell = next) {
      next = cell->mLink;
      ymuint pos1 = cell->mTvFunc.hash() % mHashSize;
      cell->mLink = mHashTable[pos1];
      mHashTable[pos1] = cell;
    }
  }
  delete [] old_table;
}

// ハッシュ表用の領域を確保する．
void
LogicMgr::alloc_table(ymuint req_size)
{
  mHashSize = req_size;
  mHashTable = new Cell*[mHashSize];
  for (ymuint i = 0; i < mHashSize; ++ i) {
    mHashTable[i] = NULL;
  }
  mNextLimit = static_cast<ymuint>(mHashSize * kHashCapacity);
}

END_NAMESPACE_YM_NETWORKS_TGNET
