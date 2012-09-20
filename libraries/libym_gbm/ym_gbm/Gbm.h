#ifndef YM_GBM_GBM_H
#define YM_GBM_GBM_H

/// @file ym_gbm/Gbm.h
/// @brief Gbm のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ym_gbm/gbm_nsdef.h"
#include "ym_gbm/GbmNodeHandle.h"
#include "ym_logic/SatSolver.h"


BEGIN_NAMESPACE_YM_GBM

class GbmNode;

//////////////////////////////////////////////////////////////////////
/// @class Gbm Gbm.h "ym_gbm/Gbm.h"
/// @brief Generilized Boolean Matcher
//////////////////////////////////////////////////////////////////////
class Gbm
{
public:

  /// @brief コンストラクタ
  /// @param[in] sat_type SAT-solver の種類を表す文字列
  /// @param[in] sat_opt SAT-solver に渡すオプション文字列
  /// @param[in] sat_log ログの出力用ストリーム
  Gbm(const string& sat_type,
      const string& sat_opt,
      ostream* sat_log);

  /// @brief デストラクタ
  ~Gbm();


public:
  //////////////////////////////////////////////////////////////////////
  // ネットワーク作成用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 外部入力ノードを作る．
  /// @return 作成したノードのハンドルを返す．
  GbmNodeHandle
  new_input();

  /// @brief ANDゲートを作る．
  /// @param[in] input0 ファンイン0のハンドル
  /// @param[in] input1 ファンイン0のハンドル
  /// @return 作成したノードのハンドルを返す．
  GbmNodeHandle
  new_and(GbmNodeHandle input0,
	  GbmNodeHandle input1);

  /// @brief ORゲートを作る．
  /// @param[in] input0 ファンイン0のハンドル
  /// @param[in] input1 ファンイン0のハンドル
  /// @return 作成したノードのハンドルを返す．
  GbmNodeHandle
  new_or(GbmNodeHandle input0,
	 GbmNodeHandle input1);

  /// @brief XORゲートを作る．
  /// @param[in] input0 ファンイン0のハンドル
  /// @param[in] input1 ファンイン0のハンドル
  /// @return 作成したノードのハンドルを返す．
  GbmNodeHandle
  new_xor(GbmNodeHandle input0,
	  GbmNodeHandle input1);

  /// @brief XNORゲートを作る．
  /// @param[in] input0 ファンイン0のハンドル
  /// @param[in] input1 ファンイン0のハンドル
  /// @return 作成したノードのハンドルを返す．
  GbmNodeHandle
  new_xnor(GbmNodeHandle input0,
	   GbmNodeHandle input1);

  /// @brief LUTを作る．
  /// @param[in] inputs ファンインのハンドルのリスト
  /// @return 作成したノードのハンドルを返す．
  GbmNodeHandle
  new_lut(const vector<GbmNodeHandle>& inputs);


public:
  //////////////////////////////////////////////////////////////////////
  // 情報を取得する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 全ノード数を返す．
  /// @note ノードIDの最大値 + 1 と一致する．
  ymuint
  node_num() const;

  /// @brief ノードを返す．
  /// @param[in] id ID番号 ( 0 <= id < node_num() )
  const GbmNode*
  node(ymuint id) const;

  /// @brief 外部入力数を返す．
  ymuint
  input_num() const;

  /// @brief 外部入力ノードを返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < input_num() )
  const GbmNode*
  input_node(ymuint pos) const;

  /// @brief ANDノード数を返す．
  ymuint
  and_num() const;

  /// @brief ANDノードを返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < and_num() )
  const GbmNode*
  and_node(ymuint pos) const;

  /// @brief LUTノード数を返す．
  ymuint
  lut_num() const;

  /// @brief LUTノードを返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < lut_num() )
  const GbmNode*
  lut_node(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // SATソルバ
  SatSolver mSolver;

  // 全ての GbmNode を格納するベクタ
  vector<GbmNode*> mNodeList;

  // 入力ノードを格納するベクタ
  vector<GbmNode*> mInputList;

  // ANDノードを格納するベクタ
  vector<GbmNode*> mAndList;

  // LUTノードを格納するベクタ
  vector<GbmNode*> mLutList;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief ORゲートを作る．
// @param[in] input0 ファンイン0のハンドル
// @param[in] input1 ファンイン0のハンドル
// @return 作成したノードのハンドルを返す．
inline
GbmNodeHandle
Gbm::new_or(GbmNodeHandle input0,
	    GbmNodeHandle input1)
{
  return ~new_and(~input0, ~input1);
}

// @brief XORゲートを作る．
// @param[in] input0 ファンイン0のハンドル
// @param[in] input1 ファンイン0のハンドル
// @return 作成したノードのハンドルを返す．
inline
GbmNodeHandle
Gbm::new_xor(GbmNodeHandle input0,
	     GbmNodeHandle input1)
{
  GbmNodeHandle n1 = new_and( input0, ~input1);
  GbmNodeHandle n2 = new_and(~input0,  input1);
  return new_or(n1, n2);
}

// @brief XNORゲートを作る．
// @param[in] input0 ファンイン0のハンドル
// @param[in] input1 ファンイン0のハンドル
// @return 作成したノードのハンドルを返す．
inline
GbmNodeHandle
Gbm::new_xnor(GbmNodeHandle input0,
	      GbmNodeHandle input1)
{
  GbmNodeHandle n1 = new_and( input0,  input1);
  GbmNodeHandle n2 = new_and(~input0, ~input1);
  return new_or(n1, n2);
}

END_NAMESPACE_YM_GBM

#endif // GBM_H
