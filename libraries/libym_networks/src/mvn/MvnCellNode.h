#ifndef MVNCELLNODE_H
#define MVNCELLNODE_H

/// @file MvnCellNode.h
/// @brief MvnCellNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "MvnNodeBase.h"


BEGIN_NAMESPACE_YM_NETWORKS_MVN

//////////////////////////////////////////////////////////////////////
/// @calss MvnCellNode MvnCellNode.h "MvnCellNode.h"
/// @brief セルインスタンスを表すクラス
//////////////////////////////////////////////////////////////////////
class MvnCellNode :
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
  MvnCellNode(MvnModule* module,
	      const Cell* cell);

  /// @brief デストラクタ
  virtual
  ~MvnCellNode();


public:
  //////////////////////////////////////////////////////////////////////
  // 情報を参照するための関数
  //////////////////////////////////////////////////////////////////////

  /// @brief セルを得る．
  /// @note type() が kCell の時のみ意味を持つ．
  virtual
  const Cell*
  cell() const;

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


//////////////////////////////////////////////////////////////////////
/// @class MvnExtCellNode MvnCellNode.h "MvnCellNode.h"
/// @brief 2つめ以降の出力ピンを表すクラス
//////////////////////////////////////////////////////////////////////
class MvnExtCellNode :
  public MvnNodeBase
{
  friend class MvnMgr;

private:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ / デストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ
  /// @param[in] module 親のモジュール
  /// @param[in] cell_node 代表ノード
  /// @param[in] opos 出力ピン番号
  MvnExtCellNode(MvnModule* module,
		 MvnNode* cell_node,
		 ymuint opos);

  /// @brief デストラクタ
  virtual
  ~MvnExtCellNode();


public:
  //////////////////////////////////////////////////////////////////////
  // 情報を参照するための関数
  //////////////////////////////////////////////////////////////////////

  /// @brief セルを得る．
  /// @note type() が kCell の時のみ意味を持つ．
  virtual
  const Cell*
  cell() const;

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

  // 代表ノード
  MvnNode* mCellNode;

  // 出力ピン番号
  ymuint32 mOpos;

};

END_NAMESPACE_YM_NETWORKS_MVN

#endif // MVNCELLNODE_H
