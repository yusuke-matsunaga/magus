#ifndef LOGICMGR_H
#define LOGICMGR_H

/// @file LogicMgr.h
/// @brief LogicMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_networks/tgnet.h"
#include "ym_logic/LogExpr.h"
#include "ym_logic/TvFunc.h"


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
  void
  clear();

  /// @brief 新しい論理式を登録する．
  /// @param[in] lexp 論理式
  /// @return ID番号を返す．
  ymuint32
  reg_logic(const LogExpr& lexp);

  /// @brief 登録されている論理式の数を返す．
  ymuint
  num() const;

  /// @brief 論理式を返す．
  /// @param[in] gate_type ゲートタイプ
  /// @param[in] ni 入力数
  /// @note 組み込み型の場合の効率はあまりよくない．
  LogExpr
  get_expr(tTgGateType gate_type,
	   ymuint ni) const;

  /// @brief 論理式を返す．
  /// @param[in] id ID番号
  /// @note 組み込み型の場合の効率はあまりよくない．
  LogExpr
  get_expr(ymuint32 id) const;

  /// @brief 論理関数を返す．
  /// @param[in] gate_type ゲートタイプ
  /// @param[in] ni 入力数
  /// @note 組み込み型の場合の効率はあまりよくない．
  const TvFunc&
  get_func(tTgGateType gate_type,
	   ymuint ni) const;

  /// @brief 論理関数を返す．
  /// @param[in] id ID番号
  /// @note 組み込み型の場合の効率はあまりよくない．
  const TvFunc&
  get_func(ymuint32 id) const;


public:
  //////////////////////////////////////////////////////////////////////
  // reg_logic() の返り値の変換用関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ゲートタイプと入力数から ID番号を作る．
  /// @param[in] type ゲートタイプ
  /// @param[in] ni 入力数
  static
  ymuint32
  pack(tTgGateType type,
       ymuint ni);

  /// @brief ゲートタイプを得る．
  /// @param[in] id ID番号
  static
  tTgGateType
  type(ymuint32 id);

  /// @brief 入力数を得る．
  /// @param[in] id ID番号
  static
  ymuint
  ni(ymuint32 id);


public:
  //////////////////////////////////////////////////////////////////////
  // デバッグ用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内部データを出力する．
  /// @param[in] s 出力先のストリーム
  void
  dump(ostream& s) const;


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
    LogExpr mLexp;

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
  const double kHashCapacity = 1.8;

  // 一般の論理関数のオフセット値
  static
  const ymuint32 kBase = static_cast<ymuint32>(kTgUsrDef);

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

// @brief 論理式を返す．
// @param[in] id ID番号
inline
LogExpr
LogicMgr::get_expr(ymuint32 id) const
{
  return get_expr(type(id), ni(id));
}

// @brief 論理関数を返す．
// @param[in] id ID番号
inline
const TvFunc&
LogicMgr::get_func(ymuint32 id) const
{
  return get_func(type(id), ni(id));
}

// @brief データを圧縮する．
inline
ymuint32
LogicMgr::pack(tTgGateType type,
	       ymuint ni)
{
  return (static_cast<ymuint32>(type) << 12) | static_cast<ymuint32>(ni);
}

// @brief ゲートタイプを得る．
inline
tTgGateType
LogicMgr::type(ymuint32 data)
{
  return static_cast<tTgGateType>(data >> 12);
}

// @brief 入力数を得る．
inline
ymuint
LogicMgr::ni(ymuint32 data)
{
  return static_cast<ymuint>(data & 0xFFF);
}


END_NAMESPACE_YM_NETWORKS_TGNET

#endif // LOGICMGR_H
