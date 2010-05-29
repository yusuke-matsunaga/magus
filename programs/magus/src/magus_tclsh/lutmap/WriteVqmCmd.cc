
/// @file magus/lutmap/WriteVqmCmd.cc
/// @brief WriteVqmCmd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: WriteVqmCmd.cc 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "LutmapCmd.h"
#include "SbjGraph.h"
#include "LutNetwork.h"


BEGIN_NAMESPACE_MAGUS_LUTMAP

//////////////////////////////////////////////////////////////////////
// マッピングを行うコマンド
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
WriteVqmCmd::WriteVqmCmd(NetMgr* mgr,
			 LutmapData* data) :
  LutmapCmd(mgr, data)
{
  set_usage_string("?<filename>? <module-name>");
}

// @brief デストラクタ
WriteVqmCmd::~WriteVqmCmd()
{
}

BEGIN_NONAMESPACE

void
write_vqm_cycloneiii(const LnGraph& mapgraph,
		     const string& net_name,
		     ostream& fout)
{
  const LnNodeList& pi_list = mapgraph.input_list();
  const LnNodeList& po_list = mapgraph.output_list();
  const LnNodeList& lut_list = mapgraph.lnode_list();

  fout << "module " << net_name << "(" << endl;
  fout << " clk ," << endl;

  const char* comma = "";
  for ( LnNodeListConstIter i = pi_list.begin(); i != pi_list.end(); ++i ) {
    const LnNode* pi = *i;
    fout << comma << " pi_" << pi->name();
    comma = ", ";
  }
  for ( LnNodeListConstIter i = po_list.begin(); i != po_list.end(); ++i ) {
    fout << " ," << endl;
    const LnNode* po = *i;
    fout << " po_" << po->name();
  }
  fout << endl
       << ");" << endl;
  
  fout << " input clk ;" << endl;
  for ( LnNodeListConstIter i = pi_list.begin(); i != pi_list.end(); ++i ){
    const LnNode* pi = *i;
    fout << " input pi_" << pi->name() << " ;" << endl;
  }
  for ( LnNodeListConstIter i = po_list.begin(); i != po_list.end(); ++i ){
    const LnNode* po = *i;
    fout << " output po_" << po->name() << " ;" << endl;
  }
  for ( LnNodeListConstIter i = pi_list.begin(); i != pi_list.end(); ++i ){
    const LnNode* pi = *i;
    fout << " wire w_" << pi->name() << " ;" << endl;
  }
  for ( LnNodeListConstIter i = po_list.begin(); i != po_list.end(); ++i ){
    const LnNode* po = *i;
    const LnNode* po_fi = po->fanin(0);
    if ( po_fi->name() != po->name() ){
      fout << " wire w_" << po->name() << " ;" << endl;
    }
  }
  for ( LnNodeListConstIter i = lut_list.begin(); i != lut_list.end(); ++i ){
    const LnNode* lut = *i;
    fout << " wire w_" << lut->name() << " ;" << endl;
  }
  fout << " wire w_one ;" << endl;
  fout << " wire w_gnd ;" << endl;
  
  fout << endl << " assign w_one = 1'b1 ;" << endl;
  fout << "assign w_gnd = 1'b0 ;" << endl;
  fout << endl;
  
  for ( LnNodeListConstIter i = pi_list.begin(); i != pi_list.end(); ++i ){
    const LnNode* pi = *i;
    fout << " DFFE FI" << pi->name()
	 << "( .Q(w_" << pi->name() << ")"
	 << ", .CLK(clk), .ENA(w_one)"
	 << ", .D(pi_" << pi->name() << ")"
	 << ", .CLRN(w_one),　.PRN(w_one));" << endl;
  }
  for ( LnNodeListConstIter i = po_list.begin(); i != po_list.end(); ++i ){
    const LnNode* po = *i;
    const LnNode* po_fanin = po->fanin(0);
    fout << " DFFE FO" << po->name()
	 << "( .Q(po_" << po->name() << ")"
	 << ", .CLK(clk), .ENA(w_one)"
	 << ", .D(w_" << po_fanin->name() << ")"
	 << ",　.CLRN(w_one), .PRN(w_one));" << endl;
  }
  string lut_in[4];
  lut_in[0] = "dataa";
  lut_in[1] = "datab";
  lut_in[2] = "datac";
  lut_in[3] = "datad";
  
  for ( LnNodeListConstIter i = lut_list.begin(); i != lut_list.end(); ++i ){
    const LnNode* lut = *i;
    vector<int> bool_table;
    lut->tv( bool_table ); // 真理値表を得る
    ymuint ni = lut->ni();
    ymuint table_size = bool_table.size();
    
    fout << endl;
    
    if ( ni == 0 ){
      fout << "assign w_" << lut->name() << " = "
	   << bool_table[0] << endl << endl;
    }
    else {
      fout << " cycloneiii_lcell_comb lut_" << lut->name() << " (" << endl;
      for ( ymuint j = 0; j != 4; ++j ){
	const LnNode* fanin = lut->fanin(j);
	fout << " ." << lut_in[j] << "(w_";
	if ( j < ni ) {
	  fout << fanin->name() << ")," << endl;
	}
	else {
	  fout << "gnd )," << endl;
	}
      }
      fout << " .combout(w_" << lut->name() << ") );" << endl;
      fout << " defparam lut_" << lut->name()
	   << " .sum_lutc_input =\"datac\";" << endl;
      fout << " defparam lut_" << lut->name()
	   << " .lut_mask = 16'b";
      for ( ymuint j = 16; j != 0; --j ){
	if ( j > table_size ){
	  fout << "0";
	}
	else{
	  fout << bool_table[j-1];
	}
      }
      fout << ";" << endl;
    }
  }
  fout << "endmodule" << endl;
}

END_NONAMESPACE


// @brief コマンドを実行する仮想関数
int
WriteVqmCmd::cmd_proc(TclObjVector& objv)
{
  ymuint objc = objv.size();
  if ( objc != 2 && objc != 3 ) {
    print_usage();
    return TCL_ERROR;
  }

  try {
    ostream* outp = &cout;
    ofstream ofs;
    ymuint base = 1;
    if ( objc == 3 ) {
      base = 2;
      string file_name = objv[1];
      if ( !open_ofile(ofs, file_name) ) {
	// ファイルが開けなかった．
	return TCL_ERROR;
      }
      outp = &ofs;
    }
    string module_name = objv[base];
    write_vqm_cycloneiii(lutnetwork(), module_name, *outp);
    return TCL_OK;
  }
  catch ( AssertError x ) {
    cerr << x << endl;
    TclObj emsg;
    emsg << "Assertion Error";
    set_result(emsg);
    return TCL_ERROR;
  }
  
  return TCL_OK;
}

END_NAMESPACE_MAGUS_LUTMAP
