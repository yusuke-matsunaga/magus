#ifndef LOGIC_DIMACSVERIFIER_H
#define LOGIC_DIMACSVERIFIER_H

/// @file logic/DimacsVerifier.h
/// @brief DimacsVerifier のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2013 Yusuke Matsunaga
/// All rights reserved.


#include "logic/DimacsHandler.h"
#include "logic/Bool3.h"


BEGIN_NAMESPACE_YM_SAT

//////////////////////////////////////////////////////////////////////
/// @class DimacsVerifier DimacsVerifier.h "logic/DimacsVerifier.h"
/// @brief 検証用のDimacsHandler
//////////////////////////////////////////////////////////////////////
class DimacsVerifier :
  public DimacsHandler
{
public:

  /// @brief コンストラクタ
  DimacsVerifier();

  /// @brief デストラクタ
  virtual
  ~DimacsVerifier();


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


public:
  //////////////////////////////////////////////////////////////////////
  // DimacsVerifier の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 検証する．
  /// @param[in] model モデル
  /// @retval true model が実際に充足解だった．
  /// @retval false model は充足解ではなかった．
  bool
  verify(const vector<Bool3>& model) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // CNF を覚えておくためのリスト
  // 内容は DimacsHandler::read_clause() の lits のリスト
  vector<vector<int> > clause_list;

};

END_NAMESPACE_YM_SAT

#endif // LOGIC_DIMACSVERIFIER_H
