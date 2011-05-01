
/// @file libym_cell/mislib/MislibParser.cc
/// @brief MislibParser の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: MislibParser.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "MislibParser.h"


#include "MislibPtImpl1.h"
#include "MislibPtImpl2.h"


BEGIN_NAMESPACE_YM_CELL_MISLIB

#include "mislib_grammer.h"


//////////////////////////////////////////////////////////////////////
// MislibParser
//////////////////////////////////////////////////////////////////////

// コンストラクタ
MislibParser::MislibParser() :
  mAlloc(4096),
  mLex(mMsgMgr)
{
}

// デストラクタ
MislibParser::~MislibParser()
{
  clear();
}

// @brief mislib ファイルを読み込んでライブラリを生成する．
// @param[in] filename ファイル名
// @return パース木の根のノードを返す．
// @note 読み込みが失敗したら NULL を返す．
const MislibPt*
MislibParser::read(const string& filename)
{
  int yyparse(MislibParser& parser);

  if ( !mLex.open_file(filename) ) {
    // エラー
    ostringstream buf;
    buf << filename << " : No such file.";
    mMsgMgr.put_msg(__FILE__, __LINE__, FileRegion(),
		    kMsgFailure, "MISLIB_PARSER", buf.str());
    return false;
  }

  // 初期化
  clear();
  mGateList = new_list();
  mError = false;

  // パース木を作る．
  // 結果は mGateList に入っている．
  yyparse(*this);

  if ( mError ) {
    // 異常終了
    return NULL;
  }

  // 読み込みまではうまくいった．
  return mGateList;
}

// 文字列ノードを生成する．
MislibPt*
MislibParser::new_str(const FileRegion& loc,
		      ShString str)
{
  void* p = mAlloc.get_memory(sizeof(MislibPtStr));
  return new (p) MislibPtStr(loc, str);
}

// 数値ノードを生成する．
MislibPt*
MislibParser::new_num(const FileRegion& loc,
		      double num)
{
  void* p = mAlloc.get_memory(sizeof(MislibPtNum));
  return new (p) MislibPtNum(loc, num);
}

// NONINV ノードを生成する．
MislibPt*
MislibParser::new_noninv(const FileRegion& loc)
{
  void* p = mAlloc.get_memory(sizeof(MislibPtNoninv));
  return new (p) MislibPtNoninv(loc);
}

// INV ノードを生成する．
MislibPt*
MislibParser::new_inv(const FileRegion& loc)
{
  void* p = mAlloc.get_memory(sizeof(MislibPtInv));
  return new (p) MislibPtInv(loc);
}

// UNKNOWN ノードを生成する．
MislibPt*
MislibParser::new_unknown(const FileRegion& loc)
{
  void* p = mAlloc.get_memory(sizeof(MislibPtUnknown));
  return new (p) MislibPtUnknown(loc);
}

// 定数0ノードを生成する．
MislibPt*
MislibParser::new_const0(const FileRegion& loc)
{
  void* p = mAlloc.get_memory(sizeof(MislibPtConst0));
  return new (p) MislibPtConst0(loc);
}

// 定数1ノードを生成する．
MislibPt*
MislibParser::new_const1(const FileRegion& loc)
{
  void* p = mAlloc.get_memory(sizeof(MislibPtConst1));
  return new (p) MislibPtConst1(loc);
}

// NOT ノードを生成する．
MislibPt*
MislibParser::new_not(const FileRegion& loc,
		      MislibPt* child1)
{
  void* p = mAlloc.get_memory(sizeof(MislibPtNot));
  return new (p) MislibPtNot(loc, child1);
}

// AND ノードを生成する．
MislibPt*
MislibParser::new_and(const FileRegion& loc,
		      MislibPt* child1,
		      MislibPt* child2)
{
  void* p = mAlloc.get_memory(sizeof(MislibPtAnd));
  return new (p) MislibPtAnd(loc, child1, child2);
}

// OR ノードを生成する．
MislibPt*
MislibParser::new_or(const FileRegion& loc,
		     MislibPt* child1,
		     MislibPt* child2)
{
  void* p = mAlloc.get_memory(sizeof(MislibPtOr));
  return new (p) MislibPtOr(loc, child1, child2);
}

// XOR ノードを生成する．
MislibPt*
MislibParser::new_xor(const FileRegion& loc,
		      MislibPt* child1,
		      MislibPt* child2)
{
  void* p = mAlloc.get_memory(sizeof(MislibPtXor));
  return new (p) MislibPtXor(loc, child1, child2);
}

// PIN ノードを生成する．
MislibPt*
MislibParser::new_pin(const FileRegion& loc,
		      MislibPt* name,
		      MislibPt* phase,
		      MislibPt* input_load,
		      MislibPt* max_load,
		      MislibPt* rise_block_delay,
		      MislibPt* rise_fanout_delay,
		      MislibPt* fall_block_delay,
		      MislibPt* fall_fanout_delay)
{
  void* p = mAlloc.get_memory(sizeof(MislibPtPin));
  return new (p) MislibPtPin(loc,
			     name, phase,
			     input_load, max_load,
			     rise_block_delay, rise_fanout_delay,
			     fall_block_delay, fall_fanout_delay);
}

// リストノードを生成する．
MislibPt*
MislibParser::new_list()
{
  void* p = mAlloc.get_memory(sizeof(MislibPtList));
  return new (p) MislibPtList();
}

// GATE ノードを生成する．
void
MislibParser::new_gate1(const FileRegion& loc,
			MislibPt* pt_name,
			MislibPt* pt_area,
			MislibPt* pt_oname,
			MislibPt* pt_expr,
			MislibPt* pt_ipin_list)
{
  assert_cond(pt_name, __FILE__, __LINE__);
  assert_cond(pt_name->type() == kStr, __FILE__, __LINE__);
  assert_cond(pt_area, __FILE__, __LINE__);
  assert_cond(pt_area->type() == kNum, __FILE__, __LINE__);
  assert_cond(pt_oname, __FILE__, __LINE__);
  assert_cond(pt_oname->type() == kStr, __FILE__, __LINE__);
  assert_cond(pt_expr, __FILE__, __LINE__);
  assert_cond(pt_expr->is_expr(), __FILE__, __LINE__);
  assert_cond(pt_ipin_list, __FILE__, __LINE__);
  assert_cond(pt_ipin_list->type() == kList, __FILE__, __LINE__);

  void* p = mAlloc.get_memory(sizeof(MislibPtGate));
  MislibPt* gate = new (p) MislibPtGate(loc, pt_name, pt_area,
					pt_oname, pt_expr, pt_ipin_list);

  mGateList->push_back(gate);
}

// GATE ノードを生成する．
void
MislibParser::new_gate2(const FileRegion& loc,
			MislibPt* pt_name,
			MislibPt* pt_area,
			MislibPt* pt_oname,
			MislibPt* pt_expr,
			MislibPt* pt_ipin)
{
  assert_cond(pt_name, __FILE__, __LINE__);
  assert_cond(pt_name->type() == kStr, __FILE__, __LINE__);
  assert_cond(pt_area, __FILE__, __LINE__);
  assert_cond(pt_area->type() == kNum, __FILE__, __LINE__);
  assert_cond(pt_oname, __FILE__, __LINE__);
  assert_cond(pt_oname->type() == kStr, __FILE__, __LINE__);
  assert_cond(pt_expr, __FILE__, __LINE__);
  assert_cond(pt_expr->is_expr(), __FILE__, __LINE__);
  assert_cond(pt_ipin, __FILE__, __LINE__);
  assert_cond(pt_ipin->type() == kPin, __FILE__, __LINE__);
  assert_cond(pt_ipin->name() == NULL, __FILE__, __LINE__);

  void* p = mAlloc.get_memory(sizeof(MislibPtGate));
  MislibPt* gate = new (p) MislibPtGate(loc, pt_name, pt_area,
					pt_oname, pt_expr, pt_ipin);
  mGateList->push_back(gate);
}

// GATE ノードを生成する．
void
MislibParser::new_gate3(const FileRegion& loc,
			MislibPt* pt_name,
			MislibPt* pt_area,
			MislibPt* pt_oname,
			MislibPt* pt_expr)
{
  assert_cond(pt_name, __FILE__, __LINE__);
  assert_cond(pt_name->type() == kStr, __FILE__, __LINE__);
  assert_cond(pt_area, __FILE__, __LINE__);
  assert_cond(pt_area->type() == kNum, __FILE__, __LINE__);
  assert_cond(pt_oname, __FILE__, __LINE__);
  assert_cond(pt_oname->type() == kStr, __FILE__, __LINE__);
  assert_cond(pt_expr, __FILE__, __LINE__);
  assert_cond(pt_expr->is_expr(), __FILE__, __LINE__);

  void* p = mAlloc.get_memory(sizeof(MislibPtGate));
  MislibPt* gate = new (p) MislibPtGate(loc, pt_name, pt_area,
					pt_oname, pt_expr, new_list());
  mGateList->push_back(gate);
}

// 今までに生成したすべてのオブジェクトを解放する．
void
MislibParser::clear()
{
  mAlloc.destroy();
  mGateList = NULL;
}

// @brief 字句解析を行う．
// @param[out] lval トークンの値を格納する変数
// @param[out] lloc トークンの位置を格納する変数
// @return トークンの型を返す．
int
MislibParser::scan(MislibPt*& lval,
		   FileRegion& lloc)
{
  int tok = mLex.read_token();
  lloc = mLex.cur_loc();

  switch ( tok ) {
  case STR:
    lval = new_str(lloc, ShString(mLex.cur_string()));
    break;

  case NUM:
    lval = new_num(lloc, mLex.cur_num());
    break;

  case NONINV:
    lval = new_noninv(lloc);
    break;

  case INV:
    lval = new_inv(lloc);
    break;

  case UNKNOWN:
    lval = new_unknown(lloc);
    break;

  case CONST0:
    lval = new_const0(lloc);
    break;

  case CONST1:
    lval = new_const1(lloc);
    break;

  }
  return tok;
}

// @brief エラーメッセージを出力する．
// @note 副作用で mError が true にセットされる．
void
MislibParser::error(const FileRegion& loc,
		    const char* msg)
{
  string buff;
  const char* msg2;
  // 好みの問題だけど "parse error" よりは "syntax error" の方が好き．
  if ( !strncmp(msg, "parse error", 11) ) {
    buff ="syntax error";
    buff += (msg + 11);
    msg2 = buff.c_str();
  }
  else {
    msg2 = msg;
  }

  msg_mgr().put_msg(__FILE__, __LINE__, loc,
		    kMsgError, "MISLIB_PARSER", msg2);
  mError = true;
}

END_NAMESPACE_YM_CELL_MISLIB
