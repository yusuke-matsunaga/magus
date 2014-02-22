#ifndef VERILOGWRITERIMPL_H
#define VERILOGWRITERIMPL_H

/// @file VerilogWriterImpl.h
/// @brief VerilogWriterImpl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "networks/mvn.h"


BEGIN_NAMESPACE_YM_NETWORKS_MVN

//////////////////////////////////////////////////////////////////////
/// @class VerilogWriterImpl VerilogWriterImpl.h
/// @brief MvnVerilogWriter の実際の処理を行うクラス
//////////////////////////////////////////////////////////////////////
class VerilogWriterImpl
{
public:

  /// @brief コンストラクタ
  VerilogWriterImpl();

  /// @brief デストラクタ
  ~VerilogWriterImpl();


public:

  /// @brief 内容を出力する．
  /// @param[in] s 出力先のストリーム
  /// @param[in] mgr MvnMgr
  void
  dump(ostream& s,
       const MvnMgr& mgr);

  /// @brief 内容を出力する．
  /// @param[in] s 出力先のストリーム
  /// @param[in] mgr MvnMgr
  /// @param[in] node_map ノードと Verilog 名の対応表
  void
  dump(ostream& s,
       const MvnMgr& mgr,
       const MvnVlMap& node_map);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で使われる関数
  //////////////////////////////////////////////////////////////////////

  void
  dump_module(ostream& s,
	      const MvnModule* module,
	      const MvnMgr& mgr);

  void
  dump_port(ostream& s,
	    const MvnPort* port);

  void
  dump_port_ref(ostream& s,
		const MvnPortRef* port_ref);

  void
  dump_node(ostream& s,
	    const MvnNode* node);

  void
  dump_uop(ostream& s,
	   const MvnNode* node,
	   const char* opr_str);

  void
  dump_binop(ostream& s,
	     const MvnNode* node,
	     const char* opr_str,
	     bool need_paren = false);

  void
  dump_nop(ostream& s,
	   const MvnNode* node,
	   const char* opr_str);

  string
  node_name(const MvnNode* node);

  void
  set_node_name(const MvnNode* node,
		const string& name);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ノードのID をキーにして名前を格納する配列
  vector<string> mNameArray;

};

END_NAMESPACE_YM_NETWORKS_MVN

#endif // VERILOGWRITERIMPL_H
