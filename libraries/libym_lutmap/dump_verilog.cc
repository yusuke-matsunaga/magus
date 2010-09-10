
/// @file libym_lutmap/dump_verilog.cc
/// @brief dump_verilog() の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: LutNetwork.cc 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_lutmap/LnGraph.h"


BEGIN_NAMESPACE_YM_LUTMAP

// 内容を Verilog-HDL 形式で s に出力する．
void
dump_verilog(ostream& s,
	     const LnGraph& lngraph)
{
  s << "module " << lngraph.name() << "(";
  ymuint np = lngraph.port_num();
  const char* sep = "";
  for (ymuint i = 0; i < np; ++ i) {
    const LnPort* port = lngraph.port(i);
    s << sep << "." << port->name()
      << "(";
    ymuint nb = port->bit_width();
    assert_cond( nb > 0 , __FILE__, __LINE__);
    if ( nb == 1 ) {
      const LnNode* node = port->bit(0);
      s << "n" << node->id_str();
    }
    else {
      s << "{";
      const char* comma = "";
      for (ymuint j = 0; j < nb; ++ j) {
	const LnNode* node = port->bit(j);
	s << comma << "n" << node->id_str();
	comma = ", ";
      }
      s << "}";
    }
    s << ")";
    sep = ", ";
  }
  s << ");" << endl;

  const LnNodeList& input_list = lngraph.input_list();
  for (LnNodeList::const_iterator p = input_list.begin();
       p != input_list.end(); ++ p) {
    const LnNode* node = *p;
    s << "  input  n" << node->id_str() << ";" << endl;
  }
  const LnNodeList& output_list = lngraph.output_list();
  for (LnNodeList::const_iterator p = output_list.begin();
       p != output_list.end(); ++ p) {
    const LnNode* node = *p;
    s << "  output n" << node->id_str() << ";" << endl;
  }
  s << endl;

  const LnNodeList& lut_list = lngraph.lut_list();
  for (LnNodeList::const_iterator p = lut_list.begin();
       p != lut_list.end(); ++ p) {
    const LnNode* node = *p;
    s << "  wire   n" << node->id_str() << ";" << endl;
  }
  s << endl;

  for (LnNodeList::const_iterator p = output_list.begin();
       p != output_list.end(); ++ p) {
    const LnNode* node = *p;
    const LnEdge* e = node->fanin_edge(0);
    const LnNode* inode = e->from();
    assert_cond( inode != NULL, __FILE__, __LINE__);
    s << "  assign n" << node->id_str() << " = n"
      << inode->id_str() << ";" << endl;
  }

#if 0
  const LnNodeList& dff_list = lngraph.dff_list();
  for (LnNodeList::const_iterator p = dff_list.begin();
       p != dff_list.end(); ++ p) {
    const LnNode* node = *p;
    const LnNode* inode = node->fanin(0);
    const LnNode* cnode = node->fanin(1);
    s << "DFF(" << node->id_str() << "):"
      << " DATA = " << inode->id_str()
      << " , CLOCK = " << cnode->id_str();
    const LnNode* snode = node->fanin(2);
    if ( snode ) {
      s << ", SET = " << snode->id_str();
    }
    const LnNode* rnode = node->fanin(3);
    if ( rnode ) {
      s << ", RST = " << rnode->id_str();
    }
    s << endl;
  }
#endif

  const LnNodeList& lut_list = lngraph.lut_list();
  for (LnNodeList::const_iterator p = lut_list.begin();
       p != lut_list.end(); ++ p) {
    const LnNode* node = *p;
    s << "LUT(" << node->id_str() << ")  = (";
    const char* comma = "";
    ymuint ni = node->ni();
    for (ymuint i = 0; i < ni; ++ i) {
      const LnNode* inode = node->fanin(i);
      s << comma << inode->id_str();
      comma = ", ";
    }
    s << ")";
    s << endl;
    ymuint np = 1U << ni;
    vector<int> tv;
    node->tv(tv);
    s << "\t";
    for (ymuint i = 0; i < np; ++ i) {
      s << tv[i];
    }
    s << endl;
  }
}

END_NAMESPACE_YM_LUTMAP
