#ifndef DIMACSPARSERIMPL_H
#define DIMACSPARSERIMPL_H

/// @file DimacsParserImpl.h
/// @brief DimacsParserImpl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmLogic/sat_nsdef.h"


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
  /// @param[in] ido 入力データ
  /// @retval true 読み込みが成功した．
  /// @retval false 読み込みが失敗した．
  bool
  read(IDO& ido);

  /// @brief イベントハンドラの登録
  void
  add_handler(DimacsHandler* handler);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // イベントハンドラのリスト
  list<DimacsHandler*> mHandlerList;

};

END_NAMESPACE_YM_SAT

#endif // DIMACSPARSERIMPL_H
