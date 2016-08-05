#ifndef SBJGRAPH_H
#define SBJGRAPH_H

/// @file SbjGraph.h
/// @brief SbjGraph のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2016 Yusuke Matsunaga
/// All rights reserved.


#include "sbj_nsdef.h"

#include "ym/ym_logic.h"
#include "ym/SimpleAlloc.h"
#include "ym/FragAlloc.h"
#include "ym/ItvlMgr.h"


BEGIN_NAMESPACE_YM_SBJ

//////////////////////////////////////////////////////////////////////
/// @class SbjGraph SbjGraph.h "SbjGraph.h"
/// @brief サブジェクトグラフを表すクラス
///
/// 入力ノード，出力ノード，論理ノード，DFFノードを分けてリストで持つ
/// ( @sa input_list(), output_list(), lnode_list(), dff_list() )．
/// また，すべてのノードに唯一な ID 番号を割り振っており，その ID 番号
/// からノードを取り出すこともできる( @sa node(ymuint id) )．
/// 入力ノード，出力ノードは ID 番号とは別に入力 ID 番号，および出力
/// ID 番号を持っており，それらの ID 番号からノードを取り出すこともできる．
/// ( @sa input(ymuint id), output(ymuint id) )
/// 論理ノードを入力からのトポロジカル順で処理するためには sort()
/// を用いてソートされたノードのベクタを得る．
/// @sa SbjEdge SbjNode
//////////////////////////////////////////////////////////////////////
class SbjGraph
{
public:

  /// @brief コンストラクタ
  SbjGraph();

  /// @brief コピーコンストラクタ
  SbjGraph(const SbjGraph& src);

  /// @brief 代入演算子
  const SbjGraph&
  operator=(const SbjGraph& src);

  /// @brief デストラクタ
  ~SbjGraph();


public:
  //////////////////////////////////////////////////////////////////////
  /// @name 外部インターフェイス情報の取得
  /// @{

  /// @brief モジュール名を得る．
  string
  name() const;

  /// @brief ポート数を得る．
  ymuint
  port_num() const;

  /// @brief ポートを得る．
  /// @param[in] pos 位置 ( 0 <= pos < port_num() )
  const SbjPort*
  port(ymuint pos) const;

  /// @brief 入出力ノードに関連づけられたポートを得る．
  /// @param[in] node 入出力ノード
  const SbjPort*
  port(const SbjNode* node) const;

  /// @brief 入出力ノードのポートにおけるビット位置を得る．
  /// @param[in] node 入出力ノード
  ymuint
  port_pos(const SbjNode* node) const;

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name 外部インターフェイス情報の設定
  /// @{

  /// @brief モジュール名を設定する．
  void
  set_name(const string& name);

  /// @brief ポートを追加する(1ビット版)．
  /// @param[in] name ポート名
  /// @param[in] io_node 対応付ける入出力ノード
  void
  add_port(const string& name,
	   SbjNode* io_node);

  /// @brief ポートを追加する(ベクタ版)．
  /// @param[in] name ポート名
  /// @param[in] body 対応付ける入出力ノードのベクタ
  void
  add_port(const string& name,
	   const vector<SbjNode*>& body);

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name ノード関連の情報の取得
  /// @{

  /// @brief ノードIDの最大値 + 1 の取得
  /// @return ノードIDの最大値 + 1 を返す．
  ymuint
  max_node_id() const;

  /// @brief ID 番号によるノードの取得
  /// @param[in] id ID 番号 ( 0 <= id < max_node_id() )
  /// @return ID 番号が id のノードを返す．
  /// @note 該当するノードが無い場合には nullptr を返す．
  const SbjNode*
  node(ymuint id) const;

  /// @brief 入力ノード数の取得
  /// @return 入力ノード数を返す．
  ymuint
  input_num() const;

  /// @brief 入力 ID 番号による入力ノードの取得
  /// @param[in] id 入力 ID 番号 ( 0 <= id < n_inputs() )
  /// @return 入力 ID 番号が id のノードを返す．
  SbjNode*
  input(ymuint id) const;

  /// @brief 出力のノード数を得る．
  ymuint
  output_num() const;

  /// @brief 出力 ID 番号による出力ノードの取得
  /// @param[in] id 出力 ID 番号 ( 0 <= id < n_outputs() )
  /// @return 出力 ID 番号が id のノードを返す．
  SbjNode*
  output(ymuint id) const;

  /// @brief 論理ノード数を得る．
  ymuint
  lnode_num() const;

  /// @brief 論理ノードを得る．
  /// @param[in] pos 位置番号 ( 0 <= pos < lnode_num() )
  SbjNode*
  lnode(ymuint pos) const;

  /// @brief ソートされた論理ノードのリストを得る．
  void
  sort(vector<const SbjNode*>& node_list) const;

  /// @brief 逆順で論理ソートされたノードのリストを得る．
  void
  rsort(vector<const SbjNode*>& node_list) const;

  /// @brief 段数を求める．
  /// @note 副作用として各 SbjNode のレベルが設定される．
  ymuint
  level() const;

  /// @brief 各ノードの minimum depth を求める．
  /// @param[in] k LUT の最大入力数
  /// @param[out] depth_array 各ノードの深さを収める配列
  /// @return 出力の最大深さを返す．
  ymuint
  get_min_depth(ymuint k,
		vector<ymuint>& depth_array) const;

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name DFF の情報の取得
  /// @{

  /// @brief DFFノード数を得る．
  ymuint
  dff_num() const;

  /// @brief DFFノードを得る．
  /// @param[in] pos 位置番号 ( 0 <= pos < dff_num() )
  SbjDff*
  dff(ymuint pos) const;

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name ノードの生成／変更
  /// @{

  /// @brief 入力ノードを作る．
  /// @return 作成したノードを返す．
  SbjNode*
  new_input();

  /// @brief 出力ノードを作る．
  /// @param[in] ihandle 入力ハンドル
  /// @return 作成したノードを返す．
  SbjNode*
  new_output(SbjHandle ihandle);

  /// @brief 論理式から論理ノード(の木)を作る．
  /// @param[in] expr 論理式
  /// @param[in] ihandle_list 入力ハンドルのリスト
  /// @return 作成したノードのハンドルを返す．
  ///
  /// 入力が定数の時も考慮している．
  SbjHandle
  new_logic(const Expr& expr,
	    const vector<SbjHandle>& ihandle_list);

  /// @brief ANDノードを作る．
  /// @param[in] ihandle1 1番めの入力ハンドル
  /// @param[in] ihandle2 2番めの入力ハンドル
  /// @return 作成したノードのハンドルを返す．
  ///
  /// ihandle1/ihandle2 が定数の時も考慮している．
  SbjHandle
  new_and(SbjHandle ihandle1,
	  SbjHandle ihandle2);

  /// @brief ANDノードを作る．
  /// @param[in] ihandle_list 入力ハンドルのリスト
  /// @return 作成したノードのハンドルを返す．
  ///
  /// 入力が定数の時も考慮している．
  SbjHandle
  new_and(const vector<SbjHandle>& ihandle_list);

  /// @brief ORノードを作る．
  /// @param[in] ihandle1 1番めの入力ハンドル
  /// @param[in] ihandle2 2番めの入力ハンドル
  /// @return 作成したノードのハンドルを返す．
  ///
  /// ihandle1/ihandle2 が定数の時も考慮している．
  SbjHandle
  new_or(SbjHandle ihandle1,
	 SbjHandle ihandle2);

  /// @brief ORノードを作る．
  /// @param[in] ihandle_list 入力ハンドルのリスト
  /// @return 作成したノードのハンドルを返す．
  ///
  /// 入力が定数の時も考慮している．
  SbjHandle
  new_or(const vector<SbjHandle>& ihandle_list);

  /// @brief XORノードを作る．
  /// @param[in] ihandle1 1番めの入力ハンドル
  /// @param[in] ihandle2 2番めの入力ハンドル
  /// @return 作成したノードのハンドルを返す．
  ///
  /// ihandle1/ihandle2 が定数の時も考慮している．
  SbjHandle
  new_xor(SbjHandle ihandle1,
	  SbjHandle ihandle2);

  /// @brief XORノードを作る．
  /// @param[in] ihandle_list 入力ハンドルのリスト
  /// @return 作成したノードのハンドルを返す．
  ///
  /// 入力が定数の時も考慮している．
  SbjHandle
  new_xor(const vector<SbjHandle>& ihandle_list);

  /// @brief 出力ノードの内容を変更する
  /// @param[in] node 変更対象の出力ノード
  /// @param[in] ihandle 入力ハンドル
  void
  change_output(SbjNode* node,
		SbjHandle ihandle);

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name DFF の生成を行う関数
  /// @{

  /// @brief DFFノードを作る．
  /// @param[in] input データ入力
  /// @param[in] output データ出力
  /// @param[in] clock クロック
  /// @param[in] clear リセット端子
  /// @param[in] preset セット端子
  /// @return 作成したノードを返す．
  SbjDff*
  new_dff(SbjNode* input,
	  SbjNode* output,
	  SbjNode* clock,
	  SbjNode* clear = nullptr,
	  SbjNode* preset = nullptr);

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name その他の関数
  /// @{

  /// @brief 空にする．
  void
  clear();

  /// @}
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // プライベートメンバ関数
  //////////////////////////////////////////////////////////////////////

  // 複製する．
  void
  copy(const SbjGraph& src,
       vector<SbjNode*>& nodemap);

  /// @brief 論理ノードを作る．
  /// @param[in] fcode 機能コード
  /// @param[in] inode1 1番めの入力ノード
  /// @param[in] inode2 2番めの入力ノード
  /// @return 作成したノードを返す．
  SbjNode*
  _new_lnode(ymuint fcode,
	     SbjNode* inode1,
	     SbjNode* inode2);

  /// @brief new_and の下請け関数
  /// @param[in] ihandle_list 入力ハンドルのリスト
  /// @param[in] start 開始位置
  /// @param[in] num 要素数
  SbjHandle
  _new_and(const vector<SbjHandle>& ihandle_list,
	   ymuint start,
	   ymuint num);

  /// @brief new_or の下請け関数
  /// @param[in] ihandle_list 入力ハンドルのリスト
  /// @param[in] start 開始位置
  /// @param[in] num 要素数
  SbjHandle
  _new_or(const vector<SbjHandle>& ihandle_list,
	  ymuint start,
	  ymuint num);

  /// @brief new_xor の下請け関数
  /// @param[in] ihandle_list 入力ハンドルのリスト
  /// @param[in] start 開始位置
  /// @param[in] num 要素数
  SbjHandle
  _new_xor(const vector<SbjHandle>& ihandle_list,
	   ymuint start,
	   ymuint num);

  // 新しいノードを作成し mNodeList に登録する．
  // 作成されたノードを返す．
  SbjNode*
  new_node(ymuint ni);

  // 入力ノードの削除
  void
  delete_input(SbjNode* node);

  // 出力ノードの削除
  void
  delete_output(SbjNode* node);

  // 論理ノードの削除
  void
  delete_logic(SbjNode* node);

  // DFFノードの削除
  void
  delete_dff(SbjDff* node);

  // node を削除する．
  void
  delete_node(SbjNode* node,
	      ymuint ni);

  // from を to の pos 番目のファンインとする．
  // to の pos 番目にすでに接続があった場合には自動的に削除される．
  void
  connect(SbjNode* from,
	  SbjNode* to,
	  ymuint pos);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられるデータ構造
  //////////////////////////////////////////////////////////////////////

  /// @brief 入出力ノードに関係するポートの情報
  struct PortInfo
  {
    /// @brief ポート
    SbjPort* mPort;
    /// @brief ビット位置
    ymuint32 mPos;
  };


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ノードを確保するためのアロケータ
  SimpleAlloc mAlloc;

  // SbjEdge の配列を確保するためのアロケータ
  FragAlloc mAlloc2;

  // 名前
  string mName;

  // ポートの配列
  vector<SbjPort*> mPortArray;

  // ID 番号をキーにしたノードの配列
  // すべてのノードが格納される．
  vector<SbjNode*> mNodeArray;

  // ID 番号を管理するためのオブジェクト
  ItvlMgr mItvlMgr;

  // 入力番号をキーにした入力ノードの配列
  // 穴はあいていない．
  vector<SbjNode*> mInputArray;

  // 入力番号をキーにしたポート情報の配列
  vector<PortInfo> mInputPortArray;

  // 出力番号をキーにした出力ノードの配列
  // 穴はあいていない．
  vector<SbjNode*> mOutputArray;

  // 出力番号をキーにしたポート情報の配列
  vector<PortInfo> mOutputPortArray;

  // 論理ノードのリスト
  vector<SbjNode*> mLnodeList;

  // DFFノードのリスト
  vector<SbjDff*> mDffList;

  // 最大レベル
  mutable
  ymuint32 mLevel;

  // mLevel および各 SbjNode::mLevel が正しいとき true となるフラグ
  mutable
  bool mLevelValid;

};


//////////////////////////////////////////////////////////////////////
// inline 関数の定義
//////////////////////////////////////////////////////////////////////

// @brief モジュール名を得る．
inline
string
SbjGraph::name() const
{
  return mName;
}

// @brief モジュール名を設定する．
inline
void
SbjGraph::set_name(const string& name)
{
  mName = name;
}

// @brief ポート数を得る．
inline
ymuint
SbjGraph::port_num() const
{
  return mPortArray.size();
}

// @brief ポートを得る．
// @param[in] pos 位置 ( 0 <= pos < port_num() )
inline
const SbjPort*
SbjGraph::port(ymuint pos) const
{
  return mPortArray[pos];
}

// @brief ポートを追加する(1ビット版)．
// @param[in] name ポート名
// @param[in] io_node 対応付ける入出力ノード
inline
void
SbjGraph::add_port(const string& name,
		   SbjNode* io_node)
{
  add_port(name, vector<SbjNode*>(1, io_node));
}

// ノード番号の最大値 + 1 を返す．
inline
ymuint
SbjGraph::max_node_id() const
{
  return mNodeArray.size();
}

// ID 番号が id のノードを取り出す．
// 該当するノードが無い場合には nullptr を返す．
inline
const SbjNode*
SbjGraph::node(ymuint id) const
{
  return mNodeArray[id];
}

// 入力ノード数を得る．
inline
ymuint
SbjGraph::input_num() const
{
  return mInputArray.size();
}

// ID 番号が id の入力ノードを取り出す．
inline
SbjNode*
SbjGraph::input(ymuint id) const
{
  return mInputArray[id];
}

// 出力のノード数を得る．
inline
ymuint
SbjGraph::output_num() const
{
  return mOutputArray.size();
}

// ID 番号が id の出力ノードを取り出す．
inline
SbjNode*
SbjGraph::output(ymuint id) const
{
  return mOutputArray[id];
}

// 論理ノード数を得る．
inline
ymuint
SbjGraph::lnode_num() const
{
  return mLnodeList.size();
}

// @brief 論理ノードを得る．
// @param[in] pos 位置番号 ( 0 <= pos < lnode_num() )
inline
SbjNode*
SbjGraph::lnode(ymuint pos) const
{
  ASSERT_COND( pos < lnode_num() );
  return mLnodeList[pos];
}

// DFFノード数を得る．
inline
ymuint
SbjGraph::dff_num() const
{
  return mDffList.size();
}

// @brief DFFノードを得る．
// @param[in] pos 位置番号 ( 0 <= pos < dff_num() )
inline
SbjDff*
SbjGraph::dff(ymuint pos) const
{
  ASSERT_COND( pos < dff_num() );
  return mDffList[pos];
}

END_NAMESPACE_YM_SBJ

#endif // SBJGRAPH_H
