#ifndef BNODEMGR_H
#define BNODEMGR_H

/// @file BNodeMgr.h
/// @brief BNodeMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmNetworks/BNetwork.h"
#include "YmUtils/UnitAlloc.h"
#include "YmUtils/FragAlloc.h"


BEGIN_NAMESPACE_YM_NETWORKS_BNET

//////////////////////////////////////////////////////////////////////
/// @class BNodeMgr BNodeMgr.h "BNodeMgr.h"
/// @brief BNode を管理するクラス
//////////////////////////////////////////////////////////////////////
class BNodeMgr
{
public:

  /// @brief 唯一のオブジェクトを取り出す．
  static
  BNodeMgr&
  the_obj();


private:

  /// @brief コンストラクタ．
  BNodeMgr();

  /// @brief デストラクタ
  ~BNodeMgr();


public:

  /// @brief BNode を確保する．
  BNode*
  alloc_node();

  /// @brief BNode を削除する．
  void
  free_node(BNode* node);

  /// @brief BNode の名前用の文字列領域を確保する．
  const char*
  alloc_name(const char* src);

  /// @brief alloc_name() で確保した文字列領域を開放する．
  void
  free_name(const char* src);

  /// @brief BNodeEdge の配列を確保する．
  BNodeEdge*
  alloc_edgearray(size_t n);

  /// @brief BNodeEdge の配列を削除する．
  void
  free_edgearray(size_t n,
		 BNodeEdge* array);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // BNode 用の領域を確保するクラス
  UnitAlloc mNodeAlloc;

  // 文字列領域を確保するクラス
  FragAlloc mNameAlloc;

  // BNodeEdge の配列用の領域を確保するクラス
  FragAlloc mEdgeAlloc;

};

END_NAMESPACE_YM_NETWORKS_BNET

#endif // BNODEMGR_H
