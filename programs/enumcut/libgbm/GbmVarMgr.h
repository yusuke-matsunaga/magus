#ifndef GBMVARMGR_H
#define GBMVARMGR_H

/// @file GbmVarMgr.h
/// @brief GbmVarMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmTools.h"
#include "GbmLit.h"
#include "RcfNetwork.h"
#include "RcfNode.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class GbmVarMgr GbmVarMgr.h "GbmVarMgr.h"
/// @brief GBM の基本処理を行なうクラス
//////////////////////////////////////////////////////////////////////
class GbmVarMgr
{
public:

  /// @brief コンストラクタ
  GbmVarMgr();

  /// @brief デストラクタ
  ~GbmVarMgr();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief debug フラグを立てる
  void
  debug_on();

  /// @brief debug フラグを降ろす
  void
  debug_off();

  /// @brief debug フラグの値を得る．
  bool
  debug() const;

  /// @brief 設定変数を初期化する．
  /// @param[in] network 対象のネットワーク
  void
  init(const RcfNetwork& network);

  /// @brief モデルから設定変数の割り当てを取り出す．
  /// @param[in] model SAT モデル
  /// @param[out] conf_bits 設定変数の割り当て
  void
  get_conf_bits(const vector<Bool3>& model,
		vector<bool>& conf_bits) const;

  /// @brief モデルから入力順を取り出す．
  /// @param[in] model SAT モデル
  /// @param[out] iorder 入力順
  ///             iorder[pos] に network の pos 番めの入力に対応した
  ///             関数の入力番号が入る．
  virtual
  void
  get_iorder(const vector<Bool3>& model,
	     vector<ymuint>& iorder) const;


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスから用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ノードに対応するリテラルを登録する．
  /// @param[in] id ノード番号
  /// @param[in] lit リテラル
  void
  set_node_var(ymuint id,
	       GbmLit lit);

  /// @brief 内部ノードに変数番号を割り当て，CNF式を作る．
  /// @param[in] network 対象の LUT ネットワーク
  /// @param[in] oval 出力値
  /// @return 割り当てが矛盾を起こしたら false を返す．
  bool
  make_nodes_cnf(const RcfNetwork& network,
		 ymuint oval);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ノードIDをキーにしてリテラルを格納する配列
  vector<GbmLit> mNodeVarArray;

  // 設定変数番号をキーにして論理式上の変数番号を格納する配列
  vector<VarId> mConfVarArray;

  // debug フラグ
  bool mDebug;

};

END_NAMESPACE_YM

#endif // GBMVARMGR_H
