
/// @file libym_mislib/MislibParserImpl.cc
/// @brief MislibParserImpl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: MislibParser.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "MislibParserImpl.h"
#include "MislibNum.h"
#include "MislibStr.h"
#include "MislibSymbol.h"
#include "MislibConst.h"
#include "MislibList.h"
#include "MislibNot.h"
#include "MislibBop.h"
#include "MislibPin.h"
#include "MislibGate.h"
#include "ym_utils/MsgMgr.h"


BEGIN_NONAMESPACE

// MislibLex::read_token() をデバッグする時に true にする．
bool debug_read_token = false;

END_NONAMESPACE

BEGIN_NAMESPACE_YM_MISLIB

#include "mislib_grammer.h"


//////////////////////////////////////////////////////////////////////
// クラス MislibParserImpl
//////////////////////////////////////////////////////////////////////

// コンストラクタ
MislibParserImpl::MislibParserImpl() :
  mAlloc(4096)
{
}

// デストラクタ
MislibParserImpl::~MislibParserImpl()
{
  clear();
}

// @brief mislib ファイルを読み込んでライブラリを生成する．
// @param[in] filename ファイル名
// @return パース木の根のノードを返す．
// @note 読み込みが失敗したら NULL を返す．
const MislibNode*
MislibParserImpl::read(const string& filename)
{
  int yyparse(MislibParserImpl& parser);

  if ( !mLex.open_file(filename) ) {
    // エラー
    ostringstream buf;
    buf << filename << " : No such file.";
    MsgMgr::put_msg(__FILE__, __LINE__,
		    FileRegion(),
		    kMsgFailure,
		    "MISLIB_PARSER",
		    buf.str());
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
MislibNodeImpl*
MislibParserImpl::new_str(const FileRegion& loc,
			  ShString str)
{
  void* p = mAlloc.get_memory(sizeof(MislibStr));
  return new (p) MislibStr(loc, str);
}

// 数値ノードを生成する．
MislibNodeImpl*
MislibParserImpl::new_num(const FileRegion& loc,
			  double num)
{
  void* p = mAlloc.get_memory(sizeof(MislibNum));
  return new (p) MislibNum(loc, num);
}

// NONINV ノードを生成する．
MislibNodeImpl*
MislibParserImpl::new_noninv(const FileRegion& loc)
{
  void* p = mAlloc.get_memory(sizeof(MislibNoninv));
  return new (p) MislibNoninv(loc);
}

// INV ノードを生成する．
MislibNodeImpl*
MislibParserImpl::new_inv(const FileRegion& loc)
{
  void* p = mAlloc.get_memory(sizeof(MislibInv));
  return new (p) MislibInv(loc);
}

// UNKNOWN ノードを生成する．
MislibNodeImpl*
MislibParserImpl::new_unknown(const FileRegion& loc)
{
  void* p = mAlloc.get_memory(sizeof(MislibUnknown));
  return new (p) MislibUnknown(loc);
}

// 定数0ノードを生成する．
MislibNodeImpl*
MislibParserImpl::new_const0(const FileRegion& loc)
{
  void* p = mAlloc.get_memory(sizeof(MislibConst0));
  return new (p) MislibConst0(loc);
}

// 定数1ノードを生成する．
MislibNodeImpl*
MislibParserImpl::new_const1(const FileRegion& loc)
{
  void* p = mAlloc.get_memory(sizeof(MislibConst1));
  return new (p) MislibConst1(loc);
}

// NOT ノードを生成する．
MislibNodeImpl*
MislibParserImpl::new_not(const FileRegion& loc,
			  const MislibNode* child1)
{
  void* p = mAlloc.get_memory(sizeof(MislibNot));
  return new (p) MislibNot(loc, child1);
}

// AND ノードを生成する．
MislibNodeImpl*
MislibParserImpl::new_and(const FileRegion& loc,
			  const MislibNode* child1,
			  const MislibNode* child2)
{
  void* p = mAlloc.get_memory(sizeof(MislibAnd));
  return new (p) MislibAnd(loc, child1, child2);
}

// OR ノードを生成する．
MislibNodeImpl*
MislibParserImpl::new_or(const FileRegion& loc,
			 const MislibNode* child1,
			 const MislibNode* child2)
{
  void* p = mAlloc.get_memory(sizeof(MislibOr));
  return new (p) MislibOr(loc, child1, child2);
}

// XOR ノードを生成する．
MislibNodeImpl*
MislibParserImpl::new_xor(const FileRegion& loc,
			  const MislibNode* child1,
			  const MislibNode* child2)
{
  void* p = mAlloc.get_memory(sizeof(MislibXor));
  return new (p) MislibXor(loc, child1, child2);
}

// リストノードを生成する．
MislibNodeImpl*
MislibParserImpl::new_list()
{
  void* p = mAlloc.get_memory(sizeof(MislibList));
  return new (p) MislibList();
}

// PIN ノードを生成する．
MislibNodeImpl*
MislibParserImpl::new_pin(const FileRegion& loc,
			  const MislibNode* name,
			  const MislibNode* phase,
			  const MislibNode* input_load,
			  const MislibNode* max_load,
			  const MislibNode* rise_block_delay,
			  const MislibNode* rise_fanout_delay,
			  const MislibNode* fall_block_delay,
			  const MislibNode* fall_fanout_delay)
{
  void* p = mAlloc.get_memory(sizeof(MislibPin));
  return new (p) MislibPin(loc,
			   name, phase,
			   input_load, max_load,
			   rise_block_delay, rise_fanout_delay,
			   fall_block_delay, fall_fanout_delay);
}

// GATE ノードを生成する．
void
MislibParserImpl::new_gate1(const FileRegion& loc,
			    const MislibNode* pt_name,
			    const MislibNode* pt_area,
			    const MislibNode* pt_oname,
			    const MislibNode* pt_expr,
			    const MislibNode* pt_ipin_list)
{
  assert_cond(pt_name, __FILE__, __LINE__);
  assert_cond(pt_name->type() == MislibNode::kStr, __FILE__, __LINE__);
  assert_cond(pt_area, __FILE__, __LINE__);
  assert_cond(pt_area->type() == MislibNode::kNum, __FILE__, __LINE__);
  assert_cond(pt_oname, __FILE__, __LINE__);
  assert_cond(pt_oname->type() == MislibNode::kStr, __FILE__, __LINE__);
  assert_cond(pt_expr, __FILE__, __LINE__);
  assert_cond(pt_expr->is_expr(), __FILE__, __LINE__);
  assert_cond(pt_ipin_list, __FILE__, __LINE__);
  assert_cond(pt_ipin_list->type() == MislibNode::kList, __FILE__, __LINE__);

  void* p = mAlloc.get_memory(sizeof(MislibGate));
  MislibNodeImpl* gate = new (p) MislibGate(loc, pt_name, pt_area,
					    pt_oname, pt_expr, pt_ipin_list);

  mGateList->push_back(gate);
}

// GATE ノードを生成する．
void
MislibParserImpl::new_gate2(const FileRegion& loc,
			    const MislibNode* pt_name,
			    const MislibNode* pt_area,
			    const MislibNode* pt_oname,
			    const MislibNode* pt_expr,
			    const MislibNode* pt_ipin)
{
  assert_cond(pt_name, __FILE__, __LINE__);
  assert_cond(pt_name->type() == MislibNode::kStr, __FILE__, __LINE__);
  assert_cond(pt_area, __FILE__, __LINE__);
  assert_cond(pt_area->type() == MislibNode::kNum, __FILE__, __LINE__);
  assert_cond(pt_oname, __FILE__, __LINE__);
  assert_cond(pt_oname->type() == MislibNode::kStr, __FILE__, __LINE__);
  assert_cond(pt_expr, __FILE__, __LINE__);
  assert_cond(pt_expr->is_expr(), __FILE__, __LINE__);
  assert_cond(pt_ipin, __FILE__, __LINE__);
  assert_cond(pt_ipin->type() == MislibNode::kPin, __FILE__, __LINE__);
  assert_cond(pt_ipin->name() == NULL, __FILE__, __LINE__);

  void* p = mAlloc.get_memory(sizeof(MislibGate));
  MislibNodeImpl* gate = new (p) MislibGate(loc, pt_name, pt_area,
					    pt_oname, pt_expr, pt_ipin);
  mGateList->push_back(gate);
}

// GATE ノードを生成する．
void
MislibParserImpl::new_gate3(const FileRegion& loc,
			    const MislibNode* pt_name,
			    const MislibNode* pt_area,
			    const MislibNode* pt_oname,
			    const MislibNode* pt_expr)
{
  assert_cond(pt_name, __FILE__, __LINE__);
  assert_cond(pt_name->type() == MislibNode::kStr, __FILE__, __LINE__);
  assert_cond(pt_area, __FILE__, __LINE__);
  assert_cond(pt_area->type() == MislibNode::kNum, __FILE__, __LINE__);
  assert_cond(pt_oname, __FILE__, __LINE__);
  assert_cond(pt_oname->type() == MislibNode::kStr, __FILE__, __LINE__);
  assert_cond(pt_expr, __FILE__, __LINE__);
  assert_cond(pt_expr->is_expr(), __FILE__, __LINE__);

  void* p = mAlloc.get_memory(sizeof(MislibGate));
  MislibNodeImpl* gate = new (p) MislibGate(loc, pt_name, pt_area,
					    pt_oname, pt_expr, new_list());
  mGateList->push_back(gate);
}

// 今までに生成したすべてのオブジェクトを解放する．
void
MislibParserImpl::clear()
{
  mAlloc.destroy();
  mGateList = NULL;
}

// @brief 字句解析を行う．
// @param[out] lval トークンの値を格納する変数
// @param[out] lloc トークンの位置を格納する変数
// @return トークンの型を返す．
int
MislibParserImpl::scan(MislibNodeImpl*& lval,
		       FileRegion& lloc)
{
  int tok = mLex.read_token(lloc);

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

  default:
    break;
  }

  if ( debug_read_token ) {
    cout << "MislibParserImpl::scan(): ";
    switch ( tok ) {
    case STR:
      cout << "STR(" << mLex.cur_string() << ")" << endl;
      break;

    case NUM:
      cout << "NUM(" << mLex.cur_num() << ")" << endl;
      break;

    case NONINV:
      cout << "NONINV" << endl;
      break;

    case INV:
      cout << "INV" << endl;
      break;

    case UNKNOWN:
      cout << "UNKNOWN" << endl;
      break;

    case CONST0:
      cout << "CONST0" << endl;
      break;

    case CONST1:
      cout << "CONST1" << endl;
      break;

    case LP:
      cout << "LP" << endl;
      break;

    case RP:
      cout << "RP" << endl;
      break;

    case SEMI:
      cout << "SEMI" << endl;
      break;

    case EQ:
      cout << "EQ" << endl;
      break;

    case GATE:
      cout << "GATE" << endl;
      break;

    case PIN:
      cout << "PIN" << endl;
      break;

    case ERROR:
      cout << "ERROR" << endl;
      break;

    case PLUS:
      cout << "PLUS" << endl;
      break;

    case HAT:
      cout << "HAT" << endl;
      break;

    case STAR:
      cout << "STAR" << endl;
      break;

    case NOT:
      cout << "NOT" << endl;
      break;

    case EOF:
      cout << "EOF" << endl;
      break;

    default:
      cout << tok << endl;
      break;
    }
  }

  return tok;
}

// @brief エラーメッセージを出力する．
// @note 副作用で mError が true にセットされる．
void
MislibParserImpl::error(const FileRegion& loc,
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

  MsgMgr::put_msg(__FILE__, __LINE__,
		  loc,
		  kMsgError,
		  "MISLIB_PARSER",
		  msg2);
  mError = true;
}

END_NAMESPACE_YM_MISLIB
