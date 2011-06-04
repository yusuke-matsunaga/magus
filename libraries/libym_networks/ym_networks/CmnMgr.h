#ifndef YM_NETWORKS_CMNMGR_H
#define YM_NETWORKS_CMNMGR_H

/// @file ym_networks/CmnMgr.h
/// @brief CmnMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: LutNetwork.h 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_networks/cmn_nsdef.h"
#include "ym_cell/cell_nsdef.h"


BEGIN_NAMESPACE_YM_CMN

//////////////////////////////////////////////////////////////////////
/// @class CmnMgr CmnMgr.h "ym_networks/CmnMgr.h"
/// @brief セルネットワークを表すクラス
///
/// 入力ノード，出力ノード，セルノード，DFF ノードを分けてリストで持つ
/// ( @sa input_list(), output_list(), cellnode_list(), dff_list() )．
/// また，すべてのノードに唯一な ID 番号を割り振っており，その ID 番号
/// からノードを取り出すこともできる( @sa node(ymuint id) )．
/// 入力ノード，出力ノードは ID 番号とは別に入力 ID 番号，および出力
/// ID 番号を持っており，それらの ID 番号からノードを取り出すこともできる．
/// ( @sa input(ymuint id), output(ymuint id) )
/// セルノードを入力からのトポロジカル順で処理するためには sort()
/// を用いてソートされたノードのベクタを得る．
/// @sa CmnEdge CmnNode CmnPort
//////////////////////////////////////////////////////////////////////
class CmnMgr
{
public:

  /// @brief コンストラクタ
  CmnMgr();

  /// @brief コピーコンストラクタ
  /// @param[in] src コピー元のオブジェクト
  CmnMgr(const CmnMgr& src);

  /// @brief 代入演算子
  /// @param[in] src コピー元のオブジェクト
  /// @return 自分自身への参照を返す．
  const CmnMgr&
  operator=(const CmnMgr& src);

  /// @brief デストラクタ
  ~CmnMgr();


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

  /// @brief ポートを追加する(1ビット版)．
  /// @param[in] name ポート名
  /// @param[in] io_node 対応する入出力ノード
  void
  add_port(const string& name,
	   CmnNode* io_node);

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
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 実体
  CmnMgrImpl* mImpl;

};

/// @relates CmnMgr
/// @brief 内容を s に出力する．
/// @param[in] s 出力先のストリーム
/// @param[in] lngraph 対象のネットワーク
void
dump(ostream& s,
     const CmnMgr& lngraph);

/// @relates CmnMgr
/// @brief 内容をシミュレーション可能な Verilog-HDL 形式で出力する．
/// @param[in] s 出力先のストリーム
/// @param[in] cngraph 対象のネットワーク
void
dump_verilog(ostream& s,
	     const CmnMgr& cngraph);


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief ポートを追加する(1ビット版)．
// @param[in] name ポート名
// @param[in] io_node 対応する入出力ノード
inline
void
CmnMgr::add_port(const string& name,
		 CmnNode* io_node)
{
  add_port(name, vector<CmnNode*>(1, io_node));
}

END_NAMESPACE_YM_CMN

#endif // YM_NETWORKS_CMNMGR_H
