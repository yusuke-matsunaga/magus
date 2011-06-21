#ifndef YM_NETWORKS_CMNEDGE_H
#define YM_NETWORKS_CMNEDGE_H

/// @file ym_networks/CmnEdge.h
/// @brief CmnEdge のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: LutNetwork.h 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_networks/cmn.h"
#include "ym_cell/cell_nsdef.h"

#include "ym_utils/DlList.h"


BEGIN_NAMESPACE_YM_NETWORKS

//////////////////////////////////////////////////////////////////////
/// @class CmnEdge CmnEdge.h "ym_networks/CmnEdge.h"
/// @brief CELLネットワークの枝を表すクラス
///
/// - 入力側のノード
/// - 出力側のノード
/// - 出力側のノードの何番目の入力か
/// の情報を持つ．
/// @sa CmnNode CmnMgr
//////////////////////////////////////////////////////////////////////
class CmnEdge :
  public DlElem
{
  friend class CmnNodeOutput;
  friend class CmnNodeLogic;
  friend class CmnMgrImpl;

public:

  //////////////////////////////////////////////////////////////////////
  /// @name 関数処理に関する情報にアクセスする関数
  /// @{

  /// @brief 入力側のノードを得る．(const 版)
  /// @retval 入力側のノード 通常の枝の場合
  /// @retval NULL 定数0に接続している枝の場合
  const CmnNode*
  from() const;

  /// @brief 入力側のノードを得る．
  /// @retval 入力側のノード 通常の枝の場合
  /// @retval NULL 定数0に接続している枝の場合
  CmnNode*
  from();

  /// @brief 出力側のノードを得る．(const 版)
  /// @return 出力側のノードを返す．
  const CmnNode*
  to() const;

  /// @brief 出力側のノードを得る．
  /// @return 出力側のノードを返す．
  CmnNode*
  to();

  /// @brief 出力側のノードの何番目の入力かを示す．
  ymuint
  to_pos() const;

  /// @}
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // プライベートメンバ関数
  // これらは friend class からしか呼ぶことができない．
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ
  CmnEdge();

  /// @brief デストラクタ
  ~CmnEdge();

  /// @brief from ノードをセットする．
  /// @param[in] from 入力元のノード
  void
  set_from(CmnNode* from);

  /// @brief to ノードをセットする．
  /// @param[in] to 出力先のノード
  /// @param[in] pos 出力のノードの入力ピン番号
  void
  set_to(CmnNode* to,
	 ymuint pos);


private:
  //////////////////////////////////////////////////////////////////////
  // プライベートメンバ変数
  //////////////////////////////////////////////////////////////////////

  // 入力側のノード
  CmnNode* mFrom;

  // 出力側のノード
  CmnNode* mTo;

  // 出力側のノードの入力ピン番号
  ymuint8 mToPos;

};


//////////////////////////////////////////////////////////////////////
// inline 関数の定義
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// クラス CmnEdge
//////////////////////////////////////////////////////////////////////

// コンストラクタ
inline
CmnEdge::CmnEdge() :
  mFrom(NULL),
  mTo(NULL),
  mToPos(0)
{
}

// デストラクタ
inline
CmnEdge::~CmnEdge()
{
}

// 入力側のノードを得る．
inline
const CmnNode*
CmnEdge::from() const
{
  return mFrom;
}

// 入力側のノードを得る．
inline
CmnNode*
CmnEdge::from()
{
  return mFrom;
}

// 出力側のノードを得る．
inline
const CmnNode*
CmnEdge::to() const
{
  return mTo;
}

// 出力側のノードを得る．
inline
CmnNode*
CmnEdge::to()
{
  return mTo;
}

// 出力側のノードの何番目の入力かを示す．
inline
ymuint
CmnEdge::to_pos() const
{
  return mToPos;
}

// @brief from ノードをセットする．
// @param[in] from 入力元のノード
inline
void
CmnEdge::set_from(CmnNode* from)
{
  mFrom = from;
}

// @brief to ノードをセットする．
// @param[in] to 出力先のノード
// @param[in] pos 出力のノードの入力ピン番号
inline
void
CmnEdge::set_to(CmnNode* to,
		ymuint pos)
{
  mTo = to;
  mToPos = pos;
}

END_NAMESPACE_YM_NETWORKS

#endif // YM_NETWORKS_CMNEDGE_H
