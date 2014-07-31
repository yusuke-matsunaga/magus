#ifndef GBMSOLVER_H
#define GBMSOLVER_H

/// @file GbmSolver.h
/// @brief GbmSolver のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "YmTools.h"
#include "YmLogic/TvFunc.h"


BEGIN_NAMESPACE_YM

class RcfNetwork;

//////////////////////////////////////////////////////////////////////
/// @class GbmSolver GbmSolver.h "GbmSolver.h"
/// @brief GBM を解くクラス
//////////////////////////////////////////////////////////////////////
class GbmSolver
{
public:

  /// @brief コンストラクタ
  GbmSolver();

  /// @brief デストラクタ
  virtual
  ~GbmSolver();

  /// @brief 派生クラスを作るクラスメソッド
  /// @param[in] method 手法を表す文字列
  static
  GbmSolver*
  new_solver(const string& method = string());


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief verify フラグを立てる
  void
  verify_on();

  /// @brief verify フラグを降ろす
  void
  verify_off();

  /// @brief debug フラグを立てる．
  void
  debug_on();

  /// @brief debug フラグを降ろす.
  void
  debug_off();

  /// @brief debug フラグを得る．
  bool
  debug() const;

  /// @brief 入力順を考慮したマッチング問題を解く
  /// @param[in] networrk RcfNetwork
  /// @param[in] func マッチング対象の関数
  /// @param[out] conf_bits configuration ビットの値を収める配列
  /// @param[out] iorder 入力順序
  ///             iorder[pos] に network の pos 番めの入力に対応した
  ///             関数の入力番号が入る．
  bool
  solve(const RcfNetwork& network,
	const TvFunc& func,
	vector<bool>& conf_bits,
	vector<ymuint>& iorder,
	ymuint& loop_count);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力順を考慮したマッチング問題を解く
  /// @param[in] network RcfNetwork
  /// @param[in] output Reconfigurable Network の出力
  /// @param[in] func マッチング対象の関数
  /// @param[in] rep 関数の対称変数の代表番号を収める配列
  ///            rep[pos] に pos 番めの入力の代表番号が入る．
  /// @param[out] conf_bits configuration ビットの値を収める配列
  /// @param[out] iorder 入力順序
  ///             iorder[pos] に network の pos 番めの入力に対応した
  ///             関数の入力番号が入る．
  virtual
  bool
  _solve(const RcfNetwork& network,
	 const TvFunc& func,
	 const vector<ymuint>& rep,
	 vector<bool>& conf_bits,
	 vector<ymuint>& iorder,
	 ymuint& loop_count) = 0;

  /// @brief 検証を行う．
  /// @param[in] network RcfNetwork
  /// @param[in] func マッチング対象の関数
  /// @param[in] conf_bits configuration ビットの値を収める配列
  /// @param[in] iorder 入力順序
  ///             iorder[pos] に network の pos 番めの入力に対応した
  ///             関数の入力番号が入る．
  bool
  verify(const RcfNetwork& network,
	 const TvFunc& func,
	 const vector<bool>& conf_bits,
	 const vector<ymuint>& iorder);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // verify フラグ
  bool mVerify;

  // debug フラグ
  bool mDebug;

};

END_NAMESPACE_YM

#endif // GBMSOLVER_H
