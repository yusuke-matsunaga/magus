
/// @file DimacsParser.cc
/// @brief DimacsParser の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmLogic/DimacsParser.h"
#include "YmLogic/DimacsHandler.h"
#include "DimacsParserImpl.h"
#include "DimacsScanner.h"
#include "YmUtils/MsgMgr.h"


BEGIN_NAMESPACE_YM_SAT

//////////////////////////////////////////////////////////////////////
// DimacsHandler
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
DimacsHandler::DimacsHandler()
{
}

// @brief デストラクタ
DimacsHandler::~DimacsHandler()
{
}

// @brief 初期化
bool
DimacsHandler::init()
{
  return true;
}

// @brief p 行の読込み
// @param[in] loc ファイル上の位置情報
// @param[in] nv 変数の数
// @param[in] nc 節の数
// @retval true 処理が成功した．
// @retval false エラーが起こった．
bool
DimacsHandler::read_p(const FileRegion& loc,
		      ymuint nv,
		      ymuint nc)
{
  return true;
}

// @brief clause 行の読込み
// @param[in] loc ファイル上の位置情報
// @param[in] lits リテラルの配列．最後の0は含まない
// @retval true 処理が成功した．
// @retval false エラーが起こった．
bool
DimacsHandler::read_clause(const FileRegion& loc,
			   const vector<int>& lits)
{
  return true;
}

// @brief 終了処理
// @param[in] loc 位置情報
bool
DimacsHandler::end()
{
  return true;
}

// @brief エラー終了時の処理
void
DimacsHandler::error_exit()
{
}


//////////////////////////////////////////////////////////////////////
// DimacsParser
//////////////////////////////////////////////////////////////////////

// コンストラクタ
DimacsParser::DimacsParser() :
  mImpl(new DimacsParserImpl)
{
}

// デストラクタ
DimacsParser::~DimacsParser()
{
  delete mImpl;
}

// @brief 読み込みを行う．
// @param[in] ido 入力データ
// @retval true 読み込みが成功した．
// @retval false 読み込みが失敗した．
bool
DimacsParser::read(IDO& ido)
{
  return mImpl->read(ido);
}

// @brief イベントハンドラの登録
void
DimacsParser::add_handler(DimacsHandler* handler)
{
  mImpl->add_handler(handler);
}


//////////////////////////////////////////////////////////////////////
// クラス DimacsParserImpl
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
DimacsParserImpl::DimacsParserImpl()
{
}

// @brief デストラクタ
DimacsParserImpl::~DimacsParserImpl()
{
}

// @brief 読み込みを行なう．
// @param[in] ido 入力データ
// @retval true 読み込みが成功した．
// @retval false 読み込みが失敗した．
bool
DimacsParserImpl::read(IDO& ido)
{
  // 読込用の内部状態
  enum {
    ST_INIT,
    ST_P1,
    ST_P2,
    ST_P3,
    ST_BODY1,
    ST_BODY2,
    ST_BODY3
  } state = ST_INIT;

  // 宣言された変数の数
  int dec_nv = 0;
  // 宣言された節の数
  int dec_nc = 0;

  // 実際に読み込んだ変数の最大値
  int max_v = 0;
  // 実際に読み込んだ節の数
  int act_nc = 0;

#if 0
  FileIDO ido;
  if ( !ido.open(filename) ) {
    // ファイルが開けなかった．
    ostringstream buf;
    buf << filename << " : No such file.";
    MsgMgr::put_msg(__FILE__, __LINE__,
		    FileRegion(),
		    kMsgFailure,
		    "DIMACS_PARSER",
		    buf.str());
    return false;
  }
#endif

  vector<int> lits;

  DimacsScanner scanner(ido);

  bool stat = true;
  for (list<DimacsHandler*>::iterator p = mHandlerList.begin();
       p != mHandlerList.end(); ++ p) {
    DimacsHandler* handler = *p;
    if ( !handler->init() ) {
      stat = false;
    }
  }
  if ( !stat ) {
    goto error;
  }

  for ( ; ; ) {
    FileRegion loc;
    tToken tk = scanner.read_token(loc);
    if ( tk == kERR ) {
      return false;
    }
    if ( tk == kC ) {
      // コメント行なのでなにもしない．
      continue;
    }

    switch ( state ) {
    case ST_INIT:
      if ( tk == kP ) {
	state = ST_P1;
      }
      break;

    case ST_P1:
      if ( tk != kNUM ) {
	goto p_error;
      }
      dec_nv = scanner.cur_val();
      state = ST_P2;
      break;

    case ST_P2:
      if ( tk != kNUM ) {
	goto p_error;
      }
      dec_nc = scanner.cur_val();
      state = ST_P3;
      break;

    case ST_P3:
      if ( tk != kNL ) {
	goto p_error;
      }
      for (list<DimacsHandler*>::iterator p = mHandlerList.begin();
	   p != mHandlerList.end(); ++ p) {
	DimacsHandler* handler = *p;
	if ( !handler->read_p(loc, dec_nv, dec_nc) ) {
	  stat = false;
	}
      }
      if ( !stat ) {
	goto error;
      }
      state = ST_BODY1;
      break;

    case ST_BODY1:
      if ( tk == kP ) {
#if 0
	put_msg(__FILE__, __LINE__,
		loc,
		kMsgError,
		"ERR01",
		"duplicated 'p' lines");
#endif
	cout << "ERR01: duplicated 'p' lines" << endl;
	return false;
      }
      if ( tk == kEOF ) {
	goto normal_end;
      }
      if ( tk == kNL ) {
	continue;
      }
      if ( tk == kNUM ) {
	int v = scanner.cur_val();
	lits.clear();
	lits.push_back(v);
	if ( v < 0 ) {
	  v = - v;
	}
	if ( max_v < v ) {
	  max_v = v;
	}
	state = ST_BODY2;
	break;
      }
      // それ以外はエラー
      goto n_error;

    case ST_BODY2:
      if ( tk == kZERO ) {
	state = ST_BODY3;
      }
      else if ( tk == kNL ) {
	continue;
      }
      else if ( tk == kNUM ) {
	int v = scanner.cur_val();
	lits.push_back(v);
	if ( v < 0 ) {
	  v = - v;
	}
	if ( max_v < v ) {
	  max_v = v;
	}
      }
      else {
	goto n_error;
      }
      break;

    case ST_BODY3:
      if ( tk != kNL && tk != kEOF ) {
	goto n_error;
      }
      ++ act_nc;
      for (list<DimacsHandler*>::iterator p = mHandlerList.begin();
	   p != mHandlerList.end(); ++ p) {
	DimacsHandler* handler = *p;
	if ( !handler->read_clause(loc, lits) ) {
	  stat = false;
	}
      }
      if ( !stat ) {
	goto error;
      }
      state = ST_BODY1;
      break;
    }
  }

  normal_end:
  if ( dec_nv == 0 ) {
#if 0
    add_msg(__FILE__, __LINE__,
	    loc,
	    kMsgError,
	    "ERR02",
	    "unexpected end-of-file");
#endif
    cout << "ERR02: unexpected end-of-file" << endl;
    goto error;
  }
  if ( dec_nv < max_v ) {
#if 0
    add_msg(__FILE__, __LINE__,
	    loc,
	    kMsgWarning,
	    "WRN01",
	    "actual number of variables is more than the declared");
#endif
  }
  if ( dec_nc > act_nc ) {
#if 0
    add_msg(__FILE__, __LINE__,
	    loc,
	    kMsgWarning,
	    "WRN02",
	    "actual number of clauses is less than the declared");
#endif
  }
  else if ( dec_nc < act_nc ) {
#if 0
    add_msg(__FILE__, __LINE__,
	    loc,
	    kMsgWarning,
	    "WRN03",
	    "actual number of clauses is more than the declared");
#endif
  }

  for (list<DimacsHandler*>::iterator p = mHandlerList.begin();
       p != mHandlerList.end(); ++ p) {
    DimacsHandler* handler = *p;
    if ( !handler->end() ) {
	 stat = false;
    }
  }
  if ( !stat ) {
    goto error;
  }
  return true;

 p_error:
#if 0
  add_msg(__FILE__, __LINE__,
	  loc,
	  kMsgError,
	  "ERR03",
	  "syntax error \"p cnf <num of vars> <num of clauses>\" expected");
#endif
  cout << "ERR03 : "
       << "syntax error \"p cnf <num of vars> <num of clauses>\" expected"
       << endl;

  goto error;

 n_error:
#if 0
  add_msg(__FILE__, __LINE__,
	  loc,
	  kMsgError,
	  "ERR04",
	  "syntax error \"<lit_1> <lit_2> ... <lit_n> 0\" expected");
#endif
  cout << "ERR04 : "
       << "syntax error \"<lit_1> <lit_2> ... <lit_n> 0\" expected"
       << endl;
  goto error;

 error:
  for (list<DimacsHandler*>::iterator p = mHandlerList.begin();
       p != mHandlerList.end(); ++ p) {
    DimacsHandler* handler = *p;
    handler->error_exit();
  }

  return false;
}

// @brief イベントハンドラの登録
void
DimacsParserImpl::add_handler(DimacsHandler* handler)
{
#if 0
  handler->mParser = this;
#endif
  mHandlerList.push_back(handler);
}

END_NAMESPACE_YM_SAT
