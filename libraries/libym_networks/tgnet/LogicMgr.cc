
/// @file LogicMgr.cc
/// @brief LogicMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013 Yusuke Matsunaga
/// All rights reserved.


#include "LogicMgr.h"


BEGIN_NAMESPACE_YM_NETWORKS_TGNET

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

// @brief 登録されている論理式の数を返す．
ymuint
LogicMgr::num() const
{
  return mCellArray.size();
}

// @brief 論理式を取り出す．
// @param[in] gate_type ゲートタイプ
// @param[in] ni 入力数
LogExpr
LogicMgr::get_expr(tTgGateType type,
		   ymuint ni) const
{
  if ( static_cast<ymuint32>(type) < kBase ) {
    vector<LogExpr> literals(ni);
    for (ymuint i = 0; i < ni; ++ i) {
      literals[i] = LogExpr::make_posiliteral(VarId(i));
    }
    switch ( type ) {
    case kTgBuff:
      return literals[0];

    case kTgNot:
      return ~literals[0];

    case kTgAnd:
      for (ymuint i = 1; i < ni; ++ i) {
	literals[0] &= literals[i];
      }
      return literals[0];

    case kTgNand:
      for (ymuint i = 1; i < ni; ++ i) {
	literals[0] &= literals[i];
      }
      return ~literals[0];

    case kTgOr:
      for (ymuint i = 1; i < ni; ++ i) {
	literals[0] |= literals[i];
      }
      return literals[0];

    case kTgNor:
      for (ymuint i = 1; i < ni; ++ i) {
	literals[0] |= literals[i];
      }
      return ~literals[0];

    case kTgXor:
      for (ymuint i = 1; i < ni; ++ i) {
	literals[0] ^= literals[i];
      }
      return literals[0];

    case kTgXnor:
      for (ymuint i = 1; i < ni; ++ i) {
	literals[0] ^= literals[i];
      }
      return ~literals[0];

    default:
      assert_not_reached(__FILE__, __LINE__);
      return LogExpr(); // ダミー
    }
  }
  else {
    return mCellArray[type - kBase]->mLexp;
  }
}

// @brief 論理関数を返す．
// @param[in] gate_type ゲートタイプ
// @param[in] ni 入力数
const TvFunc&
LogicMgr::get_func(tTgGateType gate_type,
		   ymuint ni) const
{
  // 組み込み型のためのスタティックオブジェクト
  static TvFunc builtin_func;
  if ( static_cast<ymuint32>(gate_type) < kBase ) {
    builtin_func = TvFunc::posi_literal(ni, VarId(0));

    switch ( gate_type ) {
    case kTgBuff:
      break;

    case kTgNot:
      builtin_func = ~builtin_func;
      break;

    case kTgAnd:
      for (ymuint i = 1; i < ni; ++ i) {
	builtin_func &= TvFunc::posi_literal(ni, VarId(i));
      }
      break;

    case kTgNand:
      for (ymuint i = 1; i < ni; ++ i) {
	builtin_func &= TvFunc::posi_literal(ni, VarId(i));
      }
      builtin_func = ~builtin_func;
      break;

    case kTgOr:
      for (ymuint i = 1; i < ni; ++ i) {
	builtin_func |= TvFunc::posi_literal(ni, VarId(i));
      }
      break;

    case kTgNor:
      for (ymuint i = 1; i < ni; ++ i) {
	builtin_func |= TvFunc::posi_literal(ni, VarId(i));
      }
      builtin_func = ~builtin_func;
      break;

    case kTgXor:
      for (ymuint i = 1; i < ni; ++ i) {
	builtin_func ^= TvFunc::posi_literal(ni, VarId(i));
      }
      break;

    case kTgXnor:
      for (ymuint i = 1; i < ni; ++ i) {
	builtin_func ^= TvFunc::posi_literal(ni, VarId(i));
      }
      builtin_func = ~builtin_func;
      break;

    default:
      assert_not_reached(__FILE__, __LINE__);
      break;
    }
    return builtin_func;
  }
  else {
    return mCellArray[gate_type - kBase]->mTvFunc;
  }
}


BEGIN_NONAMESPACE

// 組み込み型のチェック
tTgGateType
check_builtin(const LogExpr& lexp,
	      ymuint ni,
	      const TvFunc& tmp_func)
{
  if ( lexp.is_posiliteral() ) {
    assert_cond(lexp.varid().val() == 0, __FILE__, __LINE__);
    return kTgBuff;
  }
  if ( lexp.is_negaliteral() ) {
    assert_cond(lexp.varid().val() == 0, __FILE__, __LINE__);
    return kTgNot;
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
    return kTgAnd;
  }
  if ( nand_flag ) {
    return kTgNand;
  }
  if ( or_flag ) {
    return kTgOr;
  }
  if ( nor_flag ) {
    return kTgNor;
  }
  if ( xor_flag ) {
    return kTgXor;
  }
  if ( xnor_flag ) {
    return kTgXnor;
  }
  return kTgUndef;
}

END_NONAMESPACE


// @brief 新しい論理式を登録する．
ymuint32
LogicMgr::reg_logic(const LogExpr& lexp)
{
  // 真理値ベクタに変換する．
  TvFunc tmp_func = lexp.make_tv();
  ymuint ni = lexp.input_size();

  // 組み込み型のチェック
  tTgGateType type = check_builtin(lexp, ni, tmp_func);

  if ( type != kTgUndef ) {
    return pack(type, ni);
  }

  // ハッシュ表から同一の関数がないか調べる．
  ymuint pos0 = tmp_func.hash();
  ymuint pos = pos0 % mHashSize;
  for (Cell* cell = mHashTable[pos]; cell; cell = cell->mLink) {
    if ( LogicMgr::ni(cell->mId) == ni && cell->mTvFunc == tmp_func ) {
      // 同じ関数が登録されていた．
      // もしもリテラル数が少なかったら論理式を変更する．
      if ( cell->mLexp.litnum() > lexp.litnum() ) {
	cell->mLexp = lexp;
      }
      return cell->mId;
    }
  }

  if ( num() > mNextLimit ) {
    // ハッシュ表を拡大する．
    expand();
    pos = pos0 % mHashSize;
  }

  Cell* new_cell = new Cell;
  type = static_cast<tTgGateType>(mCellArray.size() + kBase);
  new_cell->mId = pack(type, ni);
  new_cell->mLexp = lexp;
  new_cell->mTvFunc = tmp_func;
  new_cell->mLink = mHashTable[pos];
  mCellArray.push_back(new_cell);
  mHashTable[pos] = new_cell;
  return new_cell->mId;
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

// @brief デバッグ用の関数
// @param[in] s 出力先のストリーム
void
LogicMgr::dump(ostream& s) const
{
  s << "=====<LogicMgr Dump Start>=====" << endl;
  ymuint n = mCellArray.size();
  for (ymuint i = 0; i < n; ++ i) {
    Cell* tmp = mCellArray[i];
    tTgGateType type = LogicMgr::type(tmp->mId);
    ymuint ni = LogicMgr::ni(tmp->mId);
    switch ( type ) {
    case kTgUndef:  s << "--UNDEF--"; break;

    case kTgInput:  s << "--INPUT--"; break;
    case kTgOutput: s << "--OUTPUT--"; break;

    case kTgConst0: s << "--CONST0--"; break;
    case kTgConst1: s << "--CONST1--"; break;

    case kTgBuff:   s << "--BUFF--"; break;
    case kTgNot:    s << "--NOT--"; break;

    case kTgAnd:    s << "--AND(" << ni << ")--"; break;

    case kTgNand:   s << "--NAND(" << ni << ")--"; break;

    case kTgOr:     s << "--OR(" << ni << ")--"; break;

    case kTgNor:    s << "--NOR(" << ni << ")--"; break;

    case kTgXor:    s << "--XOR(" << ni << ")--"; break;

    case kTgXnor:   s << "--XNOR(" << ni << ")--"; break;

    default:
      if ( static_cast<ymuint32>(type) >= static_cast<ymuint32>(kTgUsrDef) ) {
	s << "--CPLX(" << ni << ")--"; break;
      }
      else {
	s << "--ERROR--"; break;
      }
    }

    s << "\t: " << tmp->mLexp << endl;
  }
  s << "=====< LogicMgr Dump End >=====" << endl;
}

END_NAMESPACE_YM_NETWORKS_TGNET
