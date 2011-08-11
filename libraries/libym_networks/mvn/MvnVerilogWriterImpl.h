#ifndef VERILOGWRITERIMPL_H
#define VERILOGWRITERIMPL_H

/// @file VerilogWriterImpl.h
/// @brief VerilogWriterImpl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.

#include "ym_networks/mvn.h"


BEGIN_NAMESPACE_YM_NETWORKS_MVN

class NodeDumper;

//////////////////////////////////////////////////////////////////////
/// @class VerilogWriterImpl VerilogWriterImpl.h
/// @brief MvnVerilogWriter の実際の処理を行う実装クラス
//////////////////////////////////////////////////////////////////////
class VerilogWriterImpl
{
public:

  /// @brief コンストラクタ
  VerilogWriterImpl();

  /// @brief デストラクタ
  ~VerilogWriterImpl();


public:
  //////////////////////////////////////////////////////////////////////
  // MvnVerilogWriter から使われる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を Verilog-HDL 形式で出力する．
  /// @param[in] s 出力先のストリーム
  /// @param[in] mgr MvnMgr
  void
  dump(ostream& s,
       const MvnMgr& mgr);

  /// @brief ノードと Verilog 名の対応関係をコメントで出力する．
  /// @param[in] s 出力先のストリーム
  /// @param[in] mgr MvnMgr
  /// @param[in] node_map ノードと Verilog 名の対応表
  void
  dump_map(ostream& s,
	   const MvnMgr& mgr,
	   const MvnVlMap& node_map);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で使われる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief モジュールを出力する．
  /// @param[in] s 出力先のストリーム
  /// @param[in] module 対象のモジュール
  void
  dump_module(ostream& s,
	      const MvnModule* module);

  /// @brief ノードを出力する．
  /// @param[in] s 出力先のストリーム
  /// @param[in] node 対象のノード
  void
  dump_node(ostream& s,
	    const MvnNode* node);

  /// @brief 組み合わせ回路用のノードの内容を出力する．
  /// @param[in] s 出力先のストリーム
  /// @param[in] node 対象のノード
  /// @param[in] dumper ノードの内容を出力するためのクラス
  void
  dump_comb(ostream& s,
	    const MvnNode* node,
	    const NodeDumper& dumper);

  /// @brief D-FF 用のノードの内容を出力する．
  /// @param[in] s 出力先のストリーム
  /// @param[in] node 対象のノード
  void
  dump_dff(ostream& s,
	   const MvnNode* node);

  /// @brief ラッチ用のノードの内容を出力する．
  /// @param[in] s 出力先のストリーム
  /// @param[in] node 対象のノード
  void
  dump_latch(ostream& s,
	     const MvnNode* node);

  /// @brief ノードを表す右辺式を出力する．
  /// @param[in] s 出力先のストリーム
  /// @param[in] node 対象のノード
  void
  dump_node_rhs(ostream& s,
		const MvnNode* node);

  /// @brief ノードIDの最大値を設定する．
  /// @param[in] max_id ノードIDの最大値
  void
  set_max_id(ymuint max_id);

  /// @brief 出力ピンに対応するネットIDを返す．
  /// @param[in] pin 出力ピン
  ymuint
  net_id(const MvnOutputPin* pin);

  /// @brief 出力ピンに対応するネット名を返す．
  /// @param[in] pin 出力ピン
  string
  net_name(const MvnOutputPin* pin);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 使用可能な番号
  ymuint32 mNextId;

  // ノードID + ピン位置をキーにして番号を格納する配列
  vector<vector<ymuint32> > mMap;

};

END_NAMESPACE_YM_NETWORKS_MVN

#endif // VERILOGWRITERIMPL_H
