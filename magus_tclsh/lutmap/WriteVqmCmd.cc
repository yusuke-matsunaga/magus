
/// @file magus/lutmap/WriteVqmCmd.cc
/// @brief WriteVqmCmd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: WriteVqmCmd.cc 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "LutmapCmd.h"

#include "ym/BnNode.h"
#include "ym/TvFunc.h"


BEGIN_NAMESPACE_MAGUS

//////////////////////////////////////////////////////////////////////
// マッピングを行うコマンド
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
WriteVqmCmd::WriteVqmCmd(MagMgr* mgr,
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
write_vqm_cycloneiii(const BnNetwork& mapgraph,
		     const string& net_name,
		     ostream& fout)
{
  fout << "module " << net_name << "(" << endl;
  fout << " clk ," << endl;

  const char* comma = "";
  for ( auto id: mapgraph.input_id_list() ) {
    fout << comma << " pi_" << id;
    comma = ", ";
  }
  for ( auto id: mapgraph.output_src_id_list() ) {
    fout << " ," << endl;
    fout << " po_" << id;
  }
  fout << endl
       << ");" << endl;

  fout << " input clk ;" << endl;
  for ( auto id: mapgraph.input_id_list() ) {
    fout << " input pi_" << id << " ;" << endl;
  }
  for ( auto id: mapgraph.output_src_id_list() ) {
    fout << " output po_" << id << " ;" << endl;
  }
  for ( auto id: mapgraph.input_id_list() ) {
    fout << " wire w_" << id << " ;" << endl;
  }
#if 0
  for (ymuint i = 0; i < no; ++ i) {
    const BnNode* po = *i;
    const BnNode* po_fi = po->fanin(0);
    if ( po_fi->id_str() != po->id_str() ){
      fout << " wire w_" << po->id_str() << " ;" << endl;
    }
  }
#endif
  for ( auto id: mapgraph.logic_id_list() ) {
    fout << " wire w_" << id << " ;" << endl;
  }
  fout << " wire w_one ;" << endl;
  fout << " wire w_gnd ;" << endl;

  fout << endl << " assign w_one = 1'b1 ;" << endl;
  fout << "assign w_gnd = 1'b0 ;" << endl;
  fout << endl;

  for ( auto id: mapgraph.input_id_list() ) {
    fout << " DFFE FI" << id
	 << "( .Q(w_" << id << ")"
	 << ", .CLK(clk), .ENA(w_one)"
	 << ", .D(pi_" << id << ")"
	 << ", .CLRN(w_one), .PRN(w_one));" << endl;
  }
#if 0
  for (ymuint i = 0; i < no; ++ i) {
    const BnNode* po = mapgraph.output(i);
    const BnNode* po_fanin = po->fanin(0);
    fout << " DFFE FO" << po->id()
	 << "( .Q(po_" << po->id() << ")"
	 << ", .CLK(clk), .ENA(w_one)"
	 << ", .D(w_" << po_fanin->id_str() << ")"
	 << ",　.CLRN(w_one), .PRN(w_one));" << endl;
  }
#endif
  string lut_in[4];
  lut_in[0] = "dataa";
  lut_in[1] = "datab";
  lut_in[2] = "datac";
  lut_in[3] = "datad";

  for ( auto id: mapgraph.logic_id_list() ) {
    auto& lut = mapgraph.node(id);
    ASSERT_COND( lut.type() == BnNodeType::TvFunc );
    const TvFunc& tv = mapgraph.func(lut.func_id());
    int ni = lut.fanin_num();
    int table_size = 1U << ni;

    fout << endl;

    if ( ni == 0 ){
      fout << "assign w_" << id << " = "
	   << tv.value(0) << endl << endl;
    }
    else {
      fout << " cycloneiii_lcell_comb lut_" << lut.id() << " (" << endl;
      for ( int j = 0; j < 4; ++ j ){
	fout << " ." << lut_in[j] << "(w_";
	if ( j < ni ) {
	  fout << lut.fanin_id(j) << ")," << endl;
	}
	else {
	  fout << "gnd )," << endl;
	}
      }
      fout << " .combout(w_" << lut.id() << ") );" << endl;
      fout << " defparam lut_" << lut.id()
	   << " .sum_lutc_input =\"datac\";" << endl;
      fout << " defparam lut_" << lut.id()
	   << " .lut_mask = 16'b";
      for ( ymuint j = 16; j != 0; --j ){
	if ( j > table_size ){
	  fout << "0";
	}
	else{
	  fout << tv.value(j - 1);
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

END_NAMESPACE_MAGUS
