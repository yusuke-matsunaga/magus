#ifndef DUMPER_H
#define DUMPER_H

/// @file Dumper.h
/// @brief Dumper のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ZddMgrImpl.h"
#include "IdMgr.h"
#include "ym_utils/ODO.h"


BEGIN_NAMESPACE_YM_ZDD

//////////////////////////////////////////////////////////////////////
/// @class Dumper Dumper.h "Dumper.h"
/// @brief ZDD の内容を保存するためのクラス
//////////////////////////////////////////////////////////////////////
class Dumper
{
public:

  /// @brief コンストラクタ
  /// @param[in] mgr ZddMgr
  /// @param[in] s 出力先のストリーム
  Dumper(ZddMgrImpl* mgr,
	 ODO& s);

  /// @brief デストラクタ
  ~Dumper();


public:

  /// @brief e を根とするZDDの内容を出力する．
  void
  dump(ZddEdge e);

  /// @brief e の内容を出力する．
  void
  dump_edge(ZddEdge e);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ZddMgrImpl へのポインタ
  ZddMgrImpl* mMgr;

  // 出力用のストリーム
  ODO& mStream;

  // ID 番号を管理するマネージャ
  IdMgr mIdMgr;

};

END_NAMESPACE_YM_ZDD

#endif // DUMPER_H
