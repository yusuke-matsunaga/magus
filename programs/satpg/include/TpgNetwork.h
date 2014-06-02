#ifndef TPGNETWORK_H
#define TPGNETWORK_H

/// @file TpgNetwork.h
/// @brief TpgNetwork のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2014 Yusuke Matsunaga
/// All rights reserved.


#include "satpg_nsdef.h"
#include "networks/tgnet.h"
#include "cell/cell_nsdef.h"
#include "logic/Bool3.h"
#include "logic/expr_nsdef.h"
#include "utils/SimpleAlloc.h"


BEGIN_NAMESPACE_YM_SATPG

class TpgMap;

//////////////////////////////////////////////////////////////////////
/// @class CplxInfo TpgNetwork.h "TpgNetwork.h"
/// @brief cplx_logic 用の情報を格納するクラス
//////////////////////////////////////////////////////////////////////
struct CplxInfo
{
  /// @brief コンストラクタ
  /// @param[in] node_num ノード数
  /// @param[in] fanin_num ファンイン数
  CplxInfo(ymuint node_num,
	   ymuint fanin_num) :
    mExtraNodeCount(node_num),
    mCVal(fanin_num * 2) { }

  // 根のノード以外に必要となるノード数
  ymuint32 mExtraNodeCount;

  // 制御値を納める配列
  // pos 番目の 0 が mCVal[pos * 2 + 0] に対応する．
  vector<Bool3> mCVal;

};


//////////////////////////////////////////////////////////////////////
/// @class TpgNetwork TpgNetwork.h "TpgNetwork.h"
/// @brief SATPG 用のネットワークを表すクラス
//////////////////////////////////////////////////////////////////////
class TpgNetwork
{
  typedef unordered_map<ymuint, CplxInfo> CplxInfoMap;

public:
  //////////////////////////////////////////////////////////////////////
  // ファイルを読み込んでインスタンスを作るクラスメソッド
  //////////////////////////////////////////////////////////////////////

  /// @brief blif ファイルを読み込んでインスタンスを作る．
  /// @param[in] filename ファイル名
  /// @param[in] cell_library セルライブラリ
  /// @note エラーが起こったら NULL を返す．
  static
  TpgNetwork*
  read_blif(const string& filename,
	    const CellLibrary* cell_library);

  /// @brief iscas89 形式のファイルを読み込む．
  /// @param[in] filename ファイル名
  /// @note エラーが起こったら NULL を返す．
  static
  TpgNetwork*
  read_iscas89(const string& filename);

  /// @brief デストラクタ
  ~TpgNetwork();


public:
  //////////////////////////////////////////////////////////////////////
  // 通常の構造情報を得る関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ノード数を得る．
  ymuint
  node_num() const;

  /// @brief ノードを得る．
  /// @param[in] id ID番号 ( 0 <= id < node_num() )
  /// @note node->id() == id となるノードを返す．
  TpgNode*
  node(ymuint id);

  /// @brief ノードを得る．
  /// @param[in] id ID番号 ( 0 <= id < node_num() )
  /// @note node->id() == id となるノードを返す．
  const TpgNode*
  node(ymuint id) const;

  /// @brief ノード番号の最大値を得る．
  ///
  /// ダミーノードがあるので上の node_num() とは異なる．
  ymuint
  max_node_id() const;

  /// @brief 外部入力数を得る．
  ymuint
  input_num() const;

  /// @brief 外部入力数 + FF数を得る．
  ymuint
  input_num2() const;

  /// @brief 外部入力ノードを得る．
  /// @param[in] pos 位置番号 ( 0 <= pos < input_num2() )
  TpgNode*
  input(ymuint pos) const;

  /// @brief 外部出力数を得る．
  ymuint
  output_num() const;

  /// @brief 外部出力数 + FF数を得る．
  ymuint
  output_num2() const;

  /// @brief 外部出力ノードを得る．
  /// @param[in] pos 位置番号 ( 0 <= pos < output_num2() )
  TpgNode*
  output(ymuint pos) const;

  /// @brief サイズの降順で整列した順番で外部出力ノードを取り出す．
  TpgNode*
  output2(ymuint pos) const;

  /// @brief 代表故障のリストを得る．
  const vector<TpgFault*>&
  rep_faults() const;

  /// @brief 故障IDの最大値+1を返す．
  ymuint
  max_fault_id() const;


public:
  //////////////////////////////////////////////////////////////////////
  // 故障挿入に関する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 故障挿入の開始
  void
  begin_fault_injection();

  /// @brief ブランチの故障用のダミーノードを挿入する．
  /// @param[in] node 対象のノード
  /// @param[in] ipos 入力位置
  /// @return 挿入したノードを返す．
  TpgNode*
  inject_fnode(TpgNode* node,
	       ymuint ipos);

  /// @brief 挿入したダミーノードを削除する．
  void
  end_fault_injection();


public:
  //////////////////////////////////////////////////////////////////////
  // アクティブ領域に関するアクセス関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 一つの外部出力に関係するノードのみをアクティブにする．
  /// @param[in] po_pos 出力番号
  void
  activate_po(ymuint po_pos);

  /// @brief 一つの外部出力に関係するノードのみをアクティブにする．
  /// @param[in] po 出力ノード
  void
  activate_po(TpgNode* po);

  /// @brief 全てのノードをアクティブにする．
  void
  activate_all();

  /// @brief アクティブなノード数を得る．
  ymuint
  active_node_num() const;

  /// @brief アクティブなノードを得る．
  /// @param[in] pos 位置番号 ( 0 <= pos < active_node_num() )
  TpgNode*
  active_node(ymuint pos);

  /// @brief アクティブなノードを得る．
  /// @param[in] pos 位置番号 ( 0 <= pos < active_node_num() )
  const TpgNode*
  active_node(ymuint pos) const;


public:
  //////////////////////////////////////////////////////////////////////
  // 必要割り当てに関する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief f の検出に必要な割り当てを求める．
  /// @param[in] f 対象の故障
  /// @param[in] ma_list 割り当て結果を格納するリスト
  /// @return 矛盾が生じたら(fが冗長故障の場合) false を返す．
  /// @note TpgNetwork のメンバにはアクセスしないので static メンバになっている．
  /// @note ma_list の内容は TpgNode::id() * 2 + val (0 / 1)
  static
  bool
  get_mandatory_assignment(TpgFault* f,
			   vector<ymuint32>& ma_list);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる下請け関数
  //////////////////////////////////////////////////////////////////////

  /// TpgNode の TFIbits のサイズを計算する．
  ymuint
  tfibits_size() const;

  /// @brief 入力ノードを生成する．
  /// @param[in] tgnode もととなる TgNode
  /// @return 生成したノードを返す．
  TpgNode*
  make_input_node(const TgNode* tgnode);

  /// @brief 出力ノードを生成する．
  /// @param[in] tgnode もととなる TgNode
  /// @return 生成したノードを返す．
  TpgNode*
  make_output_node(const TgNode* tgnode);

  /// @brief 論理ノードを生成する．
  /// @param[in] tgnode もととなる TgNode
  /// @param[in] tgnetwork もととなるネットワーク
  /// @return 生成したノードを返す．
  ///
  /// 場合によっては複数の TpgNode を生成する．
  TpgNode*
  make_logic_node(const TgNode* tgnode,
		  const TgNetwork& tgnetwork);

  /// @brief 論理式から TpgNode の木を生成する．
  /// @param[in] expr 式
  /// @param[in] nfo 根のノードのファンアウト数
  /// @param[in] leaf_nodes 式のリテラルに対応するノードの配列
  /// @param[in] input_map ファンインの対応関係を収める配列
  /// @return 生成したノードを返す．
  ///
  /// leaf_nodes は 変数番号 * 2 + (0/1) に
  /// 該当する変数の肯定/否定のリテラルが入っている．
  TpgNode*
  make_cplx_node(const Expr& expr,
		 ymuint nfo,
		 const vector<TpgNode*>& leaf_nodes,
		 vector<pair<TpgNode*, ymuint> >& input_map);

  /// @brief 組み込み型の論理ゲートを生成する．
  /// @param[in] type ゲートの型
  /// @param[in] root 根のノードの時 true
  /// @param[in] ni ファンイン数
  /// @param[in] nfo ファンアウト数
  /// @return 生成したノードを返す．
  TpgNode*
  make_prim_node(tTgGateType type,
		 bool root,
		 ymuint ni,
		 ymuint nfo);

  /// @brief ダミーノード用の TpgNode を生成する．
  /// @return 生成したノードを返す．
  TpgNode*
  make_dummy_node();

  /// @brief TpgNode を初期化する．
  /// @param[in] node 対象のノード
  /// @param[in] ni ファンイン数
  /// @param[in] nfo ファンアウト数
  /// @return 生成したノードを返す．
  void
  init_node(TpgNode* node,
	    ymuint ni,
	    ymuint nfo);

  /// @brief TpgNode を生成する．
  /// @return 生成したノードを返す．
  ///
  /// 実際には mNodeChunk で確保した領域から返す．
  TpgNode*
  new_node();

  /// @brief ノード間の接続を行う．
  /// @param[in] src ソースノード
  /// @param[in] dst ディスティネーションノード
  /// @param[in] ipos ファンイン番号
  void
  connect(TpgNode* src,
	  TpgNode* dst,
	  ymuint ipos);

  /// @brief TpgNode と TpgNode の対応付けを行う．
  /// @param[in] node TpgNode
  /// @param[in] tgnode もととなる TgNode
  void
  bind(TpgNode* node,
       const TgNode* tgnode);

  /// @brief ノードの入力と出力の故障を作る．
  /// @param[in] tgnode もととなる TgNode
  void
  make_faults(const TgNode* tgnode,
	      const unordered_map<ymuint, CplxInfo>& en_hash);

  /// @brief 出力の故障を作る．
  /// @param[in] node 故障位置のノード
  /// @param[in] val 故障値 ( 0 / 1 )
  /// @param[in] rep 代表故障
  ///
  /// 自分自身が代表故障の場合には rep に NULL を入れる．
  TpgFault*
  new_ofault(TpgNode* node,
	     ymuint val,
	     TpgFault* rep);

  /// @brief 入力の故障を作る．
  /// @param[in] node 故障位置のノード
  /// @param[in] ipos ファンイン番号 ( 0 <= ipos < node->fanin_num() )
  /// @param[in] val 故障値
  /// @param[in] rep 代表故障
  ///
  /// 自分自身が代表故障の場合には rep に NULL を入れる．
  TpgFault*
  new_ifault(TpgNode* ode,
	     ymuint ipos,
	     ymuint val,
	     TpgFault* rep);

  /// @brief 故障を生成する．
  /// @param[in] node 対象のノード
  /// @param[in] is_output 出力の故障のときに true とするフラグ
  /// @param[in] ipos 入力の故障の時に入力番号を表す
  /// @param[in] val 縮退している値
  /// @param[in] rep 代表故障
  TpgFault*
  new_fault(TpgNode* node,
	    bool is_output,
	    ymuint ipos,
	    ymuint val,
	    TpgFault* rep);

  /// @brief ノードの TFI にマークをつける．
  /// @note 結果は mTmpMark[node->id()] に格納される．
  /// @note マークの追加ノードは mTmpNodeList[0] - mTmpNodeList[mTmpNodeNum - 1]
  /// に格納される．
  void
  tfimark(TpgNode* node);

  /// @brief TFI マークを消す．
  /// @note この関数が終了すると mTmpNodeNum は 0 になる．
  void
  clear_tfimark();

  /// @brief activate_po(), activate_all() の下請け関数
  void
  activate_sub();


private:
  //////////////////////////////////////////////////////////////////////
  // オブジェクトの生成はクラスメソッドのみが行なう．
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ
  /// @param[in] tgnetwork もとのネットワーク
  TpgNetwork(const TgNetwork& tgnetwork);

  /// @brief コピーコンストラクタは実装しない．
  TpgNetwork(const TpgNetwork& src);

  /// @brief 代入演算子も実装しない．
  const TpgNetwork&
  operator=(const TpgNetwork& src);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // TpgNetwork 関係のメモリ確保を行なうオブジェクト
  SimpleAlloc mAlloc;

  // 外部入力数
  ymuint32 mInputNum;

  // 外部出力数
  ymuint32 mOutputNum;

  // FF数
  ymuint32 mFFNum;

  // ノード数(mNodeArrayの要素数)
  ymuint32 mNodeNum;

  // ノードの本体の配列
  TpgNode* mNodeArray;

  // ノード番号の最大値
  ymuint32 mMaxNodeId;

  // TgNode->gid() をキーにした TpgMap の配列
  TpgNode** mNodeMap;

  // 外部入力ノードの配列
  TpgNode** mInputArray;

  // 外部出力ノードの配列
  TpgNode** mOutputArray;

  // TFI サイズの降順に整列した外部出力ノードの配列
  TpgNode** mOutputArray2;

  // アクティブなノード数
  ymuint32 mActNodeNum;

  // アクティブなノードの配列
  TpgNode** mActNodeArray;

  // activate_sub() で用いられるマーク用の配列
  // サイズは mNodeNum
  bool* mTmpMark;

  // 一時的に用いるノードリスト
  // サイズは mNodeNum
  TpgNode** mTmpNodeList;

  // mTmpNodeList の見かけのサイズ
  ymuint32 mTmpNodeNum;

  // 全故障数
  ymuint32 mFaultNum;

  // 故障の配列
  TpgFault* mFaultChunk;

  // 代表故障のリスト
  vector<TpgFault*> mRepFaults;

  // 未使用のブランチの故障用のダミーノードのリスト
  vector<TpgNode*> mDummyNodeList;

  // 現在挿入中のダミーノードのリスト
  vector<TpgNode*> mFnodeList;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief ノード数を得る．
inline
ymuint
TpgNetwork::node_num() const
{
  return mNodeNum;
}

// @brief ノード番号の最大値を得る．
//
// ダミーノードがあるので上の node_num() とは異なる．
inline
ymuint
TpgNetwork::max_node_id() const
{
  return mMaxNodeId;
}

// @brief 外部入力数を得る．
inline
ymuint
TpgNetwork::input_num() const
{
  return mInputNum;
}

// @brief 外部入力数 + FF数を得る．
inline
ymuint
TpgNetwork::input_num2() const
{
  return mInputNum + mFFNum;
}

// @brief 外部入力ノードを得る．
// @param[in] pos 位置番号 ( 0 <= pos < input_num2() )
inline
TpgNode*
TpgNetwork::input(ymuint pos) const
{
  assert_cond( pos < input_num2(), __FILE__, __LINE__);
  return mInputArray[pos];
}

// @brief 外部出力数を得る．
inline
ymuint
TpgNetwork::output_num() const
{
  return mOutputNum;
}

// @brief 外部出力数 + FF数を得る．
inline
ymuint
TpgNetwork::output_num2() const
{
  return mOutputNum + mFFNum;
}

// @brief 外部出力ノードを得る．
// @param[in] pos 位置番号 ( 0 <= pos < output_num2() )
inline
TpgNode*
TpgNetwork::output(ymuint pos) const
{
  assert_cond( pos < output_num2(), __FILE__, __LINE__);
  return mOutputArray[pos];
}

// @brief サイズの降順で整列した順番で外部出力ノードを取り出す．
inline
TpgNode*
TpgNetwork::output2(ymuint pos) const
{
  assert_cond( pos < output_num2(), __FILE__, __LINE__);
  return mOutputArray2[pos];
}

// @brief 代表故障のリストを得る．
inline
const vector<TpgFault*>&
TpgNetwork::rep_faults() const
{
  return mRepFaults;
}

// @brief 故障IDの最大値+1を返す．
inline
ymuint
TpgNetwork::max_fault_id() const
{
  return mFaultNum;
}

// @brief アクティブなノード数を得る．
inline
ymuint
TpgNetwork::active_node_num() const
{
  return mActNodeNum;
}

// @brief アクティブなノードを得る．
// @param[in] pos 位置番号 ( 0 <= pos < active_node_num() )
inline
TpgNode*
TpgNetwork::active_node(ymuint pos)
{
  assert_cond( pos < mActNodeNum, __FILE__, __LINE__);
  return mActNodeArray[pos];
}

// @brief アクティブなノードを得る．
// @param[in] pos 位置番号 ( 0 <= pos < active_node_num() )
inline
const TpgNode*
TpgNetwork::active_node(ymuint pos) const
{
  assert_cond( pos < mActNodeNum, __FILE__, __LINE__);
  return mActNodeArray[pos];
}

// TpgNode の TFIbits のサイズを計算する．
inline
ymuint
TpgNetwork::tfibits_size() const
{
  return (output_num2() + 7) / 8;
}

END_NAMESPACE_YM_SATPG

#endif // TPGNETWORK_H
