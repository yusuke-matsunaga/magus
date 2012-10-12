#ifndef DUMPER_H
#define DUMPER_H

/// @file Dumper.h
/// @brief Dumper のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "CNFddMgrImpl.h"
#include "IdMgr.h"
#include "ym_utils/BinO.h"


BEGIN_NAMESPACE_YM_CNFDD

//////////////////////////////////////////////////////////////////////
/// @class Dumper Dumper.h "Dumper.h"
/// @brief ZDD の内容を保存するためのクラス
//////////////////////////////////////////////////////////////////////
class Dumper
{
public:

  /// @brief コンストラクタ
  /// @param[in] mgr CNFddMgr
  /// @param[in] s 出力先のストリーム
  Dumper(CNFddMgrImpl* mgr,
	 BinO& s);

  /// @brief デストラクタ
  ~Dumper();


public:

  /// @brief e を根とするZDDの内容を出力する．
  void
  dump(CNFddEdge e);

  /// @brief e の内容を出力する．
  void
  dump_edge(CNFddEdge e);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // CNFddMgrImpl へのポインタ
  CNFddMgrImpl* mMgr;

  // 出力用のストリーム
  BinO& mStream;

  // ID 番号を管理するマネージャ
  IdMgr mIdMgr;

};

END_NAMESPACE_YM_CNFDD

#endif // DUMPER_H
