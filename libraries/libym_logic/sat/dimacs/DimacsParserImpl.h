#ifndef LIBYM_LOGIC_SAT_DIMACSPARSERIMPL_H
#define LIBYM_LOGIC_SAT_DIMACSPARSERIMPL_H

/// @file libym_logic/sat/dimacs/DimacsParserImpl.h
/// @brief DimacsParserImpl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_logic/sat_nsdef.h"
#include "DimacsScanner.h"


BEGIN_NAMESPACE_YM_SAT

//////////////////////////////////////////////////////////////////////
/// @class DimacsParserImpl DimacsParserImpl.h "DimacsParserImpl.h"
/// @brief DimacsParser の実装クラス
//////////////////////////////////////////////////////////////////////
class DimacsParserImpl
{
public:

  /// @brief コンストラクタ
  DimacsParserImpl();

  /// @brief デストラクタ
  ~DimacsParserImpl();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 読み込みを行う．
  /// @param[in] filename ファイル名
  /// @retval true 読み込みが成功した．
  /// @retval false 読み込みが失敗した．
  bool
  read(const string& filename);

  /// @brief イベントハンドラの登録
  void
  add_handler(DimacsHandler* handler);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 字句解析器
  DimacsScanner mScanner;

  // イベントハンドラのリスト
  list<DimacsHandler*> mHandlerList;

};

END_NAMESPACE_YM_SAT

#endif // LIBYM_LOGIC_SAT_DIMACSPARSERIMPL_H
