
/// @file lutmap/src/lutmap.cc
/// @brief lutmap のメイン関数
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: bliftest.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_lutmap/lutmap_nsdef.h"
#include "ym_lutmap/SbjGraph.h"
#include "ym_lutmap/LnGraph.h"
#if 0
#include "ym_lutmap/BNet2Sbj.h"
#include "ym_lutmap/Ln2BNet.h"
#include "ym_bnet/BNetwork.h"
#include "ym_bnet/BNetBlifReader.h"
#include "ym_bnet/BNetDecomp.h"
#else
#include "ym_mvn/mvn_nsdef.h"
#include "ym_mvn/MvMgr.h"
#include "ym_mvn/MvVerilogReader.h"
#endif
#include "ym_utils/MsgHandler.h"


BEGIN_NAMESPACE_YM_LUTMAP

void
mvn2sbj(const MvMgr& mvmgr,
	SbjGraph& sbjgraph);

END_NAMESPACE_YM_LUTMAP


int
main(int argc,
     char** argv)
{
  using namespace std;
  using namespace nsYm;
  
  if ( argc < 2 ) {
    cerr << "USAGE : " << argv[0] << " verilog-file" << endl;
    return 2;
  }

  list<string> filename_list;
  for (ymuint i = 1; i < argc; ++ i) {
    filename_list.push_back(argv[i]);
  }
  
  try {
#if 0
    MsgHandler* msg_handler = new StreamMsgHandler(&cerr);
    BNetBlifReader reader;

    reader.add_msg_handler(msg_handler);
    
    BNetwork network;

    if ( !reader.read(filename, network) ) {
      cerr << "Error in reading " << filename << endl;
      return 4;
    }

    // 2入力ノードに分解
    BNetDecomp decomp;
    decomp(network, 2);

    // SbjGraph に変換
    SbjGraph sbj_network;
    BNet2Sbj conv1;
    bool stat = conv1(network, sbj_network, cerr);
    if ( !stat ) {
      return 10;
    }
#else
    MvMgr mgr;
    MvVerilogReader reader;
    MsgHandler* mh = new StreamMsgHandler(&cerr);
    mh->set_mask(MsgHandler::kMaskAll);
    mh->delete_mask(kMsgInfo);
    mh->delete_mask(kMsgDebug);

    reader.set_ffname("KTECH_DFF", // セル名
		      "D",         // データ入力
		      "CK",        // クロック
		      "Q",         // ノーマル出力
		      "QN",        // 反転出力
		      "",          // セット
		      "");         // リセット
    
    // verilog ファイルの読み込み
    for (list<string>::const_iterator p = filename_list.begin();
	 p != filename_list.end(); ++ p) {
      const string& name = *p;
      cerr << "Reading " << name;
      cerr.flush();
      bool stat = reader.read(name);
      cerr << " end" << endl;
      if ( !stat ) {
	return 1;
      }
    }
    // MvNetwork に変換
    bool stat = reader.gen_network(mgr);
    if ( !stat ) {
      cerr << "error occured" << endl;
      return 2;
    }

    // SbjGraph に変換
    SbjGraph sbj_network;
    mvn2sbj(mgr, sbj_network);

#endif
    
    // LUT にマッピング
    LnGraph lut_network;
    ymuint lut_num;
    ymuint depth;
    delay_map(sbj_network,
	      4,            // LUT の入力数
	      0,            // スラック
	      3,            // モード(0から3の値)
	      lut_network,
	      lut_num,
	      depth);

    // とりあえず内容を出力する．
    dump(cout, lut_network);
    
  }
  catch ( AssertError x) {
    cout << x << endl;
  }

  return 0;
}
