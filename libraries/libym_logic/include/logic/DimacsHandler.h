#ifndef LOGIC_DIMACSHANDLER_H
#define LOGIC_DIMACSHANDLER_H

/// @file logic/DimacsHandler.h
/// @brief DimacsHandler のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013-2014 Yusuke Matsunaga
/// All rights reserved.


#include "logic/sat_nsdef.h"
#include "utils/FileRegion.h"


BEGIN_NAMESPACE_YM_SAT

//////////////////////////////////////////////////////////////////////
/// @class DimacsHandler DimacsHandler.h "logic/DimacsHandler.h"
/// @ingroup SatGroup
/// @brief DIMACS パーサーのイベントハンドラの基底クラス
/// @sa DimacsParser
//////////////////////////////////////////////////////////////////////
class DimacsHandler
{
protected:

  /// @brief コンストラクタ
  DimacsHandler();

  /// @brief デストラクタ
  virtual
  ~DimacsHandler();


public:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスは必要に応じて下記の仮想関数を上書きすること
  // デフォルトのハンドラはなにもしない．
  //////////////////////////////////////////////////////////////////////

  /// @brief 初期化
  /// @retval true 処理が成功した．
  /// @retval false エラーが起こった．
  virtual
  bool
  init();

  /// @brief p 行の読込み
  /// @param[in] loc ファイル上の位置情報
  /// @param[in] nv 変数の数
  /// @param[in] nc 節の数
  /// @retval true 処理が成功した．
  /// @retval false エラーが起こった．
  virtual
  bool
  read_p(const FileRegion& loc,
	 ymuint nv,
	 ymuint nc);

  /// @brief clause 行の読込み
  /// @param[in] loc ファイル上の位置情報
  /// @param[in] lits リテラルの配列．最後の0は含まない
  /// @retval true 処理が成功した．
  /// @retval false エラーが起こった．
  virtual
  bool
  read_clause(const FileRegion& loc,
	      const vector<int>& lits);

  /// @brief 終了処理
  /// @retval true 処理が成功した．
  /// @retval false エラーが起こった．
  virtual
  bool
  end();

  /// @brief エラー終了時の処理
  virtual
  void
  error_exit();

};

END_NAMESPACE_YM_SAT

#endif // LOGIC_DIMACSPARSER_H
