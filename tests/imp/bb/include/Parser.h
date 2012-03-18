#ifndef BB_INCLUDE_PARSER_H
#define BB_INCLUDE_PARSER_H

/// @file bb/include/Parser.h
/// @brief Parser のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "bb_nsdef.h"
#include "ym_utils/MsgMgr.h"


BEGIN_NAMESPACE_YM_BB

class PtMgr;
class Lexer;
class PtNode;

union YYSTYPE;

//////////////////////////////////////////////////////////////////////
/// @class Parser Parser.h "Parser.h"
/// @brief BB 用のパーサーマネージャ
//////////////////////////////////////////////////////////////////////
class Parser
{
public:

  /// @brief コンストラクタ
  /// @param[in] msg_mgr メッセージマネージャ
  /// @param[in] ptmgr 呼んだ結果のパース木を登録するマネージャ
  Parser(MsgMgr& msg_mgr,
	 PtMgr& ptmgr);

  /// @brief デストラクタ
  ~Parser();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部とのインターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイルを読み込む．
  /// @param[in] file_name ファイル名
  /// @return 読み込みに成功したら true を返す．
  bool
  read(const char* file_name);

  /// @brief ビット幅を得る．
  ymuint
  bit_width() const;


public:
  //////////////////////////////////////////////////////////////////////
  // yacc/bison とのインターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ビット幅を設定する．
  void
  set_bit_width(ymuint bw);

  /// @brief 識別子ノードを作る．
  PtNode*
  new_id(const FileRegion& file_region,
	 ymuint id);

  /// @brief 定数ノードを作る．
  PtNode*
  new_const(const FileRegion& file_region,
	    ymint32 value);

  /// @brief neg ノードを作る．
  PtNode*
  new_neg(const FileRegion& file_region,
	  PtNode* opr1);

  /// @brief uminus ノードを作る．
  PtNode*
  new_uminus(const FileRegion& file_region,
	     PtNode* opr1);

  /// @brief add ノードを作る．
  PtNode*
  new_add(const FileRegion& file_region,
	  PtNode* opr1,
	  PtNode* opr2);

  /// @brief sub ノードを作る．
  PtNode*
  new_sub(const FileRegion& file_region,
	  PtNode* opr1,
	  PtNode* opr2);

  /// @brief mul ノードを作る．
  PtNode*
  new_mul(const FileRegion& file_region,
	  PtNode* opr1,
	  PtNode* opr2);

  /// @brief div ノードを作る．
  PtNode*
  new_div(const FileRegion& file_region,
	  PtNode* opr1,
	  PtNode* opr2);

  /// @brief mod ノードを作る．
  PtNode*
  new_mod(const FileRegion& file_region,
	  PtNode* opr1,
	  PtNode* opr2);

  /// @brief and ノード作る．
  PtNode*
  new_and(const FileRegion& file_region,
	  PtNode* opr1,
	  PtNode* opr2);

  /// @brief or ノードを作る．
  PtNode*
  new_or(const FileRegion& file_region,
	 PtNode* opr1,
	 PtNode* opr2);

  /// @brief xor ノードを作る．
  PtNode*
  new_xor(const FileRegion& file_region,
	  PtNode* opr1,
	  PtNode* opr2);

  /// @brief sll ノードを作る．
  PtNode*
  new_sll(const FileRegion& file_region,
	  PtNode* opr1,
	  PtNode* opr2);

  /// @brief srl ノードを作る．
  PtNode*
  new_srl(const FileRegion& file_region,
	  PtNode* opr1,
	  PtNode* opr2);

  /// @brief 新しい代入文を追加する．
  void
  new_assign(const FileRegion& file_region,
	     ymuint lhs_id,
	     PtNode* rhs);

  /// @brief 新しい等式を追加する．
  void
  new_eq(const FileRegion& file_region,
	 PtNode* lhs,
	 PtNode* rhs);

  /// @brief 新しい不等式を追加する．
  void
  new_lt(const FileRegion& file_region,
	 PtNode* lhs,
	 PtNode* rhs);

  /// @brief 新しい不等式を追加する．
  void
  new_le(const FileRegion& file_region,
	 PtNode* lhs,
	 PtNode* rhs);

  /// @brief 新しい不等式を追加する．
  void
  new_gt(const FileRegion& file_region,
	 PtNode* lhs,
	 PtNode* rhs);

  /// @brief 新しい不等式を追加する．
  void
  new_ge(const FileRegion& file_region,
	 PtNode* lhs,
	 PtNode* rhs);

  /// @brief 新しい変数を追加する．
  void
  new_var(const FileRegion& file_region,
	  ymuint id);

  /// @brief 新しい変数を追加する．
  void
  new_var(const FileRegion& file_region,
	  ymuint id,
	  int start,
	  int end);

  /// @brief 新しい変数を追加する．
  void
  new_var(const FileRegion& file_region,
	  ymuint id,
	  int start,
	  int end,
	  int delta);


public:
  //////////////////////////////////////////////////////////////////////
  // yylex とのインターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 字句解析を行う．
  /// @param[out] lvalp 値を格納する変数
  /// @param[out] llocp 位置情報を格納する変数
  /// @return 読み込んだトークンの id を返す．
  int
  yylex(YYSTYPE& lval,
	FileRegion& lloc);


public:
  //////////////////////////////////////////////////////////////////////
  // メッセージ出力用の便利関数
  //////////////////////////////////////////////////////////////////////

  /// @brief メッセージを出力する．
  /// @param[in] src_file この関数を読んでいるソースファイル名
  /// @param[in] src_line この関数を読んでいるソースの行番号
  /// @param[in] file_loc ファイル位置
  /// @param[in] type メッセージの種類
  /// @param[in] label メッセージラベル
  /// @param[in] body メッセージ本文
  void
  put_msg(const char* src_file,
	  int src_line,
	  const FileRegion& file_loc,
	  tMsgType type,
	  const char* label,
	  const char* msg);

  /// @brief メッセージを出力する．
  /// @param[in] src_file この関数を読んでいるソースファイル名
  /// @param[in] src_line この関数を読んでいるソースの行番号
  /// @param[in] file_loc ファイル位置
  /// @param[in] type メッセージの種類
  /// @param[in] label メッセージラベル
  /// @param[in] body メッセージ本文
  void
  put_msg(const char* src_file,
	  int src_line,
	  const FileRegion& file_loc,
	  tMsgType type,
	  const char* label,
	  const string& msg);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // Lexer
  Lexer* mLexer;

  // メッセージマネージャ
  MsgMgr& mMsgMgr;

  // パース木を保持するクラス
  PtMgr& mPtMgr;

  // ビット幅
  ymuint32 mBitWidth;

};

END_NAMESPACE_YM_BB

#endif // BB_INCLUDE_PARSER_H
