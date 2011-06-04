#ifndef LIBYM_NETWORKS_CMN_CMNMGR_H
#define LIBYM_NETWORKS_CMN_CMNMGR_H

/// @file libym_networks/cmn/CmnMgrImpl.h
/// @brief CmnMgrImpl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: LutNetwork.h 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_networks/cmn_nsdef.h"
#include "ym_cell/cell_nsdef.h"

#include "ym_utils/Alloc.h"
#include "ym_utils/DlList.h"
#include "ym_utils/ItvlMgr.h"


BEGIN_NAMESPACE_YM_CMN

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
  copy(const CmnMgrImpl& src,
       vector<CmnNode*>& nodemap);


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
  /// @param[in] pos ポート位置 ( 0 <= pos < port_num() )
  const CmnPort*
  port(ymuint pos) const;

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name 外部インターフェイス情報の設定
  /// @{

  /// @brief モジュール名を設定する．
  void
  set_name(const string& name);

  /// @brief ポートを追加する(ベクタ版)．
  /// @param[in] name ポート名
  /// @param[in] io_node_vec 対応する入出力ノードのベクタ
  void
  add_port(const string& name,
	   const vector<CmnNode*>& io_node_vec);

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name ノード関連の情報の取得
  /// @{

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
  sort(vector<CmnNode*>& node_list) const;

  /// @brief DFF数を得る．
  ymuint
  dff_num() const;

  /// @brief DFFのリストを得る．
  const CmnDffList&
  dff_list() const;

  /// @brief ラッチ数を得る．
  ymuint
  latch_num() const;

  /// @brief ラッチのリストを得る．
  const CmnLatchList&
  latch_list() const;

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name ノードの生成／変更
  /// @{

  /// @brief 空にする．
  void
  clear();

  /// @brief ポートを作る．
  /// @param[in] name 名前
  /// @param[in] bit_width ビット幅
  CmnPort*
  new_port(const string& name,
	   ymuint bit_width);

  /// @brief DFFを作る．
  /// @return 作成したノードを返す．
  CmnDff*
  new_dff(const string& name = string());

  /// @brief ラッチを作る．
  /// @return 作成したノードを返す．
  CmnLatch*
  new_latch(const string& name = string());

  /// @brief 外部入力を作る．
  /// @param[in] port ポート
  /// @param[in] bitpos ビット位置
  /// @return 作成したノードを返す．
  /// @note エラー条件は以下の通り
  ///  - bitpos が port のビット幅を越えている．
  ///  - port の bitpos にすでにノードがある．
  CmnNode*
  new_port_input(CmnPort* port,
		 ymuint bitpos);

  /// @brief 外部入力とポートを同時に作る．
  /// @param[in] port_name ポート名
  /// @return 作成したノードを返す．
  /// @note ポートのビット幅は1ビットとなる．
  CmnNode*
  new_port_input(const string& port_name);

  /// @brief 外部出力ノードを作る．
  /// @param[in] port ポート
  /// @param[in] bitpos ビット位置
  /// @return 作成したノードを返す．
  /// @note エラー条件は以下の通り
  ///  - bitpos が port のビット幅を越えている．
  ///  - port の bitpos にすでにノードがある．
  CmnNode*
  new_port_output(CmnPort* port,
		  ymuint bitpos);

  /// @brief 外部出力ノードとポートを同時にを作る．
  /// @param[in] port_name ポート名
  /// @return 作成したノードを返す．
  /// @note ポートのビット幅は1ビットとなる．
  CmnNode*
  new_port_output(const string& port_name);

  /// @brief 論理ノードを作る．
  /// @param[in] inodes 入力ノードのベクタ
  /// @param[in] cell セル
  /// @return 作成したノードを返す．
  CmnNode*
  new_logic(const vector<CmnNode*>& inodes,
	    const Cell* cell);

  /// @}
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // プライベートメンバ関数
  //////////////////////////////////////////////////////////////////////

  // 新しいノードを作成し mNodeList に登録する．
  // 作成されたノードを返す．
  CmnNode*
  new_node(ymuint ni);

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
  delete_cellnode(CmnNode* node);

  /// @brief DFFノードを削除する．
  /// @param[in] node 対象のノード
  /// @note node のファンアウトは空でなければならない．
  void
  delete_dff(CmnNode* node);

  // node を削除する．
  void
  delete_node(CmnNode* node);

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

  // ノードを確保するためのアロケータ
  SimpleAlloc mAlloc;

  // ノードのファンインの枝の配列を確保するためのアロケータ
  FragAlloc mAlloc2;

  // モジュール名
  string mName;

  // ポートの配列
  vector<CmnPort*> mPortArray;

  // ID 番号をキーにしたノードの配列
  // すべてのノードが格納される．
  vector<CmnNode*> mNodeArray;

  // ID 番号を管理するためのオブジェクト
  ItvlMgr mItvlMgr;

  // 入力ノードのリスト
  CmnNodeList mInputList;

  // 出力ノードのリスト
  CmnNodeList mOutputList;

  // 論理ノードのリスト
  CmnNodeList mLogicList;

  // DFFのリスト
  CmnDffList mDffList;

  // ラッチのリスト
  CmnLatchList mLatchList;

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

END_NAMESPACE_YM_CMN

#endif // YM_NETWORKS_CMNMGR_H
