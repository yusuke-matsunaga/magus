#ifndef BNODEMAP_H
#define BNODEMAP_H

/// @file BNodeMap.h
/// @brief BNodeMap のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "YmNetworks/bnet.h"


BEGIN_NAMESPACE_YM_NETWORKS

class ImpNodeHandle;
class ImpNode;

//////////////////////////////////////////////////////////////////////
/// @class BNodeMap BNodeMap.h "BNodeMap.h"
/// @brief BNode と ImpNode の対応付けを行なうクラス
//////////////////////////////////////////////////////////////////////
class BNodeMap
{
public:

  /// @brief コンストラクタ
  BNodeMap();

  /// @brief デストラクタ
  ~BNodeMap();


public:
  //////////////////////////////////////////////////////////////////////
  // 情報を取り出す関数
  //////////////////////////////////////////////////////////////////////

  /// @brief BNode 番号の最大値 + 1 を得る．
  ymuint
  max_bnode_id() const;

  /// @brief BNode 番号に対応するハンドルを得る．
  /// @param[in] bnode_id BNode のノード番号 ( 0 <= id < max_bnode_id() )
  /// @note 場合によっては NULL のハンドルが返ることもある．
  ImpNodeHandle
  bnode_handle(ymuint bnode_id) const;

  /// @brief ImpNode に対応する BNode 番号を得る．
  /// @param[in] id ImpNode の番号
  /// @param[out] bnode_id 対応する BNode 番号
  /// @param[out] inv 反転属性
  /// @retval true 対応する BNode があった．
  /// @retval false 対応する BNode がなかった．
  bool
  get_bnode(ymuint id,
	    ymuint& bnode_id,
	    bool& inv) const;


public:
  //////////////////////////////////////////////////////////////////////
  // 情報を設定する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を空にする．
  void
  clear();

  /// @brief BNode 番号の最大値を設定する．
  /// @param[in] size BNode 番号の最大値 + 1
  void
  set_bnode_size(ymuint size);

  /// @brief BNode と ImpNode の対応付けを行なう．
  /// @param[in] bnode_id BNode 番号
  /// @param[in] node 対応する ImpNode
  /// @param[in] inv 反転属性
  void
  bind(ymuint bnode_id,
       ImpNode* node,
       bool inv);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // BNode 番号をキーにして ImpHandle を納めた配列
  vector<ImpNodeHandle> mBNodeMap;

  // ImpNode 番号をキーにして BNode 番号+反転属性を納めた配列
  vector<ymuint32> mImpNodeMap;

};

END_NAMESPACE_YM_NETWORKS


#endif // BNODEMAP_H
