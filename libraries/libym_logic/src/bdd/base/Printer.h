#ifndef PRINTER_H
#define PRINTER_H

/// @file Printer.h
/// @brief Printer のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "BddMgrImpl.h"
#include "IdMgr.h"
#include "YmUtils/HashSet.h"


BEGIN_NAMESPACE_YM_BDD

//////////////////////////////////////////////////////////////////////
/// @class Printer Printer.h "Printer.h"
/// @brief BDD の内容を出力するためのクラス
//////////////////////////////////////////////////////////////////////
class Printer
{
public:

  /// @brief コンストラクタ
  Printer(BddMgrImpl* mgr,
	  ostream& s);

  /// @brief デストラクタ
  ~Printer();


public:

  /// @brief 登録された節点数を返す．
  ymuint64
  num() const;

  /// @brief e を根とするBDDの内容を出力する．
  void
  print_root(BddEdge e);


private:
  //////////////////////////////////////////////////////////////////////
  // 下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief e の ID 番号を出力する．
  void
  print_id(BddEdge e);

  /// @brief e の内容を出力する．
  void
  print_name(BddEdge e);

  /// @brief display_root の下請関数
  void
  print_step(BddEdge e);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // BddMgrImpl へのポインタ
  BddMgrImpl* mMgr;

  // 出力用のストリーム
  ostream& mStream;

  // 処理済の BddEdge に印を付けておくためのハッシュ表
  HashSet<BddEdge> mMark;

  // ID 番号を管理するマネージャ
  IdMgr mIdMgr;

};

END_NAMESPACE_YM_BDD

#endif
