#ifndef LIBYM_NETWORKS_CMN_CMNNODELOGIC_H
#define LIBYM_NETWORKS_CMN_CMNNODELOGIC_H

/// @file libym_netwroks/cmn/CmnNodeLogic.h
/// @brief CmnNodeLogic のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_networks/CmnNode.h"


BEGIN_NAMESPACE_YM_CMN

//////////////////////////////////////////////////////////////////////
/// @class CmnNodeLogic CmnNodeLogic.h "CmnNodeLogic.h"
/// @brief 論理ノードを表す CmnNode の派生クラス
//////////////////////////////////////////////////////////////////////
class CmnNodeLogic :
  public CmnNode
{
  friend class CmnMgrImpl;

private:

  /// @brief コンストラクタ
  /// @param[in] ni 入力数
  /// @param[in] fanins ファンインの配列
  /// @param[in] cell セル
  CmnNodeLogic(ymuint ni,
	       CmnEdge* fanins,
	       const Cell* cell);

  /// @brief デストラクタ
  virtual
  ~CmnNodeLogic();


public:

  /// @brief タイプを得る．
  virtual
  tType
  type() const;

  /// @brief 論理ノードの時に true を返す．
  virtual
  bool
  is_logic() const;

  /// @brief 入力数を得る．
  virtual
  ymuint
  ni() const;

  /// @brief ファンインのノードを得る．
  /// @param[in] pos 入力番号
  /// @return pos 番めのファンインのノード
  /// @note 該当するファンインがなければ NULL を返す．
  virtual
  const CmnNode*
  fanin(ymuint pos) const;

  /// @brief ファンインの枝を得る．
  /// @param[in] pos 入力番号
  /// @return pos 番目の入力の枝
  /// @note 該当するファンインの枝がなければ NULL を返す．
  virtual
  const CmnEdge*
  fanin_edge(ymuint pos) const;

  /// @brief セルを得る．
  virtual
  const Cell*
  cell() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 入力数
  ymuint32 mNi;

  // ファンインの枝の配列
  CmnEdge* mFanins;

  // セル
  const Cell* mCell;

};

END_NAMESPACE_YM_CMN

#endif // LIBYM_NETWORKS_CMN_CMNNODELOGIC_H
