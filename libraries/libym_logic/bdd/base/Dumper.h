#ifndef DUMPER_H
#define DUMPER_H

/// @file Dumper.h
/// @brief Dumper のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "BddMgrImpl.h"
#include "IdMgr.h"
#include "ym_utils/BinO.h"


BEGIN_NAMESPACE_YM_BDD

//////////////////////////////////////////////////////////////////////
// BDD の内容を保存するためのクラス
//////////////////////////////////////////////////////////////////////
class Dumper
{
public:

  /// @brief コンストラクタ
  /// @param[in] mgr BddMgr
  /// @param[in] s 出力先のストリーム
  Dumper(BddMgrImpl* mgr,
	 BinO& s);

  /// @brief デストラクタ
  ~Dumper();


public:

  /// @brief 複数のBDDをダンプする．
  void
  write(const vector<BddEdge>& edge_list);


private:
  //////////////////////////////////////////////////////////////////////
  // 下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief e で指されたBDDノードの内容を出力する．
  void
  dump_node(BddEdge e);

  /// @brief e の内容を出力する．
  void
  dump_edge(BddEdge e);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // BddMgrImpl へのポインタ
  BddMgrImpl* mMgr;

  // 出力用のストリーム
  BinO& mStream;

  // ID 番号を管理するマネージャ
  IdMgr mIdMgr;

};

END_NAMESPACE_YM_BDD

#endif
