#ifndef PRINTER2_H
#define PRINTER2_H

/// @file Printer2.h
/// @brief Printer2 のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ZddMgrImpl.h"


BEGIN_NAMESPACE_YM_ZDD

//////////////////////////////////////////////////////////////////////
/// @class Printer2 Printer2.h "Printer2.h"
/// @brief ZDD の内容を出力するためのクラス
//////////////////////////////////////////////////////////////////////
class Printer2
{

public:

  /// @brief コンストラクタ
  Printer2(ZddMgrImpl* mgr,
	   ostream& s);

  /// @brief デストラクタ
  ~Printer2();


public:

  /// @brief e を根とするZDDの内容を出力する．
  void
  print_root(ZddEdge e);


private:
  //////////////////////////////////////////////////////////////////////
  // 下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief print_root の下請関数
  void
  print_step(ZddEdge e);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ZddMgrImpl へのポインタ
  ZddMgrImpl* mMgr;

  // 出力用のストリーム
  ostream& mStream;

  // 今までの要素を積んでおく配列
  vector<ymuint32> mElemList;
};

END_NAMESPACE_YM_ZDD

#endif // PRINTER2_H
