#ifndef YMYMLOGIC_DIMACSPARSER_H
#define YMYMLOGIC_DIMACSPARSER_H

/// @file YmLogic/DimacsParser.h
/// @brief DimacsParser のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "YmLogic/sat_nsdef.h"
#include "YmUtils/IDO.h"


BEGIN_NAMESPACE_YM_SAT

// クラスの前方宣言
class DimacsParserImpl;

//////////////////////////////////////////////////////////////////////
/// @class DimacsParser DimacsParser.h "YmLogic/DimacsParser.h"
/// @ingroup SatGroup
/// @brief DIMACS形式の CNFファイルを読み込むパーサークラス
/// @sa DimacsHandler
//////////////////////////////////////////////////////////////////////
class DimacsParser
{
public:

  /// @brief コンストラクタ
  DimacsParser();

  /// @brief デストラクタ
  ~DimacsParser();


public:

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

  // 実際の処理を行うクラス
  DimacsParserImpl* mImpl;

};

END_NAMESPACE_YM_SAT

#endif // YMYMLOGIC_DIMACSPARSER_H
