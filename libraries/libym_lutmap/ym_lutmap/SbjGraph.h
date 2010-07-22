#ifndef YM_LUTMAP_SBJGRAPH_H
#define YM_LUTMAP_SBJGRAPH_H

/// @file ym_lutmap/SbjGraph.h
/// @brief SbjGraph のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: SbjGraph.h 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_lutmap/lutmap_nsdef.h"

#include "ym_utils/Alloc.h"
#include "ym_utils/DlList.h"
#include "ym_utils/ItvlMgr.h"


BEGIN_NAMESPACE_YM_LUTMAP

//////////////////////////////////////////////////////////////////////
/// @class SbjEdge SbjGraph.h "SbjGraph.h"
/// @brief サブジェクトグラフの枝を表すクラス
///
/// - 入力側のノード
/// - 出力側のノード
/// - 出力側のノードの何番目(0 or 1)の入力か
/// の情報を持つ．
/// @sa SbjNode SbjGraph
//////////////////////////////////////////////////////////////////////
class SbjEdge :
  public DlElem
{
  friend class SbjNode;
  friend class SbjGraph;
  friend class SbjEnumCut;
  
public:
  
  /// @brief 入力側のノードを得る．
  /// @retval 入力側のノード 通常の枝の場合
  /// @retval NULL 定数0に接続している枝の場合
  const SbjNode*
  from() const;
  
  /// @brief 出力側のノードを得る．
  /// @return 出力側のノードを返す．
  const SbjNode*
  to() const;
  
  /// @brief 入力側のノードを得る．
  /// @retval 入力側のノード 通常の枝の場合
  /// @retval NULL 定数0に接続している枝の場合
  SbjNode*
  from();
  
  /// @brief 出力側のノードを得る．
  /// @return 出力側のノードを返す．
  SbjNode*
  to();

  /// @brief 定数枝の時に true を返す．
  bool
  is_const() const;
  
  /// @brief 出力側のノードの何番目の入力かを示す．
  ymuint
  pos() const;

  /// @brief 出力ノードに接続している時 true を返す．
  bool
  is_poedge() const;


public:
  //////////////////////////////////////////////////////////////////////
  // max-flow algorithm 用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief flow が流れている時 true を返す．
  bool
  flow() const;

  /// @brief flow フラグをセットする．
  void
  set_flow();

  /// @brief flow フラグをクリアする．
  void
  clear_flow();
  

private:
  //////////////////////////////////////////////////////////////////////
  // プライベートメンバ関数
  // これらは friend class からしか呼ぶことができない．
  //////////////////////////////////////////////////////////////////////
  
  /// @brief コンストラクタ
  SbjEdge();
  
  /// @brief デストラクタ
  ~SbjEdge();

  /// @brief from ノードをセットする．
  void
  set_from(SbjNode* from);

  /// @brief to ノードをセットする．
  void
  set_to(SbjNode* to,
	 ymuint pos);


private:
  //////////////////////////////////////////////////////////////////////
  // プライベートメンバ変数
  //////////////////////////////////////////////////////////////////////
  
  // 入力側のノード
  SbjNode* mFrom;

  // 出力側のノード
  SbjNode* mTo;
  
  // bit-0: flow フラグ
  // bit-1: tree フラグ
  // bit-2: 入力位置
  ymuint32 mFlags;
  
  
private:
  //////////////////////////////////////////////////////////////////////
  // 定数
  //////////////////////////////////////////////////////////////////////

  static
  const int kFlowShift = 0;

  static
  const int kIposShift = 1;
  
  static
  const ymuint kFlowMask = (1U << kFlowShift);

  static
  const ymuint kIposMask = (1U << kIposShift);
  
};


//////////////////////////////////////////////////////////////////////
/// @class SbjNode SbjGraph.h "SbjGraph.h"
/// @brief サブジェクトグラフを構成するプリミティブノードを表すクラス
///
/// ノードの種類は
/// - 入力ノード
/// - 出力ノード
/// - 論理ノード
/// - DFFノード
/// の 4種類がある．
/// @sa SbjEdge SbjGraph
//////////////////////////////////////////////////////////////////////
class SbjNode :
  public DlElem
{
  friend class SbjGraph;
  
public:
  /// @brief ノードの型
  enum tType {
    /// @brief 入力ノード
    kINPUT  = 0,
    /// @brief 出力ノード
    kOUTPUT = 1,
    /// @brief 論理ノード
    kLOGIC = 2,
    /// @brief DFFノード
    kDFF = 3
  };


private:

  /// @brief コンストラクタ
  SbjNode();
  
  /// @brief デストラクタ
  ~SbjNode();

  
public:

  //////////////////////////////////////////////////////////////////////
  /// @name 構造に関する情報を取り出す関数
  /// @{
  
  /// @brief ID 番号の取得
  /// @return ID 番号を返す．
  /// @note ID 番号はノードの生成時に SbjGraph により自動的に割り振られる．
  /// @sa SbjGraph
  ymuint
  id() const;

  /// @brief ID を表す文字列の取得
  /// @note デバッグ時にしか意味を持たない
  string
  id_str() const;

  /// @brief 名前を得る．
  /// @note 名前がない場合もある．
  const string&
  name() const;

  /// @brief タイプを得る．
  tType
  type() const;
  
  /// @brief 入力ノードの時に true を返す．
  bool
  is_input() const;
  
  /// @brief 出力ノードの時に true を返す．
  bool
  is_output() const;
  
  /// @brief 論理ノードの時に true を返す．
  bool
  is_logic() const;
  
  /// @brief DFFノードの時に true を返す．
  bool
  is_dff() const;

  /// @brief 入力ノードか DFF ノードの時に true を返す．
  bool
  is_ppi() const;

  /// @brief 出力ノードか DFF ノードの時に true を返す．
  bool
  is_ppo() const;
  
  /// @brief サブID (入力／出力番号)を得る．
  /// @note 入力ノード/出力ノードの場合のみ意味を持つ．
  ymuint
  subid() const;

  /// @brief 出力ノードの極性を返す．
  /// @return 反転していたら true を返す．
  bool
  output_inv() const;
  
  /// @brief 機能コードを得る．
  /// @note 論理ノードの場合のみ意味を持つ．
  /// @note 機能コードは2入力の真理値表(4bit)
  ymuint
  fcode() const;
  
  /// @brief ファンインのノードを得る．
  /// @param[in] pos 入力番号(0 or 1)
  /// @return pos 番めのファンインのノード
  /// @note 該当するファンインがなければ NULL を返す．
  const SbjNode*
  fanin(ymuint pos) const;

  /// @brief ファンインのノードを得る．
  /// @param[in] pos 入力番号(0 or 1)
  /// @return pos 番めのファンインのノード
  /// @note 該当するファンインがなければ NULL を返す．
  SbjNode*
  fanin(ymuint pos);
  
  /// @brief ファンインの枝を得る．
  /// @param[in] pos 入力番号(0 or 1)
  /// @return pos 番目の入力の枝
  /// @note 該当するファンインの枝がなければ NULL を返す．
  const SbjEdge*
  fanin_edge(ymuint pos) const;
  
  /// @brief ファンインの枝を得る．
  /// @param[in] pos 入力番号(0 or 1)
  /// @return pos 番目の入力の枝
  /// @note 該当するファンインの枝がなければ NULL を返す．
  SbjEdge*
  fanin_edge(ymuint pos);
  
  /// @brief ファンアウト数を得る．
  ymuint
  n_fanout() const;
  
  /// @brief ファンアウトリストを得る．
  const SbjEdgeList&
  fanout_list() const;

  /// @brief fo マークを考慮した実効的なファンアウト数を返す．
  ymuint
  n_efo() const;

  /// @brief 出力ノードにファンアウトしているとき true を返す．
  bool
  pomark() const;
  
  /// @}
  //////////////////////////////////////////////////////////////////////

  
  //////////////////////////////////////////////////////////////////////
  /// @name cut 列挙用の関数
  /// @{

  /// @brief fanout マークを返す．
  bool
  is_fo() const;

  /// @brief fanout マークをつける．
  void
  set_fo();

  /// @brief fanout マークを消す．
  void
  clear_fo();

  /// @brief unselected マークを返す．
  bool
  is_unselected() const;

  /// @brief unselected マークをつける．
  void
  set_unselected();

  /// @brief unselected マークを消す．
  void
  clear_unselected();

  /// @brief range マークを返す．
  bool
  rmark() const;

  /// @brief range マークを付ける．
  void
  set_rmark();

  /// @brief range マークを消す．
  void
  clear_rmark();

  /// @brief target マークを返す．
  bool
  tmark() const;

  /// @brief target マークを付ける．
  void
  set_tmark();

  /// @brief target マークを消す．
  void
  clear_tmark();
  
  /// @brief flow 用のマークを返す．
  bool
  fmark() const;

  /// @brief flow 用のマークを付ける．
  void
  set_fmark();

  /// @brief flow 用のマークを消す．
  void
  clear_fmark();

  /// @brief 入力側の visit フラグを返す．
  bool
  vmark1();

  /// @brief 入力側の visit フラグをつける．
  void
  set_vmark1();

  /// @brief 入力側の visit フラグを消す．
  void
  clear_vmark1();

  /// @brief 出力側の visit フラグを返す．
  bool
  vmark2();

  /// @brief 出力側の visit フラグをつける．
  void
  set_vmark2();

  /// @brief 出力側の visit フラグを消す．
  void
  clear_vmark2();

  /// @brief 深さを得る．
  ymuint
  depth() const;

  /// @brief レベルを得る．
  ymuint
  level() const;

  /// @}
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // プライベートメンバ関数
  //////////////////////////////////////////////////////////////////////

  /// @brief タイプを入力に設定する．
  void
  set_input(ymuint subid);
  
  /// @brief タイプを出力に設定する．
  /// @param[in] inv 反転属性
  void
  set_output(ymuint subid,
	     bool inv);
  
  /// @brief タイプを論理に設定する．
  void
  set_logic(ymuint fcode);
  
  /// @brief タイプをDFFに設定する．
  /// @param[in] inv 反転属性
  void
  set_dff(bool inv);

  /// @brief 実効的なファンアウト数を計算する．
  void
  scan_efo(hash_set<ymuint>& mark) const;

  /// @brief ファンアウトに出力が含まれているか調べ pomark をセットする．
  void
  scan_po();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // ID 番号
  ymuint32 mId;

  // 名前
  string mName;

  // タイプ (+ 入力／出力番号/機能コード)
  ymuint32 mFlags;

  // ファンインの枝(そのもの)の配列
  SbjEdge mFanins[2];
  
  // ファンアウトの枝のリスト
  SbjEdgeList mFanoutList;

  // 実効的なファンアウト数
  ymuint32 mNefo;
  
  // 作業用のマーク
  ymuint32 mMark;
  
  // 深さ
  ymuint32 mDepth;

  // レベル
  ymuint32 mLevel;


private:
  //////////////////////////////////////////////////////////////////////
  // 定数の定義
  //////////////////////////////////////////////////////////////////////

  // mFlags の演算で用いる定数
  static
  const int kFoShift = 0;
  static
  const int kUnselShift = 1;
  static
  const int kRangeShift = 2;
  static
  const int kTargetShift = 3;
  static
  const int kFlowShift = 4;
  static
  const int kPoShift = 5;
  static
  const int kV1Shift = 6;
  static
  const int kV2Shift = 7;
  
  static
  const ymuint kFoMask = 1U << kFoShift;
  static
  const ymuint kUnselMask = 1U << kUnselShift;
  static
  const ymuint kRangeMask = 1U << kRangeShift;
  static
  const ymuint kTargetMask = 1U << kTargetShift;
  static
  const ymuint kFlowMask = 1U << kFlowShift;
  static
  const ymuint kPoMask = 1U << kPoShift;
  static
  const ymuint kV1Mask = 1U << kV1Shift;
  static
  const ymuint kV2Mask = 1U << kV2Shift;

};


//////////////////////////////////////////////////////////////////////
/// @class SbjPort SbjGraph.h "SbjGraph.h"
/// @brief ポートを表すクラス
///
/// ポートは通常，名前を持ち，1つもしくは複数の入出力ノード
/// と対応づけられる．
//////////////////////////////////////////////////////////////////////
class SbjPort
{
  friend class SbjGraph;

private:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ / デストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ
  /// @param[in] name 名前
  /// @param[in] body 入出力ノードのベクタ
  SbjPort(const string& name,
	  const vector<SbjNode*>& body);

  /// @brief デストラクタ
  ~SbjPort();


public:
  //////////////////////////////////////////////////////////////////////
  // 情報を取得する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前を得る．
  string
  name() const;

  /// @brief ビット数を得る．
  ymuint
  bit_width() const;

  /// @brief pos ビット目の内容を得る．
  /// @param[in] pos ビット位置 ( 0 <= pos < bit_width() )
  const SbjNode*
  bit(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 名前
  string mName;

  // 入出力ノードのベクタ
  vector<SbjNode*> mBody;

};


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
  /// @name ポート関連の情報の取得
  /// @{

  /// @brief ポート数を得る．
  ymuint
  port_num() const;

  /// @brief ポートを得る．
  /// @param[in] pos 位置 ( 0 <= pos < port_num() )
  const SbjPort*
  port(ymuint pos) const;

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name ポートの生成

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
  /// @retrun 該当するノードが無い場合には NULL を返す．
  const SbjNode*
  node(ymuint id) const;
  
  /// @brief 入力ノード数の取得
  /// @return 入力ノード数を返す．
  ymuint
  n_inputs() const;

  /// @brief 入力 ID 番号による入力ノードの取得
  /// @param[in] id 入力 ID 番号 ( 0 <= id < n_inputs() )
  /// @return 入力 ID 番号が id のノードを返す．
  SbjNode*
  input(ymuint id) const;

  /// @brief 入力ノードのリストを得る．
  const SbjNodeList&
  input_list() const;

  /// @brief 入力ノードと DFF ノードのリストを返す．
  /// @param[out] node_list ノードを格納するリスト
  /// @return 要素数を返す．
  ymuint
  ppi_list(list<SbjNode*>& node_list) const;

  /// @brief 出力のノード数を得る．
  ymuint
  n_outputs() const;

  /// @brief 出力 ID 番号による出力ノードの取得
  /// @param[in] id 出力 ID 番号 ( 0 <= id < n_outputs() )
  /// @return 出力 ID 番号が id のノードを返す．
  SbjNode*
  output(ymuint id) const;

  /// @brief 出力ノードのリストを得る．
  const SbjNodeList&
  output_list() const;

  /// @brief 出力ノードと DFF ノードのリストを返す．
  /// @param[out] node_list ノードを格納するリスト
  /// @return 要素数を返す．
  ymuint
  ppo_list(list<SbjNode*>& node_list) const;

  /// @brief 論理ノード数を得る．
  ymuint
  n_lnodes() const;
  
  /// @brief 論理ノードのリストを得る．
  const SbjNodeList&
  lnode_list() const;

  /// @brief DFFノード数を得る．
  ymuint
  n_dffs() const;
  
  /// @brief DFFノードのリストを得る．
  const SbjNodeList&
  dff_list() const;

  /// @brief ソートされた論理ノードのリストを得る．
  void
  sort(vector<SbjNode*>& node_list) const;

  /// @brief 逆順で論理ソートされたノードのリストを得る．
  void
  rsort(vector<SbjNode*>& node_list) const;

  /// @brief 段数を求める．
  ymuint
  level() const;

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name ノードの生成／変更
  /// @{

  /// @brief 入力ノードを作る．
  /// @param[in] name 名前
  /// @return 作成したノードを返す．
  SbjNode*
  new_input(const string& name);
  
  /// @brief 出力ノードを作る．
  /// @param[in] name 名前
  /// @param[in] inode 入力のノード
  /// @param[in] inv 極性
  /// @return 作成したノードを返す．
  SbjNode*
  new_output(const string& name,
	     SbjNode* inode,
	     bool inv);
  
  /// @brief 論理ノードを作る．
  /// @param[in] name 名前
  /// @param[in] fcode 機能コード
  /// @param[in] inode1 1番めの入力ノード
  /// @param[in] inode2 2番めの入力ノード
  /// @return 作成したノードを返す．
  SbjNode*
  new_logic(const string& name,
	    ymuint fcode,
	    SbjNode* inode1,
	    SbjNode* inode2);
  
  /// @brief DFFノードを作る．
  /// @param[in] name 名前
  /// @param[in] inode 入力のノード
  /// @param[in] inv 極性
  /// @return 作成したノードを返す．
  SbjNode*
  new_dff(const string& name,
	  SbjNode* inode = NULL,
	  bool inv = false);
  
  /// @brief 出力ノードの内容を変更する
  /// @param[in] 変更対象の出力ノード
  /// @param[in] inode 入力のノード
  /// @param[in] inv 極性
  void
  change_output(SbjNode* node,
		SbjNode* inode,
		bool inv);
  
  /// @brief 論理ノードの内容を再設定する．
  /// @param[in] node 変更対象の論理ノード
  /// @param[in] fcode 機能コード
  /// @param[in] inode1 1番めの入力ノード
  /// @param[in] inode2 2番めの入力ノード
  void
  change_logic(SbjNode* node,
	       ymuint fcode,
	       SbjNode* inode1,
	       SbjNode* inode2);
  
  /// @brief DFFノードの内容を変更する
  /// @param[in] 変更対象のDFFノード
  /// @param[in] inode 入力のノード
  /// @param[in] inv 極性
  void
  change_dff(SbjNode* node,
	     SbjNode* inode,
	     bool inv);
  
  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name minimum depth 関係の関数
  /// @{

  /// @brief 各ノードの minimum depth を求める．
  void
  get_min_depth(ymuint k) const;
  
  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name その他の関数
  /// @{

  /// @brief 空にする．
  void
  clear();
  
  /// @brief 内容を s に出力する．
  void
  dump(ostream& s) const;

  void
  set_efo() const;
  
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
  
  // 新しいノードを作成し mNodeList に登録する．
  // 作成されたノードを返す．
  SbjNode*
  new_node(const string& name);

  // new_node で用いられる低レベル関数
  SbjNode*
  alloc_node();

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
  delete_dff(SbjNode* node);
  
  // node を削除する．
  void
  delete_node(SbjNode* node);
  
  // from を to の pos 番目のファンインとする．
  // to の pos 番目にすでに接続があった場合には自動的に削除される．
  void
  connect(SbjNode* from,
	  SbjNode* to,
	  ymuint pos);

  // node を根とする深さ d - 1 の k-feasible cut が存在するかどうか調べる．
  bool
  find_k_cut(SbjNode* node,
	     ymuint k,
	     ymuint d) const;

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ノードを確保するためのアロケータ
  SimpleAlloc mAlloc;

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

  // 入力ノードのリスト
  SbjNodeList mInputList;

  // 出力番号をキーにした出力ノードの配列
  // 穴はあいていない．
  vector<SbjNode*> mOutputArray;

  // 出力ノードのリスト
  SbjNodeList mOutputList;
  
  // 論理ノードのリスト
  SbjNodeList mLnodeList;
  
  // DFFノードのリスト
  SbjNodeList mDffList;
  
  // 最大レベル
  mutable
  ymuint32 mLevel;

  // mLevel および各 SbjNode::mLevel が正しいとき true となるフラグ
  mutable
  bool mLevelValid;
  
};


/// @relates SbjGraph
/// @brief SbjGraph の内容をダンプする関数
void
dump(ostream& s,
     const SbjGraph& sbjgraph);

/// @relates SbjGraph
/// @brief SbjGraph の内容を blif 形式で出力する関数
void
write_blif(ostream& s,
	   const SbjGraph& sbjgraph,
	   const string& module_name = "sbjgraph");


//////////////////////////////////////////////////////////////////////
// inline 関数の定義
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// クラス SbjEdge
//////////////////////////////////////////////////////////////////////

// コンストラクタ
inline
SbjEdge::SbjEdge() :
  mFrom(NULL),
  mTo(NULL),
  mFlags(0U)
{
}

// デストラクタ
inline
SbjEdge::~SbjEdge()
{
}

// 入力側のノードを得る．
inline
const SbjNode*
SbjEdge::from() const
{
  return mFrom;
}

// 出力側のノードを得る．
inline
const SbjNode*
SbjEdge::to() const
{
  return mTo;
}

// 入力側のノードを得る．
inline
SbjNode*
SbjEdge::from()
{
  return mFrom;
}

// 出力側のノードを得る．
inline
SbjNode*
SbjEdge::to()
{
  return mTo;
}

// 出力側のノードの何番目の入力かを示す．
inline
ymuint
SbjEdge::pos() const
{
  return (mFlags >> kIposShift) & 1U;
}

// @brief from ノードをセットする．
inline
void
SbjEdge::set_from(SbjNode* from)
{
  mFrom = from;
}

// @brief to ノードをセットする．
inline
void
SbjEdge::set_to(SbjNode* to,
		ymuint pos)
{
  mTo = to;
  if ( pos ) {
    mFlags |= kIposMask;
  }
  else {
    mFlags &= ~kIposMask;
  }
}

// 定数枝の時に true を返す．
inline
bool
SbjEdge::is_const() const
{
  return mFrom == NULL;
}

// @brief 出力ノードに接続している時 true を返す．
inline
bool
SbjEdge::is_poedge() const
{
  return to()->is_output();
}

// @brief flow が流れている時 true を返す．
inline
bool
SbjEdge::flow() const
{
  return static_cast<bool>((mFlags >> kFlowShift) & 1U);
}

// @brief flow フラグをセットする．
inline
void
SbjEdge::set_flow()
{
  mFlags |= kFlowMask;
}

// @brief flow フラグをクリアする．
inline
void
SbjEdge::clear_flow()
{
  mFlags &= ~kFlowMask;
}


//////////////////////////////////////////////////////////////////////
// クラス SbjNode
//////////////////////////////////////////////////////////////////////

// ID 番号を得る．
inline
ymuint
SbjNode::id() const
{
  return mId;
}

// @brief 名前を得る．
// @note 名前がない場合もある．
inline
const string&
SbjNode::name() const
{
  return mName;
}

// タイプを入力に設定する．
inline
void
SbjNode::set_input(ymuint subid)
{
  mFlags = static_cast<ymuint>(kINPUT) | (subid << 4);
}

// タイプを出力に設定する．
inline
void
SbjNode::set_output(ymuint subid,
		    bool inv)
{
  mFlags = static_cast<ymuint>(kOUTPUT) | (subid << 4) | (inv << 3);
}

// タイプを論理に設定する．
inline
void
SbjNode::set_logic(ymuint fcode)
{
  mFlags = static_cast<ymuint>(kLOGIC) | (fcode << 3);
}

// @brief タイプをDFFに設定する．
inline
void
SbjNode::set_dff(bool inv)
{
  mFlags = static_cast<ymuint>(kDFF) | (inv << 3);
}

// タイプを得る．
inline
SbjNode::tType
SbjNode::type() const
{
  return static_cast<tType>(mFlags & 3U);
}

// 入力ノードの時に true を返す．
inline
bool
SbjNode::is_input() const
{
  return type() == kINPUT;
}

// 出力ノードの時に true を返す．
inline
bool
SbjNode::is_output() const
{
  return type() == kOUTPUT;
}

// 論理ノードの時に true を返す．
inline
bool
SbjNode::is_logic() const
{
  return type() == kLOGIC;
}

// DFFノードの時に true を返す．
inline
bool
SbjNode::is_dff() const
{
  return type() == kDFF;
}

// @brief 入力ノードか DFF ノードの時に true を返す．
inline
bool
SbjNode::is_ppi() const
{
  return type() == kINPUT || type() == kDFF;
}

// @brief 出力ノードか DFF ノードの時に true を返す．
inline
bool
SbjNode::is_ppo() const
{
  return type() == kOUTPUT || type() == kDFF;
}

// @brief サブID (入力／出力番号)を得る．
inline
ymuint
SbjNode::subid() const
{
  return mFlags >> 4;
}

// @brief 出力ノードの極性を得る．
inline
bool
SbjNode::output_inv() const
{
  return static_cast<bool>((mFlags >> 3) & 1U);
}

// @brief 機能コードを得る．
inline
ymuint
SbjNode::fcode() const
{
  return mFlags >> 3;
}

// @brief ファンインのノードを得る．
inline
const SbjNode*
SbjNode::fanin(ymuint pos) const
{
  // pos の範囲チェックはしていない！！！
  return mFanins[pos & 1U].from();
}

// @brief ファンインのノードを得る．
inline
SbjNode*
SbjNode::fanin(ymuint pos)
{
  // pos の範囲チェックはしていない！！！
  return mFanins[pos & 1U].from();
}

// ファンインの枝を得る．
inline
const SbjEdge*
SbjNode::fanin_edge(ymuint pos) const
{
  // pos の範囲チェックはしていない！！！
  return &mFanins[pos & 1U];
}

// ファンインの枝を得る．
inline
SbjEdge*
SbjNode::fanin_edge(ymuint pos)
{
  // pos の範囲チェックはしていない！！！
  return &mFanins[pos & 1U];
}

// ファンアウトリストを得る．
inline
const SbjEdgeList&
SbjNode::fanout_list() const
{
  return mFanoutList;
}

// ファンアウト数を得る．
inline
ymuint
SbjNode::n_fanout() const
{
  return mFanoutList.size();
}

// @brief fo マークを考慮した実効的なファンアウト数を返す．
inline
ymuint
SbjNode::n_efo() const
{
  return mNefo;
}

// @brief 出力ノードにファンアウトしているとき true を返す．
inline
bool
SbjNode::pomark() const
{
  return static_cast<bool>((mMark >> kPoShift) & 1U);
}

// @brief fanout マークを返す．
inline
bool
SbjNode::is_fo() const
{
  return static_cast<bool>((mMark >> kFoShift) & 1U);
}

// @brief fanout マークをつける．
inline
void
SbjNode::set_fo()
{
  mMark |= kFoMask;
}

// @brief fanout マークを消す．
inline
void
SbjNode::clear_fo()
{
  mMark &= ~kFoMask;
}

// @brief unselected マークを返す．
inline
bool
SbjNode::is_unselected() const
{
  return static_cast<bool>((mMark >> kUnselShift) & 1U);
}

// @brief unselected マークをつける．
inline
void
SbjNode::set_unselected()
{
  mMark |= kUnselMask;
}

// @brief unselected マークを消す．
inline
void
SbjNode::clear_unselected()
{
  mMark &= ~kUnselMask;
}

// @brief range マークを返す．
inline
bool
SbjNode::rmark() const
{
  return static_cast<bool>((mMark >> kRangeShift) & 1U);
}

// @brief range マークを付ける．
inline
void
SbjNode::set_rmark()
{
  mMark |= kRangeMask;
}

// @brief range マークを消す．
inline
void
SbjNode::clear_rmark()
{
  mMark &= ~kRangeMask;
}

// @brief target マークを返す．
inline
bool
SbjNode::tmark() const
{
  return static_cast<bool>((mMark >> kTargetShift) & 1U);
}

// @brief target マークを付ける．
inline
void
SbjNode::set_tmark()
{
  mMark |= kTargetMask;
}

// @brief target マークを消す．
inline
void
SbjNode::clear_tmark()
{
  mMark &= ~kTargetMask;
}

// @brief flow 用のマークを返す．
inline
bool
SbjNode::fmark() const
{
  return static_cast<bool>((mMark >> kFlowShift) & 1U);
}

// @brief flow 用のマークを付ける．
inline
void
SbjNode::set_fmark()
{
  mMark |= kFlowMask;
}

// @brief flow 用のマークを消す．
inline
void
SbjNode::clear_fmark()
{
  mMark &= ~kFlowMask;
}

// @brief 入力側の visit フラグを返す．
inline
bool
SbjNode::vmark1()
{
  return static_cast<bool>((mMark >> kV1Shift) & 1U);
}

// @brief 入力側の visit フラグをつける．
inline
void
SbjNode::set_vmark1()
{
  mMark |= kV1Mask;
}

// @brief 入力側の visit フラグを消す．
inline
void
SbjNode::clear_vmark1()
{
  mMark &= ~kV1Mask;
}

// @brief 出力側の visit フラグを返す．
inline
bool
SbjNode::vmark2()
{
  return static_cast<bool>((mMark >> kV2Shift) & 1U);
}

// @brief 出力側の visit フラグをつける．
inline
void
SbjNode::set_vmark2()
{
  mMark |= kV2Mask;
}

// @brief 出力側の visit フラグを消す．
inline
void
SbjNode::clear_vmark2()
{
  mMark &= ~kV2Mask;
}

// @brief 深さを得る．
inline
ymuint
SbjNode::depth() const
{
  return mDepth;
}

// @brief レベルを得る．
inline
ymuint
SbjNode::level() const
{
  return mLevel;
}


//////////////////////////////////////////////////////////////////////
// クラス SbjPort
//////////////////////////////////////////////////////////////////////

// @brief 名前を得る．
inline
string
SbjPort::name() const
{
  return mName;
}

// @brief ビット数を得る．
inline
ymuint
SbjPort::bit_width() const
{
  return mBody.size();
}

// @brief pos ビット目の内容を得る．
// @param[in] pos ビット位置 ( 0 <= pos < bit_width() )
inline
const SbjNode*
SbjPort::bit(ymuint pos) const
{
  return mBody[pos];
}


//////////////////////////////////////////////////////////////////////
// クラス SbjGraph
//////////////////////////////////////////////////////////////////////

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
// 該当するノードが無い場合には NULL を返す．
inline
const SbjNode*
SbjGraph::node(ymuint id) const
{
  return mNodeArray[id];
}

// 入力ノード数を得る．
inline
ymuint
SbjGraph::n_inputs() const
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

// @brief 入力ノードのリストを得る．
inline
const SbjNodeList&
SbjGraph::input_list() const
{
  return mInputList;
}

// 出力のノード数を得る．
inline
ymuint
SbjGraph::n_outputs() const
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

// @brief 入力ノードのリストを得る．
inline
const SbjNodeList&
SbjGraph::output_list() const
{
  return mOutputList;
}

// 論理ノード数を得る．
inline
ymuint
SbjGraph::n_lnodes() const
{
  return mLnodeList.size();
}

// 論理ノードのリストを得る．
inline
const SbjNodeList&
SbjGraph::lnode_list() const
{
  return mLnodeList;
}

// DFFノード数を得る．
inline
ymuint
SbjGraph::n_dffs() const
{
  return mDffList.size();
}

// DFFノードのリストを得る．
inline
const SbjNodeList&
SbjGraph::dff_list() const
{
  return mDffList;
}

END_NAMESPACE_YM_LUTMAP

#endif // YM_LUTMAP_SBJGRAPH_H
