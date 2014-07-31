
/// @file CmnDumper.cc
/// @brief CmnDumper 実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmNetworks/CmnDumper.h"
#include "YmNetworks/CmnMgr.h"
#include "YmNetworks/CmnPort.h"
#include "YmNetworks/CmnDff.h"
#include "YmNetworks/CmnLatch.h"
#include "YmNetworks/CmnNode.h"
#include "YmNetworks/CmnDffCell.h"
#include "YmNetworks/CmnLatchCell.h"
#include "YmCell/Cell.h"
#include "YmCell/CellPin.h"


BEGIN_NAMESPACE_YM_NETWORKS_CMN

// @brief コンストラクタ
CmnDumper::CmnDumper()
{
}

// @brief デストラクタ
CmnDumper::~CmnDumper()
{
}

void
CmnDumper::operator()(ostream& s,
		      const CmnMgr& network)
{
  ymuint np = network.port_num();
  for (ymuint i = 0; i < np; ++ i) {
    const CmnPort* port = network.port(i);
    s << "PORT#" << i << "(" << port->name() << "): ";

    ymuint nb = port->bit_width();
    assert_cond( nb > 0, __FILE__, __LINE__);
    if ( nb == 1 ) {
      const CmnNode* input = port->input(0);
      const CmnNode* output = port->output(0);
      if ( input ) {
	s << " INPUT(" << input->id_str() << ")";
      }
      if ( output ) {
	s << " OUTPUT(" << output->id_str() << ") = "
	  << output->fanin(0)->id_str();
      }
    }
    else {
      s << "{" << endl;
      for (ymuint j = 0; j < nb; ++ j) {
	ymuint idx = nb - j - 1;
	const CmnNode* input = port->input(idx);
	const CmnNode* output = port->output(idx);
	s << "    ";
	if ( input ) {
	  s << " INPUT(" << input->id_str() << ")";
	}
	if ( output ) {
	  s << " OUTPUT(" << output->id_str() << ") = "
	    << output->fanin(0)->id_str();
	}
	s << endl;
      }
      s << "}";
    }
    s << endl;
  }

  const CmnDffList& dff_list = network.dff_list();
  for (CmnDffList::const_iterator p = dff_list.begin();
       p != dff_list.end(); ++ p) {
    const CmnDff* dff = *p;
    s << "DFF#" << dff->id() << "(" << dff->name() << ")" << endl
      << "  Cell:   " << dff->cell()->cell()->name() << endl
      << "  Q:      " << dff->output1()->id_str() << endl
      << "  IQ:     " << dff->output2()->id_str() << endl
      << "  DATA(" << dff->input()->id_str() << ") = "
      << dff->input()->fanin(0)->id_str() << endl
      << "  CLOCK(" << dff->clock()->id_str() << ") = "
      << dff->clock()->fanin(0)->id_str() << endl;

    const CmnNode* clear = dff->clear();
    if ( clear ) {
      const CmnNode* fanin = clear->fanin(0);
      s << "  CLEAR(" << clear->id_str() << ") = "
	<< fanin->id_str() << endl;
    }

    const CmnNode* preset = dff->preset();
    if ( preset ) {
      const CmnNode* fanin = preset->fanin(0);
      s << "  PRESET(" << preset->id_str() << ") = "
	<< fanin->id_str() << endl;
    }
    s << endl;
  }

  const CmnLatchList& latch_list = network.latch_list();
  for (CmnLatchList::const_iterator p = latch_list.begin();
       p != latch_list.end(); ++ p) {
    const CmnLatch* latch = *p;
    s << "LATCH#" << latch->id() << "(" << latch->name() << ")" << endl
      << "  Cell:   " << latch->cell()->cell()->name() << endl
      << "  Q:      " << latch->output1()->id_str() << endl
      << "  IQ:     " << latch->output2()->id_str() << endl
      << "  DATA:   " << latch->input()->id_str() << endl;
    if ( latch->enable() ) {
      s << "  ENABLE: " << latch->enable()->id_str() << endl;
    }
    if ( latch->clear() ) {
      s << "  CLEAR: " << latch->clear()->id_str() << endl;
    }
    if ( latch->preset() ) {
      s << "  PRESET: " << latch->preset()->id_str() << endl;
    }
    s << endl;
  }

  const CmnNodeList& logic_list = network.logic_list();
  for (CmnNodeList::const_iterator p = logic_list.begin();
       p != logic_list.end(); ++ p) {
    const CmnNode* node = *p;
    const Cell* cell = node->cell();
    s << "CELL(" << node->id_str() << ") = "
      << cell->name() << "(";
    const char* comma = "";
    ymuint ni = node->fanin_num();
    for (ymuint i = 0; i < ni; ++ i) {
      const CmnNode* inode = node->fanin(i);
      s << comma << inode->id_str();
      comma = ", ";
    }
    s << ")" << endl;
  }
}

END_NAMESPACE_YM_NETWORKS_CMN
