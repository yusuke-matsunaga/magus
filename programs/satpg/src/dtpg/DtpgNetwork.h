#ifndef DTPGNETWORK_H
#define DTPGNETWORK_H

/// @file DtpgNetwork.h
/// @brief DtpgNetwork のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "dtpg_nsdef.h"
#include "SaFault.h"
#include "ym_networks/tgnet.h"
#include "ym_logic/LogExpr.h"
#include "ym_utils/SimpleAlloc.h"


BEGIN_NAMESPACE_YM_SATPG_DTPG

//////////////////////////////////////////////////////////////////////
/// @class DtpgNetwork DtpgNetwork.h "DtpgNetwork.h"
/// @brief DtpgSat 用のネットワークを表すクラス
///
/// わかりにくいがここでは「ゲート」と「ノード」を区別して使う．
/// - ゲートは TgNode に対応するもの．入力と出力に故障を仮定する．
/// - ノードは単純な論理関数に対応するもの．通常はノードとゲート
///   は一致するが，複雑な論理式を持つゲートの場合は複数のノード
///   で構成されることになる．
/// 故障を考える場合にはゲート単位が好ましいが，パタン生成や故障
/// シミュレーションを行う場合には単純なノードのほうがわかりやすい
/// のでこのような構成になっている．
//////////////////////////////////////////////////////////////////////
class DtpgNetwork
{
public:

  /// @brief コンストラクタ
  /// @param[in] tgnetwork もとのネットワーク
  /// @param[in] fault_list 故障リスト
  DtpgNetwork(const TgNetwork& tgnetwork,
	      const vector<SaFault*>& fault_list);

  /// @brief デストラクタ
  ~DtpgNetwork();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 一つの外部出力に関係するノードのみをアクティブにする．
  /// @param[in] onode 外部出力ノード
  void
  activate_po(DtpgNode* onode);

  /// @brief 全てのノードをアクティブにする．
  void
  activate_all();

  /// @brief ノード数を得る．
  ymuint
  node_num();

  /// @brief ノードを得る．
  /// @param[in] id ID番号 ( 0 <= id < node_num() )
  /// @note node->id() == id となるノードを返す．
  DtpgNode*
  node(ymuint id);

  /// @brief 外部入力数を得る．
  ymuint
  input_num();

  /// @brief 外部入力数 + FF数を得る．
  ymuint
  input_num2();

  /// @brief 外部入力ノードを得る．
  /// @param[in] pos 位置番号 ( 0 <= pos < input_num2() )
  DtpgNode*
  input(ymuint pos);

  /// @brief 外部出力数を得る．
  ymuint
  output_num();

  /// @brief 外部出力数 + FF数を得る．
  ymuint
  output_num2();

  /// @brief 外部出力ノードを得る．
  /// @param[in] pos 位置番号 ( 0 <= pos < output_num2() )
  DtpgNode*
  output(ymuint pos);

  /// @brief サイズの降順で整列した順番で外部出力ノードを取り出す．
  DtpgNode*
  output2(ymuint pos);

  /// @brief 故障リストを得る．
  const vector<DtpgFault*>&
  fault_list();

  /// @brief アクティブなノード数を得る．
  ymuint
  active_node_num();

  /// @brief アクティブなノードを得る．
  /// @param[in] pos 位置番号 ( 0 <= pos < active_node_num() )
  DtpgNode*
  active_node(ymuint pos);

  /// @brief アクティブな部分に対して FFR を求める．
  void
  get_ffr_list(vector<DtpgFFR*>& ffr_list);

  /// @brief アクティブな部分に対して MFFC を求める．
  void
  get_mffc_list(vector<DtpgFFR*>& mffc_list);

  /// @brief fnode の TFO の TFI にマークをつける．
  /// @param[in] fnode 起点となるノード
  /// @param[in] tfo_list TFO ノードを入れるリスト
  /// @param[in] tfi_list TFI ノードを入れるリスト
  /// @note tfo_list と tfi_list はオーバーラップしない
  /// @note fnode は tfo_list[0] に入る．
  void
  mark_tfo_tfi(DtpgNode* fnode,
	       vector<DtpgNode*>& tfo_list,
	       vector<DtpgNode*>& tfi_list);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 論理式から DtpgNode を作る．
  /// @param[in] expr 論理式
  /// @param[in] tgnode 元のノード (ファンインの情報を得るために必要)
  DtpgNode*
  make_node_tree(const LogExpr& expr,
		 const TgNode* tgnode);

  /// @brief 新しいノードを確保する．
  /// @param[in] ni 入力数
  /// @note 実際には mNodeArray はすでに確保済なのでポインタを返すだけ．
  /// @note 結果 mNodeNum の値が一つ増える．
  /// @note DtptNode は適切に初期化される．
  DtpgNode*
  new_node(ymuint ni);

  /// @brief DtpgGate の設定を行う．
  /// @param[in] gate DtpgGate
  /// @param[in] root 根のノード
  /// @param[in] ni 入力数
  void
  set_gate(DtpgGate* gate,
	   DtpgNode* root,
	   ymuint ni);

  /// @brief activate_po(), activate_all() の下請け関数
  void
  activate_sub(const vector<bool>& mark);

#if 0
  /// @brief 最小項のリストからプライムカバーを作る．
  DtpgCover*
  prime_cover(ymuint ni,
	      const vector<ymuint32>& minterm_list);
#endif

private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // DtpgNetwork 関係のメモリ確保を行なうオブジェクト
  SimpleAlloc mAlloc;

  // 全ゲート数
  ymuint32 mGateNum;

  // 全ノード数
  ymuint32 mNodeNum;

  // 外部入力数
  ymuint32 mInputNum;

  // 外部出力数
  ymuint32 mOutputNum;

  // FF数
  ymuint32 mFFNum;

  // ゲートの本体の配列
  DtpgGate* mGateArray;

  // ノードの本体の配列
  DtpgNode* mNodeArray;

  // 外部入力ノードの配列
  DtpgNode** mInputArray;

  // 外部出力ノードの配列
  DtpgNode** mOutputArray;

  // TFI サイズの降順に整列した外部出力ノードの配列
  DtpgNode** mOutputArray2;

  // アクティブなノード数
  ymuint32 mActNodeNum;

  // アクティブなノードの配列
  DtpgNode** mActNodeArray;

#if 0
  // 関数の個数
  ymuint32 mFuncNum;

  // 関数番号をキーとしてプライムカバーを格納する配列
  // ただし，肯定と否定の２つづつ格納する．
  DtpgCover** mCoverList;
#endif

  // 故障の本体の配列
  DtpgFault* mFaultChunk;

  // 故障リスト
  vector<DtpgFault*> mFaultList;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief ノード数を得る．
inline
ymuint
DtpgNetwork::node_num()
{
  return mNodeNum;
}

// @brief 外部入力数を得る．
inline
ymuint
DtpgNetwork::input_num()
{
  return mInputNum;
}

// @brief 外部入力数 + FF数を得る．
inline
ymuint
DtpgNetwork::input_num2()
{
  return mInputNum + mFFNum;
}

// @brief 外部入力ノードを得る．
// @param[in] pos 位置番号 ( 0 <= pos < input_num2() )
inline
DtpgNode*
DtpgNetwork::input(ymuint pos)
{
  assert_cond( pos < input_num2(), __FILE__, __LINE__);
  return mInputArray[pos];
}

// @brief 外部出力数を得る．
inline
ymuint
DtpgNetwork::output_num()
{
  return mOutputNum;
}

// @brief 外部出力数 + FF数を得る．
inline
ymuint
DtpgNetwork::output_num2()
{
  return mOutputNum + mFFNum;
}

// @brief 外部出力ノードを得る．
// @param[in] pos 位置番号 ( 0 <= pos < output_num2() )
inline
DtpgNode*
DtpgNetwork::output(ymuint pos)
{
  assert_cond( pos < output_num2(), __FILE__, __LINE__);
  return mOutputArray[pos];
}

// @brief サイズの降順で整列した順番で外部出力ノードを取り出す．
inline
DtpgNode*
DtpgNetwork::output2(ymuint pos)
{
  assert_cond( pos < output_num2(), __FILE__, __LINE__);
  return mOutputArray2[pos];
}

// @brief 故障リストを得る．
inline
const vector<DtpgFault*>&
DtpgNetwork::fault_list()
{
  return mFaultList;
}

// @brief アクティブなノード数を得る．
inline
ymuint
DtpgNetwork::active_node_num()
{
  return mActNodeNum;
}

// @brief アクティブなノードを得る．
// @param[in] pos 位置番号 ( 0 <= pos < active_node_num() )
inline
DtpgNode*
DtpgNetwork::active_node(ymuint pos)
{
  assert_cond( pos < mActNodeNum, __FILE__, __LINE__);
  return mActNodeArray[pos];
}

END_NAMESPACE_YM_SATPG_DTPG

#endif // DTPGNETWORK_H
