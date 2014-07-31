#ifndef CMNMGRIMPL_H
#define CMNMGRIMPL_H

/// @file CmnMgrImpl.h
/// @brief CmnMgrImpl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmNetworks/cmn.h"
#include "YmCell/cell_nsdef.h"

#include "YmUtils/FragAlloc.h"
#include "YmUtils/DlList.h"
#include "YmUtils/ItvlMgr.h"


BEGIN_NAMESPACE_HASH
// Cell へのポインタをキーにしたハッシュ関数クラスの定義
template <>
struct hash<const nsYm::Cell*>
{
  ymuint
  operator()(const nsYm::Cell* cell) const
  {
    return reinterpret_cast<ympuint>(cell)/sizeof(void*);
  }
};
END_NAMESPACE_HASH


BEGIN_NAMESPACE_YM_NETWORKS_CMN

//////////////////////////////////////////////////////////////////////
/// @class CmnMgrImpl CmnMgrImpl.h "CmnMgrImpl.h"
/// @brief CmnMgr の実装を行うクラス
//////////////////////////////////////////////////////////////////////
class CmnMgrImpl
{
public:

  /// @brief コンストラクタ
  CmnMgrImpl();

  /// @brief デストラクタ
  ~CmnMgrImpl();

  // 複製する．
  void
  copy(const CmnMgrImpl& src);


public:
  //////////////////////////////////////////////////////////////////////
  /// @name 情報の取得
  /// @{

  /// @brief モジュール名を得る．
  string
  name() const;

  /// @brief ポート数を得る．
  ymuint
  port_num() const;

  /// @brief ポートを得る．
  /// @param[in] pos ポート位置 ( 0 <= pos < port_num() )
  const CmnPort*
  port(ymuint pos) const;

  /// @brief D-FFのIDの最大値 + 1 の取得
  ymuint
  max_dff_id() const;

  /// @brief ID番号から D-FF を得る．
  /// @param[in] id ID番号 ( 0 <= id < max_dff_id() )
  /// @note 該当するD-FFが無い場合には NULL を返す．
  const CmnDff*
  dff(ymuint id) const;

  /// @brief DFF数を得る．
  ymuint
  dff_num() const;

  /// @brief DFFのリストを得る．
  const CmnDffList&
  dff_list() const;

  /// @brief ラッチのIDの最大値 + 1 の取得
  ymuint
  max_latch_id() const;

  /// @brief ID番号からラッチを得る．
  /// @param[in] id ID番号 ( 0 <= id < max_latch_id() )
  /// @note 該当するラッチが無い場合には NULL を返す．
  const CmnLatch*
  latch(ymuint id) const;

  /// @brief ラッチ数を得る．
  ymuint
  latch_num() const;

  /// @brief ラッチのリストを得る．
  const CmnLatchList&
  latch_list() const;

  /// @brief ノードIDの最大値 + 1 の取得
  /// @return ノードIDの最大値 + 1 を返す．
  /// @note ノードIDは間が抜けている場合がある．
  ymuint
  max_node_id() const;

  /// @brief ID 番号によるノードの取得
  /// @param[in] id ID 番号
  /// @return ID 番号が id のノードを返す．
  /// @retrun 該当するノードが無い場合には NULL を返す．
  const CmnNode*
  node(ymuint id) const;

  /// @brief 入力ノード数の取得
  /// @return 入力ノード数を返す．
  ymuint
  input_num() const;

  /// @brief 入力ノードのリストを得る．
  const CmnNodeList&
  input_list() const;

  /// @brief 出力のノード数を得る．
  ymuint
  output_num() const;

  /// @brief 出力ノードのリストを得る．
  const CmnNodeList&
  output_list() const;

  /// @brief 論理ノード数を得る．
  ymuint
  logic_num() const;

  /// @brief 論理ノードのリストを得る．
  const CmnNodeList&
  logic_list() const;

  /// @brief ソートされた論理ノードのリストを得る．
  /// @param[out] node_list
  void
  sort(vector<const CmnNode*>& node_list) const;

  /// @brief D-FFセルの情報を得る．
  /// @param[in] cell 対象のセル
  /// @note cell が D-FF でない場合と登録されていない場合には NULL を返す．
  const CmnDffCell*
  dff_cell(const Cell* cell) const;

  /// @brief ラッチセルの情報を得る．
  /// @param[in] cell 対象のセル
  /// @note cell がラッチでない場合と登録されていない場合には NULL を返す．
  const CmnLatchCell*
  latch_cell(const Cell* cell) const;

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name 情報の設定
  /// @{

  /// @brief 空にする．
  void
  clear();

  /// @brief モジュール名を設定する．
  void
  set_name(const string& name);

  /// @brief D-FFセルを登録する．
  /// @param[in] cell 対象のセル．
  /// @param[in] pin_info ピン情報
  /// @return D-FFセルの情報を表すオブジェクトを返す．
  const CmnDffCell*
  reg_dff_cell(const Cell* cell,
	       const CellFFInfo& pin_info);

  /// @brief ラッチセルを登録する．
  /// @param[in] cell 対象のセル．
  /// @param[in] pin_info ピン情報
  /// @return ラッチセルの情報を表すオブジェクトを返す．
  const CmnLatchCell*
  reg_latch_cell(const Cell* cell,
		 const CellLatchInfo& pin_info);

  /// @brief ポートを生成する．
  /// @param[in] name ポート名
  /// @param[in] iovect ビットごとの方向を指定する配列
  /// @note iovect の要素の値の意味は以下の通り
  /// - 0 : なし
  /// - 1 : 入力のみ
  /// - 2 : 出力のみ
  /// - 3 : 入力と出力
  CmnPort*
  new_port(const string& name,
	   const vector<ymuint>& iovect);

  /// @brief D-FFを作る．
  /// @param[in] cell セル
  /// @param[in] name 名前
  /// @return 作成したD-FFを返す．
  CmnDff*
  new_dff(const CmnDffCell* cell,
	  const string& name = string());

  /// @brief ラッチを作る．
  /// @param[in] cell セル
  /// @param[in] name 名前
  /// @return 作成したノードを返す．
  CmnLatch*
  new_latch(const CmnLatchCell* cell,
	    const string& name = string());

  /// @brief 論理ノードを作る．
  /// @param[in] inodes 入力ノードのベクタ
  /// @param[in] cell セル
  /// @return 作成したノードを返す．
  CmnNode*
  new_logic(const vector<CmnNode*>& inodes,
	    const Cell* cell);

  /// @brief 出力ノードのファンインを設定する．
  /// @param[in] output 出力ノード
  /// @param[in] fanin ファンインのノード
  void
  set_output_fanin(CmnNode* output,
		   CmnNode* fanin);

  /// @}
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // プライベートメンバ関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力ノードを登録する．
  void
  reg_input(CmnNode* node);

  /// @brief 出力ノードを登録する．
  void
  reg_output(CmnNode* node);

  /// @breif ノードを登録して ID番号を割り振る．
  void
  reg_node(CmnNode* node);

  // node の登録を削除する．
  void
  unreg_node(CmnNode* node);

#if 0
  // 新しいノードを作成し mNodeList に登録する．
  // 作成されたノードを返す．
  CmnNode*
  new_node(ymuint ni);

  /// @brief D-FFを削除する．
  /// @param[in] dff 対象のD-FF
  void
  delete_dff(CmnDff* dff);

  /// @brief ラッチを削除する．
  /// @param[in] latch 対象のラッチ
  void
  delete_latch(CmnLatch* latch);

  /// @brief 入力ノードの削除
  /// @param[in] node 対象のノード
  void
  delete_input(CmnNode* node);

  /// @brief 出力ノードの削除
  /// @param[in] node 対象のノード
  void
  delete_output(CmnNode* node);

  /// @brief セルノードを削除する．
  /// @param[in] node 対象のノード
  /// @note node のファンアウトは空でなければならない．
  void
  delete_logic(CmnNode* node);
#endif

  // from を to の pos 番目のファンインとする．
  // to の pos 番目にすでに接続があった場合には自動的に削除される．
  void
  connect(CmnNode* from,
	  CmnNode* to,
	  ymuint pos);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // メモリを確保するためのアロケータ
  FragAlloc mAlloc;

  // モジュール名
  string mName;

  // ポートの配列
  vector<CmnPort*> mPortArray;

  // ID 番号をキーにした D-FF の配列
  vector<CmnDff*> mDffArray;

  // D-FF のID番号を管理するためのオブジェクト
  ItvlMgr mDffItvlMgr;

  // D-FFのリスト
  CmnDffList mDffList;

  // ID 番号をキーにしたラッチの配列
  vector<CmnLatch*> mLatchArray;

  // ラッチの ID番号を管理するためのオブジェクト
  ItvlMgr mLatchItvlMgr;

  // ラッチのリスト
  CmnLatchList mLatchList;

  // ID 番号をキーにしたノードの配列
  // すべてのノードが格納される．
  vector<CmnNode*> mNodeArray;

  // ノードの ID 番号を管理するためのオブジェクト
  ItvlMgr mNodeItvlMgr;

  // 入力ノードのリスト
  CmnNodeList mInputList;

  // 出力ノードのリスト
  CmnNodeList mOutputList;

  // 論理ノードのリスト
  CmnNodeList mLogicList;

  typedef unordered_map<const Cell*, const CmnDffCell*> CellDffMap;

  // cell のアドレスをキーにして CmnDffCell を記憶するハッシュ表
  CellDffMap mDffCellMap;

  typedef unordered_map<const Cell*, const CmnLatchCell*> CellLatchMap;

  // cell のアドレスをキーにして CmnLatchCell を記憶するハッシュ表
  CellLatchMap mLatchCellMap;

};


//////////////////////////////////////////////////////////////////////
// inline 関数の定義
//////////////////////////////////////////////////////////////////////

// @brief モジュール名を得る．
inline
string
CmnMgrImpl::name() const
{
  return mName;
}

// @brief ポート数を得る．
inline
ymuint
CmnMgrImpl::port_num() const
{
  return mPortArray.size();
}

// @brief ポートを得る．
// @param[in] pos ポート位置 ( 0 <= pos < port_num() )
inline
const CmnPort*
CmnMgrImpl::port(ymuint pos) const
{
  return mPortArray[pos];
}

// @brief D-FFのIDの最大値 + 1 の取得
inline
ymuint
CmnMgrImpl::max_dff_id() const
{
  return mDffArray.size();
}

// @brief ID番号から D-FF を得る．
// @param[in] id ID番号 ( 0 <= id < max_dff_id() )
inline
const CmnDff*
CmnMgrImpl::dff(ymuint id) const
{
  return mDffArray[id];
}

// @brief DFFノード数を得る．
inline
ymuint
CmnMgrImpl::dff_num() const
{
  return mDffList.size();
}

// @brief DFFノードのリストを得る．
inline
const CmnDffList&
CmnMgrImpl::dff_list() const
{
  return mDffList;
}

// @brief ラッチのIDの最大値 + 1 の取得
inline
ymuint
CmnMgrImpl::max_latch_id() const
{
  return mLatchArray.size();
}

// @brief ID番号からラッチを得る．
// @param[in] id ID番号 ( 0 <= id < max_latch_id() )
inline
const CmnLatch*
CmnMgrImpl::latch(ymuint id) const
{
  return mLatchArray[id];
}

// @brief ラッチ数を得る．
inline
ymuint
CmnMgrImpl::latch_num() const
{
  return mLatchList.size();
}

// @brief ラッチのリストを得る．
inline
const CmnLatchList&
CmnMgrImpl::latch_list() const
{
  return mLatchList;
}

// ノード番号の最大値 + 1 を返す．
inline
ymuint
CmnMgrImpl::max_node_id() const
{
  return mNodeArray.size();
}

// ID 番号が id のノードを取り出す．
// 該当するノードが無い場合には NULL を返す．
inline
const CmnNode*
CmnMgrImpl::node(ymuint id) const
{
  return mNodeArray[id];
}

// 入力ノード数を得る．
inline
ymuint
CmnMgrImpl::input_num() const
{
  return mInputList.size();
}

// @brief 入力ノードのリストを得る．
inline
const CmnNodeList&
CmnMgrImpl::input_list() const
{
  return mInputList;
}

// 出力のノード数を得る．
inline
ymuint
CmnMgrImpl::output_num() const
{
  return mOutputList.size();
}

// @brief 入力ノードのリストを得る．
inline
const CmnNodeList&
CmnMgrImpl::output_list() const
{
  return mOutputList;
}

// 論理ノード数を得る．
inline
ymuint
CmnMgrImpl::logic_num() const
{
  return mLogicList.size();
}

// 論理ノードのリストを得る．
inline
const CmnNodeList&
CmnMgrImpl::logic_list() const
{
  return mLogicList;
}

END_NAMESPACE_YM_NETWORKS_CMN

#endif // CMNMGRIMPL_H
