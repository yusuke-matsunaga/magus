#ifndef LIBYM_NETWORKS_MVN_MVNCELL_H
#define LIBYM_NETWORKS_MVN_MVNCELL_H

/// @file libym_networks/mvn/MvnCell.h
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
  /// @param[in] no 出力ピン数
  /// @param[in] pin_pos ピン位置を収めた配列
  MvnCell(MvnModule* module,
	  const Cell* cell,
	  ymuint ni,
	  ymuint no,
	  const vector<ymuint32>& pin_pos);

  /// @brief デストラクタ
  virtual
  ~MvnCell();


public:
  //////////////////////////////////////////////////////////////////////
  // 情報を参照するための関数
  //////////////////////////////////////////////////////////////////////

  /// @brief セルを得る．
  /// @note type() が kCell の時のみ意味をモツ．
  virtual
  const Cell*
  cell() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // セル
  const Cell* mCell;

};

END_NAMESPACE_YM_NETWORKS_MVN

#endif // LIBYM_NETWORKS_MVN_MVNCELL_H
