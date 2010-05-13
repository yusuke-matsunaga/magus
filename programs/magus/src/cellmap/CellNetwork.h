#ifndef MAGUS_CELLMAP_CELLNETWORK_H
#define MAGUS_CELLMAP_CELLNETWORK_H

/// @file magus/cellmap/CellNetwork.h 
/// @brief CellNetwork のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: CellNetwork.h 1978 2009-02-06 12:29:16Z matsunaga $
///
/// Copyright (C) 2005-2007 Yusuke Matsunaga
/// All rights reserved.

#include <ym_utils/Alloc.h>
#include <ym_utils/DlList.h>
#include <ym_utils/ItvlMgr.h>
#include <ym_lexp/LogExpr.h>

#include "cellmap.h"


BEGIN_NAMESPACE_MAGUS_CELLMAP

class CnEdge;
class CnNode;
class CnGraph;

typedef DlList<CnEdge> CnEdgeList;
typedef DlListConstIter<CnEdge> CnEdgeListConstIter;
typedef DlList<CnNode> CnNodeList;
typedef DlListConstIter<CnNode> CnNodeListConstIter;


//////////////////////////////////////////////////////////////////////
/// @class CnEdge CnGraph.h "CnGraph.h"
/// @brief CELLネットワークの枝を表すクラス
///
/// - 入力側のノード
/// - 出力側のノード
/// - 出力側のノードの何番目の入力か
/// の情報を持つ．
/// @sa CnNode CnGraph
//////////////////////////////////////////////////////////////////////
class CnEdge :
  public DlElem
{
  friend class CnNode;
  friend class CnGraph;
  friend class LnEnumCut;
  
public:

  //////////////////////////////////////////////////////////////////////
  /// @name 関数処理に関する情報にアクセスする関数
  /// @{
  
  /// @brief 入力側のノードを得る．
  /// @retval 入力側のノード 通常の枝の場合
  /// @retval NULL 定数0に接続している枝の場合
  const CnNode*
  from() const;
  
  /// @brief 出力側のノードを得る．
  /// @return 出力側のノードを返す．
  const CnNode*
  to() const;
  
  /// @brief 入力側のノードを得る．
  /// @retval 入力側のノード 通常の枝の場合
  /// @retval NULL 定数0に接続している枝の場合
  CnNode*
  from();
  
  /// @brief 出力側のノードを得る．
  /// @return 出力側のノードを返す．
  CnNode*
  to();
  
  /// @brief 出力側のノードの何番目の入力かを示す．
  size_t
  pos() const;

  /// @brief 出力ノードに接続している時 true を返す．
  bool
  is_poedge() const;

  /// @}
  //////////////////////////////////////////////////////////////////////
  
  
private:
  //////////////////////////////////////////////////////////////////////
  // プライベートメンバ関数
  // これらは friend class からしか呼ぶことができない．
  //////////////////////////////////////////////////////////////////////
  
  /// @brief コンストラクタ
  CnEdge();
  
  /// @brief デストラクタ
  ~CnEdge();

  /// @brief from ノードをセットする．
  void
  set_from(CnNode* from);

  /// @brief to ノードをセットする．
  void
  set_to(CnNode* to,
	 size_t pos);


private:
  //////////////////////////////////////////////////////////////////////
  // プライベートメンバ変数
  //////////////////////////////////////////////////////////////////////
  
  // 入力側のノード
  CnNode* mFrom;

  // 出力側のノード
  CnNode* mTo;
  
  // 入力位置
  ymuint32 mIpos;
  
};


//////////////////////////////////////////////////////////////////////
/// @class CnNode CnGraph.h "CnGraph.h"
/// @brief CELLネットワークを構成するCELLノードを表すクラス
///
/// ノードの種類は
/// - 入力ノード
/// - 出力ノード
/// - CELLノード
/// の 3種類がある．
/// @sa CnEdge CnGraph
//////////////////////////////////////////////////////////////////////
class CnNode :
  public DlElem
{
  friend class CnGraph;
  
public:
  /// @brief ノードの型
  enum tType {
    /// @brief 入力ノード
    kINPUT  = 0,
    /// @brief 出力ノード
    kOUTPUT = 1,
    /// @brief CELLノード
    kCELL = 2
  };


private:

  /// @brief コンストラクタ
  CnNode();
  
  /// @brief デストラクタ
  ~CnNode();

  
public:

  //////////////////////////////////////////////////////////////////////
  /// @name 構造に関する情報を取り出す関数
  /// @{
  
  /// @brief ID 番号の取得
  /// @return ID 番号を返す．
  /// @note ID 番号はノードの生成時に CnGraph により自動的に割り振られる．
  /// @sa CnGraph
  size_t
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
  
  /// @brief CELLノードの時に true を返す．
  bool
  is_cell() const;

  /// @brief サブID (入力／出力番号)を得る．
  /// @note 入力ノード/出力ノードの場合のみ意味を持つ．
  size_t
  subid() const;

  /// @brief 入力数を得る．
  size_t
  ni() const;

  /// @brief ファンインのノードを得る．
  /// @param[in] pos 入力番号
  /// @return pos 番めのファンインのノード
  /// @note 該当するファンインがなければ NULL を返す．
  const CnNode*
  fanin(size_t pos) const;

  /// @brief ファンインのノードを得る．
  /// @param[in] pos 入力番号
  /// @return pos 番めのファンインのノード
  /// @note 該当するファンインがなければ NULL を返す．
  CnNode*
  fanin(size_t pos);
  
  /// @brief ファンインの枝を得る．
  /// @param[in] pos 入力番号
  /// @return pos 番目の入力の枝
  /// @note 該当するファンインの枝がなければ NULL を返す．
  const CnEdge*
  fanin_edge(size_t pos) const;
  
  /// @brief ファンインの枝を得る．
  /// @param[in] pos 入力番号
  /// @return pos 番目の入力の枝
  /// @note 該当するファンインの枝がなければ NULL を返す．
  CnEdge*
  fanin_edge(size_t pos);
  
  /// @brief ファンアウト数を得る．
  size_t
  n_fanout() const;
  
  /// @brief ファンアウトリストを得る．
  const CnEdgeList&
  fanout_list() const;

  /// @brief 出力ノードにファンアウトしているとき true を返す．
  bool
  pomark() const;
  
  /// @brief CELLノードの場合に論理式を得る．
  LogExpr
  expr() const;
  
  /// @brief CELLノードの場合に真理値ベクタを得る．
  /// @param[out] tv 真理値を格納するベクタ
  void
  tv(vector<int>& tv) const;

  /// @}
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // プライベートメンバ関数
  //////////////////////////////////////////////////////////////////////

  /// @brief タイプを入力に設定する．
  void
  set_input(size_t subid = 0);
  
  /// @brief タイプを出力に設定する．
  void
  set_output(size_t subid = 0);
  
  /// @brief タイプをCELLに設定する．
  void
  set_cell();
  
  /// @brief ファンアウトに出力が含まれているか調べ pomark をセットする．
  void
  scan_po();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // ID 番号
  size_t mId;

  // 名前
  string mName;

  // タイプ (+ 入力／出力番号)
  ymuint32 mFlags;

  // 入力数
  size_t mNi;
  
  // ファンインの枝(そのもの)の配列
  CnEdge* mFanins;
  
  // ファンアウトの枝のリスト
  CnEdgeList mFanoutList;
  
  // 論理式
  LogExpr mExpr;
  
  // 作業用のマーク
  ymuint32 mMark;

  
private:
  //////////////////////////////////////////////////////////////////////
  // mFlags 用の定数
  //////////////////////////////////////////////////////////////////////

  static
  const int kPoShift = 5;

  static
  const ymuint32 kPoMask = 1U << kPoShift;

};


//////////////////////////////////////////////////////////////////////
/// @class CnGraph CnGraph.h "CnGraph.h"
/// @brief CELLネットワークを表すクラス
///
/// 入力ノード，出力ノード，CELL ノードを分けてリストで持つ
/// ( @sa input_list(), output_list(), lnode_list() )．
/// また，すべてのノードに唯一な ID 番号を割り振っており，その ID 番号
/// からノードを取り出すこともできる( @sa node(size_t id) )．
/// 入力ノード，出力ノードは ID 番号とは別に入力 ID 番号，および出力
/// ID 番号を持っており，それらの ID 番号からノードを取り出すこともできる．
/// ( @sa input(size_t id), output(size_t id) )
/// binary ノードを入力からのトポロジカル順で処理するためには sort()
/// を用いてソートされたノードのベクタを得る．
/// @sa CnEdge CnNode
//////////////////////////////////////////////////////////////////////
class CnGraph
{
public:

  /// @brief コンストラクタ
  CnGraph();

  /// @brief コピーコンストラクタ
  CnGraph(const CnGraph& src);

  /// @brief 特殊なコピーコンストラクタ
  /// nodemap に対応関係が入る．
  CnGraph(const CnGraph& src,
	  vector<CnNode*>& nodemap);

  /// @brief 代入演算子
  const CnGraph&
  operator=(const CnGraph& src);

  /// @brief デストラクタ
  ~CnGraph();

  
  //////////////////////////////////////////////////////////////////////
  /// @name ノード関連の情報の取得
  /// @{

  /// @brief ノードIDの最大値 + 1 の取得
  /// @return ノードIDの最大値 + 1 を返す．
  size_t
  max_node_id() const;
  
  /// @brief ID 番号によるノードの取得
  /// @param[in] id ID 番号
  /// @return ID 番号が id のノードを返す．
  /// @retrun 該当するノードが無い場合には NULL を返す．
  CnNode*
  node(size_t id) const;
  
  /// @brief 入力ノード数の取得
  /// @return 入力ノード数を返す．
  size_t
  n_inputs() const;

  /// @brief 入力 ID 番号による入力ノードの取得
  /// @param[in] id 入力 ID 番号
  /// @return 入力 ID 番号が id のノードを返す．
  CnNode*
  input(size_t id) const;

  /// @brief 入力ノードのリストを得る．
  const CnNodeList&
  input_list() const;

  /// @brief 出力のノード数を得る．
  size_t
  n_outputs() const;

  /// @brief 出力 ID 番号による出力ノードの取得
  /// @param[in] id 出力 ID 番号
  /// @return 出力 ID 番号が id のノードを返す．
  CnNode*
  output(size_t id) const;

  /// @brief 入力ノードのリストを得る．
  const CnNodeList&
  output_list() const;

  /// @brief CELLノード数を得る．
  size_t
  n_cnodes() const;
  
  /// @brief CELLノードのリストを得る．
  const CnNodeList&
  cnode_list() const;

  /// @brief ソートされたノードのリストを得る．
  /// @param[out] node_list
  /// @note 入力ノードと出力ノードは含まない．
  void
  sort(vector<CnNode*>& node_list) const;

  /// @}
  //////////////////////////////////////////////////////////////////////

  
  //////////////////////////////////////////////////////////////////////
  /// @name ノードの生成／変更
  /// @{

  /// @brief 入力ノードを作る．
  /// @param[in] name 名前
  /// @return 作成したノードを返す．
  CnNode*
  new_input(const string& name);
  
  /// @brief 出力ノードを作る．
  /// @param[in] name 名前
  /// @param[in] inode 入力のノード
  /// @return 作成したノードを返す．
  CnNode*
  new_output(const string& name,
	     CnNode* inode);
  
  /// @brief CELLノードを作る．
  /// @param[in] name 名前
  /// @param[in] inodes 入力ノードのベクタ
  /// @param[in] expr 論理式
  /// @return 作成したノードを返す．
  CnNode*
  new_cell(const string& name,
	   const vector<CnNode*>& inodes,
	   const LogExpr& expr);

  /// @brief 入力ノードの削除
  /// @param[in] node 対象のノード
  void
  delete_input(CnNode* node);

  /// @brief 出力ノードの削除
  /// @param[in] node 対象のノード
  void
  delete_output(CnNode* node);
  
  /// @brief CELLノードを削除する．
  /// @param[in] node 対象のノード
  /// @note node のファンアウトは空でなければならない．
  void
  delete_cell(CnNode* node);
  
  /// @brief 出力ノードの内容を変更する
  /// @param[in] 変更対象の出力ノード
  /// @param[in] inode 入力のノード
  void
  change_output(CnNode* node,
		CnNode* inode);
  
  /// @brief CELLノードの内容を再設定する．
  /// @param[in] node 変更対象の論理ノード
  /// @param[in] inodes 入力ノードのベクタ
  /// @param[in] expr 論理式
  void
  change_cell(CnNode* node,
	      const vector<CnNode*>& inodes,
	      const LogExpr& expr);
  
  /// @brief CELL ノードのファンインのみ変更する．
  /// @param[in] node 変更対象の論理ノード
  /// @param[in] inodes 入力ノードのベクタ
  void
  change_node_fanins(CnNode* node,
		     const vector<CnNode*>& inodes);
  
  /// @brief ノードの置き換えを行う．
  /// @param[in] old_node 置き換え対象のノード
  /// @param[in] new_node 置き換わる新しいノード
  /// @param[in] inv true なら極性を反転させる．
  void
  subst_node(CnNode* old_node,
	     CnNode* new_node,
	     bool inv = false);
  
  /// @}
  //////////////////////////////////////////////////////////////////////

  
  //////////////////////////////////////////////////////////////////////
  /// @name その他の関数
  /// @{

  /// @brief 空にする．
  void
  clear();
  
  /// @brief 内容を s に出力する．
  void
  dump(ostream& s) const;
  
  /// @}
  //////////////////////////////////////////////////////////////////////
  

private:
  //////////////////////////////////////////////////////////////////////
  // プライベートメンバ関数
  //////////////////////////////////////////////////////////////////////

  // 複製する．
  void
  copy(const CnGraph& src,
       vector<CnNode*>& nodemap);
  
  // 新しいノードを作成し mNodeList に登録する．
  // 作成されたノードを返す．
  CnNode*
  new_node(size_t ni);

  // new_node で用いられる低レベル関数
  CnNode*
  alloc_node();
  
  // node を削除する．
  void
  delete_node(CnNode* node);
  
  // from を to の pos 番目のファンインとする．
  // to の pos 番目にすでに接続があった場合には自動的に削除される．
  void
  connect(CnNode* from,
	  CnNode* to,
	  size_t pos);
  
  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ノードを確保するためのアロケータ
  SimpleAlloc mAlloc;

  // ノードのファンインの枝の配列を確保するためのアロケータ
  FragAlloc mAlloc2;
  
  // ID 番号をキーにしたノードの配列
  // すべてのノードが格納される．
  vector<CnNode*> mNodeArray;

  // ID 番号を管理するためのオブジェクト
  ItvlMgr mItvlMgr;

  // 入力番号をキーにした入力ノードの配列
  // 穴はあいていない．
  vector<CnNode*> mInputArray;

  // 入力ノードのリスト
  CnNodeList mInputList;

  // 出力番号をキーにした出力ノードの配列
  // 穴はあいていない．
  vector<CnNode*> mOutputArray;

  // 出力ノードのリスト
  CnNodeList mOutputList;
  
  // CELLノードのリスト
  CnNodeList mCnodeList;
  
};


//////////////////////////////////////////////////////////////////////
// inline 関数の定義
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// クラス CnEdge
//////////////////////////////////////////////////////////////////////

// コンストラクタ
inline
CnEdge::CnEdge() :
  mFrom(NULL),
  mTo(NULL),
  mIpos(0)
{
}

// デストラクタ
inline
CnEdge::~CnEdge()
{
}

// 入力側のノードを得る．
inline
const CnNode*
CnEdge::from() const
{
  return mFrom;
}

// 出力側のノードを得る．
inline
const CnNode*
CnEdge::to() const
{
  return mTo;
}

// 入力側のノードを得る．
inline
CnNode*
CnEdge::from()
{
  return mFrom;
}

// 出力側のノードを得る．
inline
CnNode*
CnEdge::to()
{
  return mTo;
}

// 出力側のノードの何番目の入力かを示す．
inline
size_t
CnEdge::pos() const
{
  return mIpos;
}

// @brief from ノードをセットする．
inline
void
CnEdge::set_from(CnNode* from)
{
  mFrom = from;
}

// @brief to ノードをセットする．
inline
void
CnEdge::set_to(CnNode* to,
	       size_t pos)
{
  mTo = to;
  mIpos = pos;
}

// @brief 出力ノードに接続している時 true を返す．
inline
bool
CnEdge::is_poedge() const
{
  return to()->is_output();
}


//////////////////////////////////////////////////////////////////////
// クラス CnNode
//////////////////////////////////////////////////////////////////////

// ID 番号を得る．
inline
size_t
CnNode::id() const
{
  return mId;
}

// @brief 名前を得る．
// @note 名前がない場合もある．
inline
const string&
CnNode::name() const
{
  return mName;
}
  
// @brief CELL ノードの場合に論理式を得る．
inline
LogExpr
CnNode::expr() const
{
  return mExpr;
}

// タイプを入力に設定する．
inline
void
CnNode::set_input(size_t subid)
{
  mFlags = static_cast<size_t>(kINPUT) | (subid << 3);
}

// タイプを出力に設定する．
inline
void
CnNode::set_output(size_t subid)
{
  mFlags = static_cast<size_t>(kOUTPUT) | (subid << 3);
}

// タイプをCELLに設定する．
inline
void
CnNode::set_cell()
{
  mFlags = static_cast<size_t>(kCELL);
}

// タイプを得る．
inline
CnNode::tType
CnNode::type() const
{
  return static_cast<tType>(mFlags & 3);
}

// 入力ノードの時に true を返す．
inline
bool
CnNode::is_input() const
{
  return type() == kINPUT;
}

// 出力ノードの時に true を返す．
inline
bool
CnNode::is_output() const
{
  return type() == kOUTPUT;
}

// CELLノードの時に true を返す．
inline
bool
CnNode::is_cell() const
{
  return type() == kCELL;
}

// @brief サブID (入力／出力番号)を得る．
inline
size_t
CnNode::subid() const
{
  return mFlags >> 3;
}

// @brief 入力数を得る．
inline
size_t
CnNode::ni() const
{
  return mNi;
}

// @brief ファンインのノードを得る．
inline
const CnNode*
CnNode::fanin(size_t pos) const
{
  // pos の範囲チェックはしていない！！！
  return mFanins[pos].from();
}

// @brief ファンインのノードを得る．
inline
CnNode*
CnNode::fanin(size_t pos)
{
  // pos の範囲チェックはしていない！！！
  return mFanins[pos].from();
}

// ファンインの枝を得る．
inline
const CnEdge*
CnNode::fanin_edge(size_t pos) const
{
  // pos の範囲チェックはしていない！！！
  return &mFanins[pos];
}

// ファンインの枝を得る．
inline
CnEdge*
CnNode::fanin_edge(size_t pos)
{
  // pos の範囲チェックはしていない！！！
  return &mFanins[pos];
}

// ファンアウトリストを得る．
inline
const CnEdgeList&
CnNode::fanout_list() const
{
  return mFanoutList;
}

// ファンアウト数を得る．
inline
size_t
CnNode::n_fanout() const
{
  return mFanoutList.size();
}

// @brief 出力ノードにファンアウトしているとき true を返す．
inline
bool
CnNode::pomark() const
{
  return static_cast<bool>((mMark >> kPoShift) & 1U);
}


//////////////////////////////////////////////////////////////////////
// クラス CnGraph
//////////////////////////////////////////////////////////////////////

// ノード番号の最大値 + 1 を返す．
inline
size_t
CnGraph::max_node_id() const
{
  return mNodeArray.size();
}

// ID 番号が id のノードを取り出す．
// 該当するノードが無い場合には NULL を返す．
inline
CnNode*
CnGraph::node(size_t id) const
{
  return mNodeArray[id];
}

// 入力ノード数を得る．
inline
size_t
CnGraph::n_inputs() const
{
  return mInputArray.size();
}

// ID 番号が id の入力ノードを取り出す．
inline
CnNode*
CnGraph::input(size_t id) const
{
  return mInputArray[id];
}

// @brief 入力ノードのリストを得る．
inline
const CnNodeList&
CnGraph::input_list() const
{
  return mInputList;
}

// 出力のノード数を得る．
inline
size_t
CnGraph::n_outputs() const
{
  return mOutputArray.size();
}

// ID 番号が id の出力ノードを取り出す．
inline
CnNode*
CnGraph::output(size_t id) const
{
  return mOutputArray[id];
}

// @brief 入力ノードのリストを得る．
inline
const CnNodeList&
CnGraph::output_list() const
{
  return mOutputList;
}

// CELLノード数を得る．
inline
size_t
CnGraph::n_cnodes() const
{
  return mCnodeList.size();
}

// CELLノードのリストを得る．
inline
const CnNodeList&
CnGraph::cnode_list() const
{
  return mCnodeList;
}

END_NAMESPACE_MAGUS_CELLMAP

#endif // MAGUS_CELLMAP_CELLNETWORK_H
