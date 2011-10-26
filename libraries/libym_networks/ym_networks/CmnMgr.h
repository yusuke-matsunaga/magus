#ifndef YM_NETWORKS_CMNMGR_H
#define YM_NETWORKS_CMNMGR_H

/// @file ym_networks/CmnMgr.h
/// @brief CmnMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_networks/cmn.h"
#include "ym_cell/cell_nsdef.h"


BEGIN_NAMESPACE_YM_NETWORKS_CMN

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
/// @sa CmnEdge CmnNode CmnPort CmnDff CmnLatch
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

  /// @brief D-FF数を得る．
  ymuint
  dff_num() const;

  /// @brief D-FFのリストを得る．
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
  /// @return FFセルの情報を表すオブジェクトを返す．
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

  /// @brief 入力ポートを生成する．
  /// @param[in] name ポート名
  /// @param[in] bit_width ビット幅
  CmnPort*
  new_input_port(const string& name,
		 ymuint bit_width);

  /// @brief 出力ポートを生成する．
  /// @param[in] name ポート名
  /// @param[in] bit_width ビット幅
  CmnPort*
  new_output_port(const string& name,
		  ymuint bit_width);

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

  /// @brief DFFを作る．
  /// @param[in] cell セル
  /// @param[in] name 名前
  /// @return 作成したノードを返す．
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
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 実体
  CmnMgrImpl* mImpl;

};

END_NAMESPACE_YM_NETWORKS_CMN

#endif // YM_NETWORKS_CMNMGR_H
