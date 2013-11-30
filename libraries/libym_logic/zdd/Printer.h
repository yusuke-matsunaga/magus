#ifndef PRINTER_H
#define PRINTER_H

/// @file Printer.h
/// @brief Printer のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ZddMgrImpl.h"
#include "IdMgr.h"


BEGIN_NAMESPACE_YM_ZDD

//////////////////////////////////////////////////////////////////////
/// @class Printer Printer.h "Printer.h"
/// @brief ZDD の内容を出力するためのクラス
//////////////////////////////////////////////////////////////////////
class Printer
{

public:

  /// @brief コンストラクタ
  Printer(ZddMgrImpl* mgr,
	  ostream& s);

  /// @brief デストラクタ
  ~Printer();


public:

  /// @brief 登録された節点数を返す．
  ymuint64
  num() const;

  /// @brief e を根とするZDDの内容を出力する．
  void
  print_root(ZddEdge e);


private:
  //////////////////////////////////////////////////////////////////////
  // 下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief e の ID 番号を出力する．
  void
  print_id(ZddEdge e);

  /// @brief e の内容を出力する．
  void
  print_name(ZddEdge e);

  /// @brief display_root の下請関数
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

  // 処理済の ZddEdge に印を付けておくためのハッシュ表
  hash_set<ZddEdge> mMark;

  // ID 番号を管理するマネージャ
  IdMgr mIdMgr;

};

END_NAMESPACE_YM_ZDD

#endif // PRINTER_H
