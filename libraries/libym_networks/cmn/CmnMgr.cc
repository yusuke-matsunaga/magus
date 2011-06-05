
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
  mImpl->copy(*src.mImpl);
}

// 代入演算子
const CmnMgr&
CmnMgr::operator=(const CmnMgr& src)
{
  if ( &src != this ) {
    clear();
    mImpl->copy(*src.mImpl);
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

// @brief D-FFのIDの最大値 + 1 の取得
ymuint
CmnMgr::max_dff_id() const
{
  return mImpl->max_dff_id();
}

// @brief ID番号から D-FF を得る．
// @param[in] id ID番号 ( 0 <= id < max_dff_id() )
// @note 該当するD-FFが無い場合には NULL を返す．
const CmnDff*
CmnMgr::dff(ymuint id) const
{
  return mImpl->dff(id);
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

// @brief ラッチのIDの最大値 + 1 の取得
ymuint
CmnMgr::max_latch_id() const
{
  return mImpl->max_latch_id();
}

// @brief ID番号からラッチを得る．
// @param[in] id ID番号 ( 0 <= id < max_latch_id() )
// @note 該当するラッチが無い場合には NULL を返す．
const CmnLatch*
CmnMgr::latch(ymuint id) const
{
  return mImpl->latch(id);
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
CmnMgr::sort(vector<const CmnNode*>& node_list) const
{
  mImpl->sort(node_list);
}

// @brief D-FFセルの情報を得る．
// @param[in] cell 対象のセル
// @note cell が D-FF でない場合と登録されていない場合には NULL を返す．
const CmnDffCell*
CmnMgr::dff_cell(const Cell* cell) const
{
  return mImpl->dff_cell(cell);
}

// @brief ラッチセルの情報を得る．
// @param[in] cell 対象のセル
// @note cell がラッチでない場合と登録されていない場合には NULL を返す．
const CmnLatchCell*
CmnMgr::latch_cell(const Cell* cell) const
{
  return mImpl->latch_cell(cell);
}

// 空にする．
void
CmnMgr::clear()
{
  mImpl->clear();
}

// @brief モジュール名を設定する．
void
CmnMgr::set_name(const string& name)
{
  mImpl->set_name(name);
}

// @brief 入力ポートを生成する．
// @param[in] name ポート名
// @param[in] bit_width ビット幅
CmnPort*
CmnMgr::new_input_port(const string& name,
		       ymuint bit_width)
{
  return mImpl->new_port(name, vector<ymuint>(bit_width, 1));
}

// @brief 出力ポートを生成する．
// @param[in] name ポート名
// @param[in] bit_width ビット幅
CmnPort*
CmnMgr::new_output_port(const string& name,
			ymuint bit_width)
{
  return mImpl->new_port(name, vector<ymuint>(bit_width, 2));
}

// @brief ポートを生成する．
// @param[in] name ポート名
// @param[in] iovect ビットごとの方向を指定する配列
// @note iovect の要素の値の意味は以下の通り
// - 0 : なし
// - 1 : 入力のみ
// - 2 : 出力のみ
// - 3 : 入力と出力
CmnPort*
CmnMgr::new_port(const string& name,
		 const vector<ymuint>& iovect)
{
  return mImpl->new_port(name, iovect);
}

// @brief DFFを作る．
// @param[in] cell セル
// @param[in] name 名前
// @return 作成したノードを返す．
CmnDff*
CmnMgr::new_dff(const CmnDffCell* cell,
		const string& name)
{
  return mImpl->new_dff(cell, name);
}

// @brief ラッチを作る．
// @param[in] cell セル
// @param[in] name 名前
// @return 作成したノードを返す．
CmnLatch*
CmnMgr::new_latch(const CmnLatchCell* cell,
		  const string& name)
{
  return mImpl->new_latch(cell, name);
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

// @brief D-FFセルを登録する．
// @param[in] cell 対象のセル．
// @param[in] pos_array ピン情報の配列
// @return D-FFセルの情報を表すオブジェクトを返す．
// @note pos_array の意味は以下の通り
//  - pos_array[0] : データ入力のピン番号     (3bit)
//  - pos_array[1] : クロック入力のピン番号   (3bit)
//  - pos_array[2] : クリア入力のピン番号     (3bit) | ピン情報 (2bit)
//  - pos_array[3] : プリセット入力のピン番号 (3bit) | ピン情報 (2bit)
//  - pos_array[4] : 肯定出力のピン番号       (3bit)
//  - pos_array[5] : 否定出力のピン番号       (3bit)
const CmnDffCell*
CmnMgr::reg_dff_cell(const Cell* cell,
		     ymuint pos_array[])
{
  return mImpl->reg_dff_cell(cell, pos_array);
}

// @brief ラッチセルを登録する．
// @param[in] cell 対象のセル．
// @param[in] pos_array ピン情報の配列
// @return ラッチセルの情報を表すオブジェクトを返す．
// @note pos_array の意味は以下の通り
//  - pos_array[0] : データ入力のピン番号     (3bit)
//  - pos_array[1] : イネーブル入力のピン番号 (3bit) | ピン情報 (2bit)
//  - pos_array[2] : クリア入力のピン番号     (3bit) | ピン情報 (2bit)
//  - pos_array[3] : プリセット入力のピン番号 (3bit) | ピン情報 (2bit)
//  - pos_array[4] : 肯定出力のピン番号       (3bit)
//  - pos_array[5] : 否定出力のピン番号       (3bit)
const CmnLatchCell*
CmnMgr::reg_latch_cell(const Cell* cell,
		       ymuint pos_array[])
{
  return mImpl->reg_latch_cell(cell, pos_array);
}

END_NAMESPACE_YM_CMN
