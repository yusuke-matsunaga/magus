#ifndef DTPGNGMGR_H
#define DTPGNGMGR_H

/// @file DtpgNgMgr.h
/// @brief DtpgNgMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2014 Yusuke Matsunaga
/// All rights reserved.


#include "satpg_nsdef.h"
#include "utils/SimpleAlloc.h"


#define USE_STL_LIST 0

BEGIN_NAMESPACE_YM_SATPG

class DtpgNodeGroup;
class DtpgNgEdge;

//////////////////////////////////////////////////////////////////////
/// @class DtpgNgMgr DtpgNgMgr.h "DtpgNgMgr.h"
/// @brief DtpgNode, DtpgNgEdge を管理するクラス
//////////////////////////////////////////////////////////////////////
class DtpgNgMgr
{
public:

  /// @brief コンストラクタ
  DtpgNgMgr();

  /// @brief デストラクタ
  ~DtpgNgMgr();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ノードを作る．
  DtpgNodeGroup*
  add_node_group(const vector<TpgNode*>& node_list);

  /// @brief 枝を作る．
  void
  add_edge(DtpgNodeGroup* src_node,
	   DtpgNodeGroup* dst_node);

  /// @brief 制限値を越えない範囲でマージする．
  void
  merge(ymuint size_limit);

  /// @brief ノードリストを返す．
  const vector<DtpgNodeGroup*>&
  node_list() const;


private:
  //////////////////////////////////////////////////////////////////////
  // 下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ノードをマージする．
  void
  merge_node(DtpgNgEdge* edge);

  /// @brief 枝を削除する．
  void
  delete_edge(DtpgNgEdge* edge);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  SimpleAlloc mAlloc;

  // ノードのリスト
  vector<DtpgNodeGroup*> mNodeList;

#if USE_STL_LIST
  // 枝のリスト
  vector<DtpgNgEdge*> mEdgeList;
#else
  DtpgNgEdge* mTop;
  DtpgNgEdge* mTail;
#endif

};

END_NAMESPACE_YM_SATPG

#endif // DTPGNGMGR_H
