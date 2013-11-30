#ifndef PRINTER_H
#define PRINTER_H

/// @file Printer.h
/// @brief Printer のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "CNFddMgrImpl.h"
#include "IdMgr.h"


BEGIN_NAMESPACE_YM_CNFDD

//////////////////////////////////////////////////////////////////////
/// @class Printer Printer.h "Printer.h"
/// @brief CNFDD の内容を出力するためのクラス
//////////////////////////////////////////////////////////////////////
class Printer
{

public:

  /// @brief コンストラクタ
  Printer(CNFddMgrImpl* mgr,
	  ostream& s);

  /// @brief デストラクタ
  ~Printer();


public:

  /// @brief 登録された節点数を返す．
  ymuint64
  num() const;

  /// @brief e を根とするZDDの内容を出力する．
  void
  print_root(CNFddEdge e);


private:
  //////////////////////////////////////////////////////////////////////
  // 下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief e の ID 番号を出力する．
  void
  print_id(CNFddEdge e);

  /// @brief e の内容を出力する．
  void
  print_name(CNFddEdge e);

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

  // 処理済の CNFddEdge に印を付けておくためのハッシュ表
  hash_set<CNFddEdge> mMark;

  // ID 番号を管理するマネージャ
  IdMgr mIdMgr;

};

END_NAMESPACE_YM_CNFDD

#endif // PRINTER_H
