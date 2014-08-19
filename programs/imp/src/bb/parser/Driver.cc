
/// @file Driver.cc
/// @brief Driver の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "Driver.h"
#include "Lexer.h"
#include "PtMgr.h"


BEGIN_NAMESPACE_YM_BB

#include "bb_grammer.hh"


// @brief コンストラクタ
// @param[in] msg_mgr メッセージマネージャ
// @param[in] ptmgr 読んだ結果のパース木を登録するマネージャ
// @param[in] alloc メモリアロケータ
Driver::Driver(MsgMgr& msg_mgr,
	       PtMgr& ptmgr) :
  mLexer(new Lexer(msg_mgr)),
  mMsgMgr(msg_mgr),
  mPtMgr(ptmgr),
  mBitWidth(0)
{
}

// @brief デストラクタ
Driver::~Driver()
{
  delete mLexer;
}

// @brief ファイルを読み込む．
// @param[in] file_name ファイル名
// @return 読み込みに成功したら true を返す．
bool
Driver::read(const char* file_name)
{
  int yyparse(Driver&);

  mPtMgr.init();

  if ( !mLexer->open_file(file_name) ) {
    return false;
  }

  int stat = yyparse(*this);

  return (stat == 0);
}

// @brief ビット幅を得る．
ymuint
Driver::bit_width() const
{
  return mBitWidth;
}

// @brief ビット幅を設定する．
void
Driver::set_bit_width(ymuint bw)
{
  mBitWidth = bw;
}

// @brief 識別子ノードを作る．
PtNode*
Driver::new_id(const FileRegion& file_region,
	       ymuint id)
{
  return mPtMgr.new_id(file_region, id);
}

// @brief 定数ノードを作る．
PtNode*
Driver::new_const(const FileRegion& file_region,
		  ymint32 value)
{
  return mPtMgr.new_const(file_region, value);
}

// @brief neg ノードを作る．
PtNode*
Driver::new_neg(const FileRegion& file_region,
		PtNode* opr1)
{
  return mPtMgr.new_neg(file_region, opr1);
}

// @brief uminus ノードを作る．
PtNode*
Driver::new_uminus(const FileRegion& file_region,
		   PtNode* opr1)
{
  return mPtMgr.new_uminus(file_region, opr1);
}

// @brief add ノードを作る．
PtNode*
Driver::new_add(const FileRegion& file_region,
		PtNode* opr1,
		PtNode* opr2)
{
  return mPtMgr.new_add(file_region, opr1, opr2);
}

// @brief sub ノードを作る．
PtNode*
Driver::new_sub(const FileRegion& file_region,
		PtNode* opr1,
		PtNode* opr2)
{
  return mPtMgr.new_sub(file_region, opr1, opr2);
}

// @brief mul ノードを作る．
PtNode*
Driver::new_mul(const FileRegion& file_region,
		PtNode* opr1,
		PtNode* opr2)
{
  return mPtMgr.new_mul(file_region, opr1, opr2);
}

// @brief div ノードを作る．
PtNode*
Driver::new_div(const FileRegion& file_region,
		PtNode* opr1,
		PtNode* opr2)
{
  return mPtMgr.new_div(file_region, opr1, opr2);
}

// @brief mod ノードを作る．
PtNode*
Driver::new_mod(const FileRegion& file_region,
		PtNode* opr1,
		PtNode* opr2)
{
  return mPtMgr.new_mod(file_region, opr1, opr2);
}

// @brief and ノード作る．
PtNode*
Driver::new_and(const FileRegion& file_region,
		PtNode* opr1,
		PtNode* opr2)
{
  return mPtMgr.new_and(file_region, opr1, opr2);
}

// @brief or ノードを作る．
PtNode*
Driver::new_or(const FileRegion& file_region,
	       PtNode* opr1,
	       PtNode* opr2)
{
  return mPtMgr.new_or(file_region, opr1, opr2);
}

// @brief xor ノードを作る．
PtNode*
Driver::new_xor(const FileRegion& file_region,
		PtNode* opr1,
		PtNode* opr2)
{
  return mPtMgr.new_xor(file_region, opr1, opr2);
}

// @brief sll ノードを作る．
PtNode*
Driver::new_sll(const FileRegion& file_region,
		PtNode* opr1,
		PtNode* opr2)
{
  return mPtMgr.new_sll(file_region, opr1, opr2);
}

// @brief srl ノードを作る．
PtNode*
Driver::new_srl(const FileRegion& file_region,
		PtNode* opr1,
		PtNode* opr2)
{
  return mPtMgr.new_srl(file_region, opr1, opr2);
}

// @brief 新しい代入文を追加する．
void
Driver::new_assign(const FileRegion& file_region,
		   ymuint lhs_id,
		   PtNode* rhs)
{
  mPtMgr.new_assign(file_region, lhs_id, rhs);
}

// @brief 新しい等式を追加する．
void
Driver::new_eq(const FileRegion& file_region,
	       PtNode* lhs,
	       PtNode* rhs)
{
  mPtMgr.new_eq(file_region, lhs, rhs);
}

// @brief 新しい不等式を追加する．
void
Driver::new_lt(const FileRegion& file_region,
	       PtNode* lhs,
	       PtNode* rhs)
{
  mPtMgr.new_lt(file_region, lhs, rhs);
}

// @brief 新しい不等式を追加する．
void
Driver::new_le(const FileRegion& file_region,
	       PtNode* lhs,
	       PtNode* rhs)
{
  mPtMgr.new_le(file_region, lhs, rhs);
}

// @brief 新しい不等式を追加する．
void
Driver::new_gt(const FileRegion& file_region,
	       PtNode* lhs,
	       PtNode* rhs)
{
  mPtMgr.new_gt(file_region, lhs, rhs);
}

// @brief 新しい不等式を追加する．
void
Driver::new_ge(const FileRegion& file_region,
	       PtNode* lhs,
	       PtNode* rhs)
{
  mPtMgr.new_ge(file_region, lhs, rhs);
}

// @brief 新しい変数を追加する．
void
Driver::new_var(const FileRegion& file_region,
		ymuint id)
{
  mPtMgr.new_var(file_region, id);
}

// @brief 新しい変数を追加する．
void
Driver::new_var(const FileRegion& file_region,
		ymuint id,
		int start,
		int end)
{
  mPtMgr.new_var(file_region, id, start, end);
}

// @brief 新しい変数を追加する．
void
Driver::new_var(const FileRegion& file_region,
		ymuint id,
		int start,
		int end,
		int delta)
{
  mPtMgr.new_var(file_region, id, start, end, delta);
}

// @brief yylex とのインターフェイス
// @param[out] lval 値を格納する変数
// @param[out] lloc 位置情報を格納する変数
// @return 読み込んだトークンの id を返す．
int
Driver::yylex(YYSTYPE& lval,
	      FileRegion& lloc)
{
  int id = mLexer->read_token();

  switch ( id ) {
  case ID:
    if ( strcmp(mLexer->cur_string(), "var") == 0 ) {
      id = VAR;
    }
    else if ( strcmp(mLexer->cur_string(), "bit_width") == 0 ) {
      id = BIT_WIDTH;
    }
    else {
      lval.inttype = mPtMgr.str2id(mLexer->cur_string());
    }
    break;

  case NUMBER:
    lval.inttype = atoi(mLexer->cur_string());
    break;

  default:
    break;
  }
  lloc = mLexer->cur_file_region();
  return id;
}

// @brief メッセージを出力する．
// @param[in] src_file この関数を読んでいるソースファイル名
// @param[in] src_line この関数を読んでいるソースの行番号
// @param[in] file_loc ファイル位置
// @param[in] type メッセージの種類
// @param[in] label メッセージラベル
// @param[in] body メッセージ本文
void
Driver::put_msg(const char* src_file,
		int src_line,
		const FileRegion& file_loc,
		tMsgType type,
		const char* label,
		const char* msg)
{
  mMsgMgr.put_msg(src_file, src_line, file_loc, type, label, msg);
}

// @brief メッセージを出力する．
// @param[in] src_file この関数を読んでいるソースファイル名
// @param[in] src_line この関数を読んでいるソースの行番号
// @param[in] file_loc ファイル位置
// @param[in] type メッセージの種類
// @param[in] label メッセージラベル
// @param[in] body メッセージ本文
void
Driver::put_msg(const char* src_file,
		int src_line,
		const FileRegion& file_loc,
		tMsgType type,
		const char* label,
		const string& msg)
{
  mMsgMgr.put_msg(src_file, src_line, file_loc, type, label, msg);
}

END_NAMESPACE_YM_BB
