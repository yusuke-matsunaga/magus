
/// @file libym_smtlibv2/parser/SmtLibParser.cc
/// @brief SmtLibParser の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: SmtLibParser.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "SmtLibParser.h"
#include "ym_utils/FileDescMgr.h"
#include "ym_lexp/LogExpr.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

#include "smtlib_grammer.h"

//////////////////////////////////////////////////////////////////////
// SmtLibParser
//////////////////////////////////////////////////////////////////////

// コンストラクタ
SmtLibParser::SmtLibParser() :
  mAlloc(4096),
  mLex(mMsgMgr)
{
}

// デストラクタ
SmtLibParser::~SmtLibParser()
{
  clear();
}

// @brief smtlib ファイルを読み込んでライブラリを生成する．
// @param[in] filename ファイル名
// @return 生成したライブラリを返す．
// @note 読み込みが失敗したら NULL を返す．
void
SmtLibParser::read(const string& filename)
{
  int yyparse(SmtLibParser& parser);

  if ( !mLex.open_file(filename) ) {
    // エラー
    ostringstream buf;
    buf << filename << " : No such file.";
    mMsgMgr.put_msg(__FILE__, __LINE__,
		    FileRegion(),
		    kMsgFailure,
		    "SMTLIB_PARSER", buf.str());
    return;
  }

  // 初期化
  clear();
  mError = false;

  // パース木を作る．
  // 結果は mGateList に入っている．
  yyparse(*this);

  if ( mError ) {
    // 異常終了
    return;
  }

  // 読み込みまではうまくいった．
}

// 今までに生成したすべてのオブジェクトを解放する．
void
SmtLibParser::clear()
{
  mAlloc.destroy();
}

// @brief 字句解析を行う．
// @param[out] lval トークンの値を格納する変数
// @param[out] lloc トークンの位置を格納する変数
// @return トークンの型を返す．
int
SmtLibParser::scan(SmtLibPt*& lval,
		   FileRegion& lloc)
{
  int tok = mLex.read_token();
  lloc = mLex.cur_file_region();

#if 0
  switch ( tok ) {
  case STR:
    lval = new_str(lloc, ShString(mLex.cur_string()).id());
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
#endif
  return tok;
}

// @brief エラーメッセージを出力する．
// @note 副作用で mError が true にセットされる．
void
SmtLibParser::error(const FileRegion& loc,
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

END_NAMESPACE_YM_SMTLIBV2
