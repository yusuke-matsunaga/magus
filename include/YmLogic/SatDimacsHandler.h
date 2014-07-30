#ifndef YMYMLOGIC_SATDIMACSHANDLER_H
#define YMYMLOGIC_SATDIMACSHANDLER_H

/// @file YmLogic/SatDimactHandler.h
/// @brief SatDimacsHandler のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2013-2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmLogic/DimacsHandler.h"
#include "YmLogic/SatSolver.h"


BEGIN_NAMESPACE_YM_SAT

//////////////////////////////////////////////////////////////////////
/// @class SatDimacsHandler SatDimacsHandler.h "YmLogic/SatDimacsHandler.h"
/// @brief SatSolver 用の DimacsHandler
//////////////////////////////////////////////////////////////////////
class SatDimacsHandler :
  public DimacsHandler
{
public:

  /// @brief コンストラクタ
  /// @param[in] solver 設定対象の SatSolver
  SatDimacsHandler(SatSolver& solver);

  /// @brief デストラクタ
  virtual
  ~SatDimacsHandler();


public:
  //////////////////////////////////////////////////////////////////////
  // DimacsHandler の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 初期化
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
  virtual
  bool
  end();

  /// @brief エラー終了時の処理
  virtual
  void
  error_exit();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  SatSolver& mSolver;

};

END_NAMESPACE_YM_SAT

#endif // YMYMLOGIC_SATDIMACSHANDLER_H
