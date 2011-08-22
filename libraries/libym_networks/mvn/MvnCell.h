#ifndef MVNCELL_H
#define MVNCELL_H

/// @file MvnCell.h
/// @brief MvnCell のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "MvnNodeBase.h"


BEGIN_NAMESPACE_YM_NETWORKS_MVN

//////////////////////////////////////////////////////////////////////
/// @calss MvnCell MvnCell.h "MvnCell.h"
/// @brief セルインスタンスを表すクラス
//////////////////////////////////////////////////////////////////////
class MvnCell :
  public MvnNodeBase
{
  friend class MvnMgr;

private:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ / デストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ
  /// @param[in] module 親のモジュール
  /// @param[in] cell セル
  /// @param[in] ni 入力ピン数
  /// @param[in] pin_pos ピン位置を収めた配列
  MvnCell(MvnModule* module,
	  const Cell* cell,
	  ymuint ni,
	  const vector<ymuint32>& pin_pos);

  /// @brief デストラクタ
  virtual
  ~MvnCell();


public:
  //////////////////////////////////////////////////////////////////////
  // 情報を参照するための関数
  //////////////////////////////////////////////////////////////////////

  /// @brief セルを得る．
  /// @note type() が kCell の時のみ意味を持つ．
  virtual
  const Cell*
  cell() const;

  /// @brief セルの入力ピン番号を返す．
  /// @param[in] pos ノードの入力番号 ( 0 <= pos < input_num() )
  /// @note type() が kCell の時のみ意味を持つ．
  virtual
  ymuint
  cell_ipin_pos(ymuint pos) const;

  /// @brief セルの出力ピン番号を返す．
  /// @note type() が kCell の時のみ意味を持つ．
  virtual
  ymuint
  cell_opin_pos() const;

  /// @brief 多出力セルノードの場合の代表ノードを返す．
  /// @note type() が kCell の時のみ意味を持つ．
  /// @note 1出力セルノードの時には自分自身を返す．
  virtual
  const MvnNode*
  cell_node() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // セル
  const Cell* mCell;

};

END_NAMESPACE_YM_NETWORKS_MVN

#endif // MVNCELL_H
