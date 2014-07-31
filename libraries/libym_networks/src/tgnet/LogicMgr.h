#ifndef LOGICMGR_H
#define LOGICMGR_H

/// @file LogicMgr.h
/// @brief LogicMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmNetworks/tgnet.h"
#include "YmLogic/Expr.h"
#include "YmLogic/TvFunc.h"


BEGIN_NAMESPACE_YM_NETWORKS_TGNET

//////////////////////////////////////////////////////////////////////
/// @class LogicMgr LogicMgr.h "LogicMgr.h"
/// @brief logic ノードのタイプ番号を管理するクラス
///
/// 論理式を登録するが，内部で論理関数に変換している．
/// 同一の論理関数を表す論理式が登録された場合には
/// リテラル数の少ない論理式を記録する．
/// BUF/NOTAND/NAND/OR/NOR/XOR/XNOR の関数はハッシュを調べる前に
/// 決められたID番号を返す．
//////////////////////////////////////////////////////////////////////
class LogicMgr
{
public:

  /// @brief コンストラクタ
  LogicMgr();

  /// @brief デストラクタ
  ~LogicMgr();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 初期化する．
  /// @note 登録されている関数がない状態になる．
  void
  clear();

  /// @brief 新しい論理式を登録する．
  /// @param[in] lexp 論理式
  /// @param[out] id kTgGateCplx の場合はID番号を格納する．
  /// @return 論理関数の型を返す．
  tTgGateType
  reg_logic(const Expr& lexp,
	    ymuint32& id);

  /// @brief 登録されている論理式の数を返す．
  ymuint32
  logic_num() const;

  /// @brief 論理式を返す．
  /// @param[in] id ID番号 ( 0 <= id < logic_num() )
  Expr
  get_expr(ymuint32 id) const;

  /// @brief 論理関数を返す．
  /// @param[in] id ID番号 ( 0 <= id < logic_num() )
  const TvFunc&
  get_func(ymuint32 id) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  // ハッシュ表を拡大して再ハッシュする．
  void
  expand();

  // ハッシュ表用の領域を確保する．
  void
  alloc_table(ymuint req_size);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いるデータ構造
  //////////////////////////////////////////////////////////////////////

  // 一つの要素を表すセル
  struct Cell
  {
    // ID番号
    ymuint32 mId;

    // 論理式
    Expr mExpr;

    // 真理値表
    TvFunc mTvFunc;

    // 次のセルを指すリンク
    Cell* mLink;
  };


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ハッシュ表を拡大するときに使われる定数
  static
  constexpr double kHashCapacity = 1.8;

  // Cell の配列
  vector<Cell*> mCellArray;

  // ハッシュ表のサイズ
  ymuint32 mHashSize;

  // ハッシュ表
  Cell** mHashTable;

  // ハッシュ表を拡大する目安
  ymuint32 mNextLimit;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 登録されている論理式の数を返す．
inline
ymuint
LogicMgr::logic_num() const
{
  return mCellArray.size();
}

// @brief 論理式を返す．
// @param[in] id ID番号 ( 0 <= id < logic_num() )
inline
Expr
LogicMgr::get_expr(ymuint32 id) const
{
  assert_cond( id < logic_num(), __FILE__, __LINE__);
  return mCellArray[id]->mExpr;
}

// @brief 論理関数を返す．
// @param[in] id ID番号 ( 0 <= id < logic_num() )
inline
const TvFunc&
LogicMgr::get_func(ymuint32 id) const
{
  assert_cond( id < logic_num(), __FILE__, __LINE__);
  return mCellArray[id]->mTvFunc;
}

END_NAMESPACE_YM_NETWORKS_TGNET

#endif // LOGICMGR_H
