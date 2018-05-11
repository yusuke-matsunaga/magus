#ifndef SBJGRAPH_H
#define SBJGRAPH_H

/// @file SbjGraph.h
/// @brief SbjGraph のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2016, 2017, 2018 Yusuke Matsunaga
/// All rights reserved.


#include "sbj_nsdef.h"

#include "ym/logic.h"


BEGIN_NAMESPACE_YM_SBJ

class IOInfo;

//////////////////////////////////////////////////////////////////////
/// @class SbjGraph SbjGraph.h "SbjGraph.h"
/// @brief サブジェクトグラフを表すクラス
///
/// 入力ノード，出力ノード，論理ノードを分けてリストで持つ
/// ( @sa input_list(), output_list(), lnode_list() )．
/// また，すべてのノードに唯一な ID 番号を割り振っており，その ID 番号
/// からノードを取り出すこともできる( @sa node(ymuint id) )．
/// 入力ノード，出力ノードは ID 番号とは別に入力 ID 番号，および出力
/// ID 番号を持っており，それらの ID 番号からノードを取り出すこともできる．
/// ( @sa input(ymuint id), output(ymuint id) )
/// 論理ノードを入力からのトポロジカル順に並んでいる．
///
/// ノードの DAG の外側にポート，DFF，ラッチを表す SbjPort, SbjDff, SbjLatch
/// を持つ．
///
/// @sa SbjEdge SbjNode, SbjPort, SbjDff, SbjLatch
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
  /// @name 外部インターフェイス情報(ポート)の取得
  /// @{

  /// @brief モジュール名を得る．
  string
  name() const;

  /// @brief ポート数を得る．
  int
  port_num() const;

  /// @brief ポートを得る．
  /// @param[in] id ポートID ( 0 <= id < port_num() )
  const SbjPort*
  port(int id) const;

  /// @brief ポートのリストを得る．
  const vector<SbjPort*>&
  port_list() const;

  /// @brief 入出力ノードに関連づけられたポートを得る．
  /// @param[in] node 入出力ノード
  ///
  /// node がポートに関連付けられていない場合には nullptr を返す．
  const SbjPort*
  port(const SbjNode* node) const;

  /// @brief 入出力ノードのポートにおけるビット位置を得る．
  /// @param[in] node 入出力ノード
  ///
  /// node がポートに関連付けられていない場合には -1 を返す．
  int
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

  /// @brief ノード数を返す．
  int
  node_num() const;

  /// @brief ID 番号によるノードの取得
  /// @param[in] id ID 番号 ( 0 <= id < node_num() )
  /// @return ID 番号が id のノードを返す．
  const SbjNode*
  node(int id) const;

  /// @brief 入力ノード数の取得
  /// @return 入力ノード数を返す．
  int
  input_num() const;

  /// @brief 入力 ID 番号による入力ノードの取得
  /// @param[in] id 入力 ID 番号 ( 0 <= id < input_num() )
  /// @return 入力 ID 番号が id のノードを返す．
  const SbjNode*
  input(int id) const;

  /// @brief 入力ノードのリストを得る．
  const vector<const SbjNode*>&
  input_list() const;

  /// @brief 出力のノード数を得る．
  int
  output_num() const;

  /// @brief 出力 ID 番号による出力ノードの取得
  /// @param[in] id 出力 ID 番号 ( 0 <= id < output_num() )
  /// @return 出力 ID 番号が id のノードを返す．
  const SbjNode*
  output(int id) const;

  /// @brief 出力ノードのリストを得る．
  const vector<const SbjNode*>&
  output_list() const;

  /// @brief 論理ノード数を得る．
  int
  logic_num() const;

  /// @brief 論理ノードを得る．
  /// @param[in] pos 位置番号 ( 0 <= pos < logic_num() )
  const SbjNode*
  logic(int pos) const;

  /// @brief 論理ノードのリストを得る．
  const vector<const SbjNode*>&
  logic_list() const;

  /// @brief 段数を求める．
  /// @note 副作用として各 SbjNode のレベルが設定される．
  int
  level() const;

  /// @brief 各ノードの minimum depth を求める．
  /// @param[in] k LUT の最大入力数
  /// @param[out] depth_array 各ノードの深さを収める配列
  /// @return 出力の最大深さを返す．
  int
  get_min_depth(int k,
		vector<int>& depth_array) const;

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name DFF の情報の取得
  /// @{

  /// @brief DFFノード数を得る．
  int
  dff_num() const;

  /// @brief DFFノードを得る．
  /// @param[in] id DFF番号 ( 0 <= id < dff_num() )
  const SbjDff*
  dff(int pos) const;

  /// @brief DFFノードのリストを得る．
  const vector<const SbjDff*>&
  dff_list() const;

  /// @brief node に関連付けられている DFF を得る．
  /// @param[in] node 対象のノード
  ///
  /// node が DFF に関連付けられていない場合には nullptr を返す．
  const SbjDff*
  dff(const SbjNode* node) const;

  /// @brief node がDFFの入力だった時に true を返す．
  /// @param[in] node 対象のノード
  bool
  is_dff_input(const SbjNode* node) const;

  /// @brief node がDFFの出力だった時に true を返す．
  /// @param[in] node 対象のノード
  bool
  is_dff_output(const SbjNode* node) const;

  /// @brief node がDFFのクロック端子だった時に true を返す．
  /// @param[in] node 対象のノード
  bool
  is_dff_clock(const SbjNode* node) const;

  /// @brief node がDFFのクリア端子だった時に true を返す．
  /// @param[in] node 対象のノード
  bool
  is_dff_clear(const SbjNode* node) const;

  /// @brief node がDFFのセット端子だった時に true を返す．
  /// @param[in] node 対象のノード
  bool
  is_dff_preset(const SbjNode* node) const;


  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name ラッチの情報の取得
  /// @{

  /// @brief ラッチノード数を得る．
  int
  latch_num() const;

  /// @brief ラッチノードを得る．
  /// @param[in] id ラッチ番号 ( 0 <= id < latch_num() )
  const SbjLatch*
  latch(int id) const;

  /// @brief ラッチノードのリストを得る．
  const vector<const SbjLatch*>&
  latch_list() const;

  /// @brief node に関連付けられているラッチを返す．
  /// @param[in] node 対象のノード
  ///
  /// 関連付けられていないばあいには nullptr を返す．
  const SbjLatch*
  latch(const SbjNode* node) const;

  /// @brief node がラッチの入力だった場合に true を返す．
  /// @param[in] node 対象のノード
  bool
  is_latch_input(const SbjNode* node) const;

  /// @brief node がラッチの出力だった場合に true を返す．
  /// @param[in] node 対象のノード
  bool
  is_latch_output(const SbjNode* node) const;

  /// @brief node がラッチのイネーブル端子だった場合に true を返す．
  /// @param[in] node 対象のノード
  bool
  is_latch_enable(const SbjNode* node) const;

  /// @brief node がラッチのクリア端子だった場合に true を返す．
  /// @param[in] node 対象のノード
  bool
  is_latch_clear(const SbjNode* node) const;

  /// @brief node がラッチのセット端子だった場合に true を返す．
  /// @param[in] node 対象のノード
  bool
  is_latch_preset(const SbjNode* node) const;

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name ノードの生成／変更
  /// @{

  /// @brief 入力ノードを作る．
  /// @param[in] bipol 両極性が利用可能な時 true にするフラグ
  /// @return 作成したノードを返す．
  SbjNode*
  new_input(bool bipol);

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
  new_expr(const Expr& expr,
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

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name DFF/ラッチの生成を行う関数
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

  /// @brief ラッチノードを作る．
  /// @param[in] input データ入力
  /// @param[in] output データ出力
  /// @param[in] enable イネーブル端子
  /// @param[in] clear リセット端子
  /// @param[in] preset セット端子
  /// @return 作成したノードを返す．
  SbjLatch*
  new_latch(SbjNode* input,
	    SbjNode* output,
	    SbjNode* enable,
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

  /// @brief 複製する．
  /// @param[in] src コピー元のオブジェクト
  /// @param[out] nodemap ノード間の対応付けを表す配列
  ///
  /// nodemap はsrcのノード番号をキーにして新しいノードを保持する．
  void
  _copy(const SbjGraph& src,
	vector<SbjNode*>& nodemap);

  /// @brief 論理ノードを作る．
  /// @param[in] fcode 機能コード (真理値ベクタ)
  /// @param[in] inode1 1番めの入力ノード
  /// @param[in] inode2 2番めの入力ノード
  /// @return 作成したノードを返す．
  SbjNode*
  _new_logic(ymuint fcode,
	     SbjNode* inode1,
	     SbjNode* inode2);

  /// @brief new_and の下請け関数
  /// @param[in] ihandle_list 入力ハンドルのリスト
  /// @param[in] start 開始位置
  /// @param[in] num 要素数
  SbjHandle
  _new_and(const vector<SbjHandle>& ihandle_list,
	   int start,
	   int num);

  /// @brief new_or の下請け関数
  /// @param[in] ihandle_list 入力ハンドルのリスト
  /// @param[in] start 開始位置
  /// @param[in] num 要素数
  SbjHandle
  _new_or(const vector<SbjHandle>& ihandle_list,
	  int start,
	  int num);

  /// @brief new_xor の下請け関数
  /// @param[in] ihandle_list 入力ハンドルのリスト
  /// @param[in] start 開始位置
  /// @param[in] num 要素数
  SbjHandle
  _new_xor(const vector<SbjHandle>& ihandle_list,
	   int start,
	   int num);

  /// @brief 新しいノードを作成し mNodeList に登録する．
  /// @return 作成されたノードを返す．
  SbjNode*
  _new_node();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 名前
  string mName;

  // ポートの配列
  vector<SbjPort*> mPortArray;

  // ID 番号をキーにしたノードの配列
  // すべてのノードが格納される．
  vector<SbjNode*> mNodeArray;

  // 入力番号をキーにした入力ノードの配列
  // 穴はあいていない．
  vector<const SbjNode*> mInputArray;

  // 入力番号をキーにしたIO情報の配列
  vector<IOInfo*> mInputInfoArray;

  // 出力番号をキーにした出力ノードの配列
  // 穴はあいていない．
  vector<const SbjNode*> mOutputArray;

  // 出力番号をキーにしたIO情報の配列
  vector<IOInfo*> mOutputInfoArray;

  // 論理ノードのリスト
  vector<const SbjNode*> mLogicList;

  // DFFノードのリスト
  vector<const SbjDff*> mDffList;

  // ラッチノードのリスト
  vector<const SbjLatch*> mLatchList;

  // 最大レベル
  mutable
  int mLevel;

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
int
SbjGraph::port_num() const
{
  return mPortArray.size();
}

// @brief ポートを得る．
// @param[in] id ポート番号 ( 0 <= id < port_num() )
inline
const SbjPort*
SbjGraph::port(int id) const
{
  ASSERT_COND( id >= 0 && id < port_num() );

  return mPortArray[id];
}

// @brief ポートのリストを得る．
inline
const vector<SbjPort*>&
SbjGraph::port_list() const
{
  return mPortArray;
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
int
SbjGraph::node_num() const
{
  return mNodeArray.size();
}

// ID 番号が id のノードを取り出す．
// 該当するノードが無い場合には nullptr を返す．
inline
const SbjNode*
SbjGraph::node(int id) const
{
  ASSERT_COND( id >= 0 && id < node_num() );

  return mNodeArray[id];
}

// 入力ノード数を得る．
inline
int
SbjGraph::input_num() const
{
  return mInputArray.size();
}

// ID 番号が id の入力ノードを取り出す．
inline
const SbjNode*
SbjGraph::input(int id) const
{
  ASSERT_COND( id >= 0 && id < input_num() );

  return mInputArray[id];
}

// @brief 入力ノードのリストを得る．
inline
const vector<const SbjNode*>&
SbjGraph::input_list() const
{
  return mInputArray;
}

// 出力のノード数を得る．
inline
int
SbjGraph::output_num() const
{
  return mOutputArray.size();
}

// ID 番号が id の出力ノードを取り出す．
inline
const SbjNode*
SbjGraph::output(int id) const
{
  ASSERT_COND( id >= 0 && id < output_num() );

  return mOutputArray[id];
}

// @brief 出力ノードのリストを得る．
inline
const vector<const SbjNode*>&
SbjGraph::output_list() const
{
  return mOutputArray;
}

// 論理ノード数を得る．
inline
int
SbjGraph::logic_num() const
{
  return mLogicList.size();
}

// @brief 論理ノードを得る．
// @param[in] pos 位置番号 ( 0 <= pos < logic_num() )
inline
const SbjNode*
SbjGraph::logic(int pos) const
{
  ASSERT_COND( pos >= 0 && pos < logic_num() );

  return mLogicList[pos];
}

// @brief 論理ノードのリストを得る．
inline
const vector<const SbjNode*>&
SbjGraph::logic_list() const
{
  return mLogicList;
}

// DFFノード数を得る．
inline
int
SbjGraph::dff_num() const
{
  return mDffList.size();
}

// @brief DFFノードを得る．
// @param[in] id DFF番号 ( 0 <= id < dff_num() )
inline
const SbjDff*
SbjGraph::dff(int id) const
{
  ASSERT_COND( id >= 0 && id < dff_num() );

  return mDffList[id];
}

// @brief DFFノードのリストを得る．
inline
const vector<const SbjDff*>&
SbjGraph::dff_list() const
{
  return mDffList;
}

// ラッチノード数を得る．
inline
int
SbjGraph::latch_num() const
{
  return mLatchList.size();
}

// @brief ラッチノードを得る．
// @param[in] id ラッチ番号 ( 0 <= id < latch_num() )
inline
const SbjLatch*
SbjGraph::latch(int id) const
{
  ASSERT_COND( id >= 0 && id < latch_num() );

  return mLatchList[id];
}

// @brief ラッチノードのリストを得る．
inline
const vector<const SbjLatch*>&
SbjGraph::latch_list() const
{
  return mLatchList;
}

// @brief 段数を求める．
inline
int
SbjGraph::level() const
{
  return mLevel;
}

END_NAMESPACE_YM_SBJ

#endif // SBJGRAPH_H
