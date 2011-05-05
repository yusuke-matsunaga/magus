
/// @file libym_mislib/MislibParserImpl.cc
/// @brief MislibParserImpl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: MislibParser.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "MislibParserImpl.h"
#include "MislibMgrImpl.h"
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
MislibParserImpl::MislibParserImpl()
{
}

// デストラクタ
MislibParserImpl::~MislibParserImpl()
{
}

// @brief mislib ファイルを読み込んでライブラリを生成する．
// @param[in] filename ファイル名
// @param[in] mgr MislibNode を管理するオブジェクト
// @return パース木の根のノードを返す．
// @note 読み込みが失敗したら false を返す．
bool
MislibParserImpl::read_file(const string& filename,
			    MislibMgrImpl* mgr)
{
  int yyparse(MislibParserImpl& parser,
	      MislibMgrImpl* mgr);

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
  mMislibMgr = mgr;
  mMislibMgr->clear();

  mError = false;

  // パース木を作る．
  // 結果は mGateList に入っている．
  yyparse(*this, mgr);

  if ( mError ) {
    // 異常終了
    return false;
  }

  // 読み込みまではうまくいった．
  return true;
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
    lval = mMislibMgr->new_str(lloc, ShString(mLex.cur_string()));
    break;

  case NUM:
    lval = mMislibMgr->new_num(lloc, mLex.cur_num());
    break;

  case NONINV:
    lval = mMislibMgr->new_noninv(lloc);
    break;

  case INV:
    lval = mMislibMgr->new_inv(lloc);
    break;

  case UNKNOWN:
    lval = mMislibMgr->new_unknown(lloc);
    break;

  case CONST0:
    lval = mMislibMgr->new_const0(lloc);
    break;

  case CONST1:
    lval = mMislibMgr->new_const1(lloc);
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
