#ifndef YM_LOGIC_DIMACSPARSER_H
#define YM_LOGIC_DIMACSPARSER_H

/// @file ym_logic/DimacsParser.h
/// @brief DimacsParser のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_logic/sat_nsdef.h"
#include "ym_utils/FileRegion.h"


BEGIN_NAMESPACE_YM_SAT

// クラスの前方宣言
class DimacsParserImpl;

//////////////////////////////////////////////////////////////////////
/// @class DimacsParser DimacsParser.h "ym_logic/DimacsParser.h"
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

  // 実際の処理を行うクラス
  DimacsParserImpl* mImpl;

};

END_NAMESPACE_YM_SAT

#endif // YM_LOGIC_DIMACSPARSER_H
