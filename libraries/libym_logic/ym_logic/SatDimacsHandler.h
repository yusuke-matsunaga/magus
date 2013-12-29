#ifndef YM_LOGIC_SATDIMACSHANDLER_H
#define YM_LOGIC_SATDIMACSHANDLER_H

/// @file ym_logic/SatDimactHandler.h
/// @brief SatDimacsHandler のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_logic/DimacsHandler.h"
#include "ym_logic/SatSolver.h"


BEGIN_NAMESPACE_YM_SAT

//////////////////////////////////////////////////////////////////////
/// @class SatDimacsHandler SatDimacsHandler.h "ym_logic/SatDimacsHandler.h"
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

#endif // YM_LOGIC_SATDIMACSHANDLER_H
