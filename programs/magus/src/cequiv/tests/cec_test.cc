
/// @file cec_test.cc
/// @brief check_cec のテストプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "cec_nsdef.h"

#include "YmNetworks/BdnNode.h"
#include "YmNetworks/BNetBdnConv.h"
#include "YmUtils/MsgMgr.h"
#include "YmUtils/MsgHandler.h"
#include "YmNetworks/BdnBlifReader.h"
#include "YmNetworks/BdnIscas89Reader.h"


BEGIN_NAMESPACE_YM_CEC

int
cec_test(int argc,
	 const char** argv)
{
  bool blif = true;

  if ( argc != 3 ) {
    cerr << "Usage: " << argv[0] << " <file1> <file2>" << endl;
    return 255;
  }

  MsgHandler* msg_handler = new StreamMsgHandler(&cerr);
  MsgMgr::reg_handler(msg_handler);

  string filename1 = argv[1];
  BdnMgr bdn_network1;
  if ( blif ) {
    BdnBlifReader bdn_read;
    if ( !bdn_read(filename1, bdn_network1) ) {
      cerr << "Error in reading " << filename1 << endl;
      return 1;
    }
  }
  else {
    BdnIscas89Reader bdn_read;
    if ( !bdn_read(filename1, bdn_network1) ) {
      cerr << "Error in reading " << filename1 << endl;
      return 1;
    }
  }

  string filename2 = argv[2];
  BdnMgr bdn_network2;
  if ( blif ) {
    BdnBlifReader bdn_read;
    if ( !bdn_read(filename2, bdn_network2) ) {
      cerr << "Error in reading " << filename2 << endl;
      return 1;
    }
  }
  else {
    BdnIscas89Reader bdn_read;
    if ( !bdn_read(filename2, bdn_network2) ) {
      cerr << "Error in reading " << filename2 << endl;
      return 1;
    }
  }

  ymuint ni = bdn_network1.input_num();
  if ( bdn_network2.input_num() != ni ) {
    cerr << "The number of inputs is different" << endl;
    return 2;
  }
  ymuint no = bdn_network1.output_num();
  if ( bdn_network2.output_num() != no ) {
    cerr << "The number of outputs is different" << endl;
    return 2;
  }

  vector<pair<ymuint, ymuint> > iassoc;
  {
    const BdnNodeList& inputs1 = bdn_network1.input_list();
    const BdnNodeList& inputs2 = bdn_network2.input_list();
    BdnNodeList::const_iterator p1 = inputs1.begin();
    BdnNodeList::const_iterator p2 = inputs2.begin();
    while ( p1 != inputs1.end() &&
	    p2 != inputs2.end() ) {
      BdnNode* node1 = *p1;
      BdnNode* node2 = *p2;
      iassoc.push_back(make_pair(node1->id(), node2->id()));
      ++ p1;
      ++ p2;
    }
  }

  vector<pair<ymuint, ymuint> > oassoc;
  {
    const BdnNodeList& outputs1 = bdn_network1.output_list();
    const BdnNodeList& outputs2 = bdn_network2.output_list();
    BdnNodeList::const_iterator p1 = outputs1.begin();
    BdnNodeList::const_iterator p2 = outputs2.begin();
    while ( p1 != outputs1.end() &&
	    p2 != outputs2.end() ) {
      BdnNode* node1 = *p1;
      BdnNode* node2 = *p2;
      oassoc.push_back(make_pair(node1->id(), node2->id()));
      ++ p1;
      ++ p2;
    }
  }

  vector<Bool3> comp_stats;
  check_ceq2(bdn_network1, bdn_network2,
	     iassoc, oassoc,
	     2, &cout,
	     "ymsat", "", &cout,
	     32,
	     comp_stats);

  return 0;
}

END_NAMESPACE_YM_CEC


int
main(int argc,
     const char** argv)
{
  return nsYm::nsCec::cec_test(argc, argv);
}
