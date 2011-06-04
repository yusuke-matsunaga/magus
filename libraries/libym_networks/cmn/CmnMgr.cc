
/// @file libym_networks/cmn/CmnMgr.cc
/// @brief CmnMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: LutNetwork.cc 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_networks/CmnMgr.h"
#include "CmnMgrImpl.h"


BEGIN_NAMESPACE_YM_CMN

///////////////////////////////////////////////////////////////////////
// クラス CmnMgr
///////////////////////////////////////////////////////////////////////

// コンストラクタ
CmnMgr::CmnMgr() :
  mImpl(new CmnMgrImpl)
{
}

// コピーコンストラクタ
CmnMgr::CmnMgr(const CmnMgr& src) :
  mImpl(new CmnMgrImpl)
{
  vector<CmnNode*> nodemap;
  mImpl->copy(*src.mImpl, nodemap);
}

// 代入演算子
const CmnMgr&
CmnMgr::operator=(const CmnMgr& src)
{
  if ( &src != this ) {
    clear();
    vector<CmnNode*> nodemap;
    mImpl->copy(*src.mImpl, nodemap);
  }
  return *this;
}

// デストラクタ
CmnMgr::~CmnMgr()
{
  clear();
  delete mImpl;
}

// @brief モジュール名を得る．
string
CmnMgr::name() const
{
  return mImpl->name();
}

// @brief ポート数を得る．
ymuint
CmnMgr::port_num() const
{
  return mImpl->port_num();
}

// @brief ポートを得る．
// @param[in] pos ポート位置 ( 0 <= pos < port_num() )
const CmnPort*
CmnMgr::port(ymuint pos) const
{
  return mImpl->port(pos);
}

// @brief モジュール名を設定する．
void
CmnMgr::set_name(const string& name)
{
  mImpl->set_name(name);
}

// @brief ポートを追加する(ベクタ版)．
// @param[in] name ポート名
// @param[in] io_node_vec 対応する入出力ノードのベクタ
void
CmnMgr::add_port(const string& name,
		 const vector<CmnNode*>& io_node_vec)
{
  mImpl->add_port(name, io_node_vec);
}

// @brief ノードIDの最大値 + 1 の取得
// @return ノードIDの最大値 + 1 を返す．
// @note ノードIDは間が抜けている場合がある．
ymuint
CmnMgr::max_node_id() const
{
  return mImpl->max_node_id();
}

// @brief ID 番号によるノードの取得
// @param[in] id ID 番号
// @return ID 番号が id のノードを返す．
// @retrun 該当するノードが無い場合には NULL を返す．
const CmnNode*
CmnMgr::node(ymuint id) const
{
  return mImpl->node(id);
}

// @brief 入力ノード数の取得
// @return 入力ノード数を返す．
ymuint
CmnMgr::input_num() const
{
  return mImpl->input_num();
}

// @brief 入力ノードのリストを得る．
const CmnNodeList&
CmnMgr::input_list() const
{
  return mImpl->input_list();
}

// @brief 出力のノード数を得る．
ymuint
CmnMgr::output_num() const
{
  return mImpl->output_num();
}

// @brief 出力ノードのリストを得る．
const CmnNodeList&
CmnMgr::output_list() const
{
  return mImpl->output_list();
}

// @brief 論理ノード数を得る．
ymuint
CmnMgr::logic_num() const
{
  return mImpl->logic_num();
}

// @brief 論理ノードのリストを得る．
const CmnNodeList&
CmnMgr::logic_list() const
{
  return mImpl->logic_list();
}

// @brief ソートされたノードのリストを得る．
void
CmnMgr::sort(vector<CmnNode*>& node_list) const
{
  mImpl->sort(node_list);
}

// @brief DFF数を得る．
ymuint
CmnMgr::dff_num() const
{
  return mImpl->dff_num();
}

// @brief DFFのリストを得る．
const CmnDffList&
CmnMgr::dff_list() const
{
  return mImpl->dff_list();
}

// @brief ラッチ数を得る．
ymuint
CmnMgr::latch_num() const
{
  return mImpl->latch_num();
}

// @brief ラッチのリストを得る．
const CmnLatchList&
CmnMgr::latch_list() const
{
  return mImpl->latch_list();
}

// 空にする．
void
CmnMgr::clear()
{
  mImpl->clear();
}

// @brief DFFを作る．
// @return 作成したノードを返す．
CmnDff*
CmnMgr::new_dff(const string& name)
{
  return mImpl->new_dff(name);
}

// @brief ラッチを作る．
// @return 作成したノードを返す．
CmnLatch*
CmnMgr::new_latch(const string& name)
{
  return mImpl->new_latch(name);
}

// @brief 外部入力を作る．
// @param[in] port ポート
// @param[in] bitpos ビット位置
// @return 作成したノードを返す．
// @note エラー条件は以下の通り
//  - bitpos が port のビット幅を越えている．
//  - port の bitpos にすでにノードがある．
CmnNode*
CmnMgr::new_port_input(CmnPort* port,
		       ymuint bitpos)
{
  return mImpl->new_port_input(port, bitpos);
}

// @brief 外部入力とポートを同時に作る．
// @param[in] port_name ポート名
// @return 作成したノードを返す．
// @note ポートのビット幅は1ビットとなる．
CmnNode*
CmnMgr::new_port_input(const string& port_name)
{
  return mImpl->new_port_input(port_name);
}

// @brief 外部出力ノードを作る．
// @param[in] port ポート
// @param[in] bitpos ビット位置
// @return 作成したノードを返す．
// @note エラー条件は以下の通り
//  - bitpos が port のビット幅を越えている．
//  - port の bitpos にすでにノードがある．
CmnNode*
CmnMgr::new_port_output(CmnPort* port,
			ymuint bitpos)
{
  return mImpl->new_port_output(port, bitpos);
}

// @brief 外部出力ノードとポートを同時にを作る．
// @param[in] port_name ポート名
// @return 作成したノードを返す．
// @note ポートのビット幅は1ビットとなる．
CmnNode*
CmnMgr::new_port_output(const string& port_name)
{
  return mImpl->new_port_output(port_name);
}

// @brief 論理ノードを作る．
// @param[in] inodes 入力ノードのベクタ
// @param[in] cell セル
// @return 作成したノードを返す．
CmnNode*
CmnMgr::new_logic(const vector<CmnNode*>& inodes,
		  const Cell* cell)
{
  return mImpl->new_logic(inodes, cell);
}

END_NAMESPACE_YM_CMN
