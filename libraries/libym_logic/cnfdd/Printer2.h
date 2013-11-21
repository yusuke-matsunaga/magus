#ifndef PRINTER2_H
#define PRINTER2_H

/// @file Printer2.h
/// @brief Printer2 のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "CNFddMgrImpl.h"
#include "ym_logic/Literal.h"


BEGIN_NAMESPACE_YM_CNFDD

//////////////////////////////////////////////////////////////////////
/// @class Printer2 Printer2.h "Printer2.h"
/// @brief CNFDD の内容を出力するためのクラス
//////////////////////////////////////////////////////////////////////
class Printer2
{

public:

  /// @brief コンストラクタ
  Printer2(CNFddMgrImpl* mgr,
	  ostream& s);

  /// @brief デストラクタ
  ~Printer2();


public:

  /// @brief e を根とするZDDの内容を出力する．
  void
  print_root(CNFddEdge e);


private:
  //////////////////////////////////////////////////////////////////////
  // 下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief display_root の下請関数
  void
  print_step(CNFddEdge e);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // CNFddMgrImpl へのポインタ
  CNFddMgrImpl* mMgr;

  // 出力用のストリーム
  ostream& mStream;

  // 今までの要素を積んでおく配列
  vector<Literal> mElemList;

};

END_NAMESPACE_YM_CNFDD

#endif // PRINTER2_H
