
/// @file BNetVerilogWriter.cc
/// @brief BNetVerilogWriter の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmNetworks/BNetVerilogWriter.h"
#include "YmNetworks/BNetwork.h"
#include "YmLogic/ExprWriter.h"

#include "VlwModule.h"
#include "VlwModuleHeader.h"
#include "VlwIO.h"
#include "VlwAssign.h"


BEGIN_NAMESPACE_YM_NETWORKS_BNET

BEGIN_NONAMESPACE

// Verilog-HDL で許された識別子名に変換する
string
verilog_name(const string& name)
{
  string::const_iterator p = name.begin();
  char c = *p ++;
  if ( !isalpha(c) && c != '_' ) {
    goto need_escape;
  }
  for ( ; p != name.end(); ++ p) {
    char c = *p;
    if ( !isalnum(c) && c != '_' ) {
      goto need_escape;
    }
  }
  return name;

 need_escape:
  // 先頭が '\' ならば次の空白までを識別子と見なす(らしい)
  string ans = "\\" + name + " ";
  return ans;
}

END_NONAMESPACE


// @brief コンストラクタ
BNetVerilogWriter::BNetVerilogWriter()
{
}

// @brief デストラクタ
BNetVerilogWriter::~BNetVerilogWriter()
{
}

// ブーリアンネットワークをVerilog形式で表示
void
BNetVerilogWriter::dump(ostream& s,
			const BNetwork& network) const
{
  VlWriter writer(s);

  // module 文の処理
  VlwModule vlw_module(writer, network.model_name());

  {
    VlwModuleHeader vlw_module_header(writer);

    const char* comma = "";
    for (BNodeList::const_iterator p = network.inputs_begin();
	 p != network.inputs_end(); ++p) {
      BNode* node = *p;
      s << comma;
      comma = ", ";
      s << verilog_name(node->name());
    }
    for (BNodeList::const_iterator p = network.outputs_begin();
	 p != network.outputs_end(); ++p) {
      BNode* node = *p;
      BNode* inode = node->fanin(0);
      s << comma;
      comma = ", ";
      if ( strcmp(inode->name(), node->name()) == 0 ) {
	s << verilog_name(node->name());
      }
      else {
	// 複数のファンアウトを持ち，その中に外部出力が含まれる場合には
	// ポート結合が複雑になる．
	// inode が外部入力でも同様
	s << "." << verilog_name(node->name()) << "("
	  << verilog_name(inode->name()) << ")";
      }
    }
  }

  // input文の処理
  for (BNodeList::const_iterator p = network.inputs_begin();
       p != network.inputs_end(); ++p) {
    VlwInput vlw_input(writer);

    BNode* node = *p;
    writer.put_elem(node->name());
  }

  // output文の処理
  for (BNodeList::const_iterator p = network.outputs_begin();
       p != network.outputs_end(); ++p) {
    VlwOutput vlw_output(writer);

    BNode* node = *p;
    writer.put_elem(node->name());
  }

  // wire文の処理
  for (BNodeList::const_iterator p = network.logic_nodes_begin();
       p != network.logic_nodes_end(); ++p) {
    VlwWire vlw_wire(writer);

    BNode* node = *p;
    writer.put_elem(node->name());
  }

  // 各内部節点の処理
  HashMap<VarId, string> fanin_name;
  for (BNodeList::const_iterator p = network.logic_nodes_begin();
       p != network.logic_nodes_end(); ++p) {
    BNode* node = *p;
    VlwAssign vlw_assign(writer);

    vlw_assign.put_lhs(node->name());

    fanin_name.clear();
    ymuint n = node->fanin_num();
    for (ymuint i = 0; i < n; i ++) {
      BNode* inode = node->fanin(i);
      assert_cond( inode != 0, __FILE__, __LINE__);
      fanin_name.add(VarId(i), inode->name());
    }
    const Expr& f = node->func();
    vlw_assign.put_rhs(f, fanin_name);
  }
}

END_NAMESPACE_YM_NETWORKS_BNET
