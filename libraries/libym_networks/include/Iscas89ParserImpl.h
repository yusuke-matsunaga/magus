#ifndef ISCAS89PARSERIMPL_H
#define ISCAS89PARSERIMPL_H

/// @file Iscas89ParserImpl.h
/// @brief Iscas89ParserImpl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "iscas89_nsdef.h"

#include "YmUtils/StrBuff.h"
#include "Iscas89IdHash.h"


BEGIN_NAMESPACE_YM_ISCAS89

class Iscas89Handler;
class Iscas89Scanner;

//////////////////////////////////////////////////////////////////////
/// @brief トークン
//////////////////////////////////////////////////////////////////////
enum Token {
  kToken_INPUT,
  kToken_OUTPUT,
  kToken_BUFF,
  kToken_NOT,
  kToken_AND,
  kToken_NAND,
  kToken_OR,
  kToken_NOR,
  kToken_XOR,
  kToken_XNOR,
  kToken_DFF,
  kToken_NAME,
  kToken_EOF,
  kToken_ERROR
};


//////////////////////////////////////////////////////////////////////
/// @class Iscas89ParserImpl Iscas89ParserImpl.h "Iscas89ParserImpl.h"
/// @brief Iscas89Parser の実装クラス
//////////////////////////////////////////////////////////////////////
class Iscas89ParserImpl
{
public:

  /// @brief コンストラクタ
  Iscas89ParserImpl();

  /// @brief デストラクタ
  ~Iscas89ParserImpl();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 読み込みを行う．
  /// @param[in] filename ファイル名
  /// @retval true 読み込みが成功した．
  /// @retval false 読み込みが失敗した．
  bool
  read(const string& filename);

  /// @brief イベントハンドラの登録
  void
  add_handler(Iscas89Handler* handler);


public:

  /// @brief yylex() 用の処理を行う．
  /// @param[out] lval トークンの値を格納する変数
  /// @param[out] lloc トークンの位置を格納する変数
  /// @return トークンの型を返す．
  int
  scan(ymuint32& lval,
       FileRegion& lloc);

  /// @brief INPUT 文を読み込む．
  /// @param[in] loc ファイル位置
  /// @param[in] name_id 入力ピン名の ID 番号
  /// @return エラーが起きたら false を返す．
  bool
  read_input(const FileRegion& loc,
	     ymuint32 name_id);

  /// @brief OUTPUT 文を読み込む．
  /// @param[in] loc ファイル位置
  /// @param[in] name_id 出力ピン名の ID 番号
  /// @return エラーが起きたら false を返す．
  bool
  read_output(const FileRegion& loc,
	      ymuint32 name_id);

  /// @brief ゲート文を読み込む．
  /// @param[in] loc ファイル位置
  /// @param[in] oname_id 出力名の ID 番号
  /// @param[in] type ゲートタイプ
  /// @return エラーが起きたら false を返す．
  /// @note 入力名のリストは push_str() で積まれている．
  bool
  read_gate(const FileRegion& loc,
	    ymuint32 oname_id,
	    tIscas89GateType type);

  /// @brief 名前をリストに追加する．
  /// @param[in] str_id 文字列の ID 番号
  void
  push_str(ymuint32 str_id);

  /// @brief ID 番号から IdCell を得る．
  IdCell*
  id2cell(ymuint32 id) const;

  /// @brief ID 番号から文字列を得る．
  const char*
  id2str(ymuint32 id) const;

  /// @brief ID 番号から位置情報を得る．
  FileRegion
  id2loc(ymuint32 id) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 文字列用の領域を確保する．
  /// @param[in] src_str ソース文字列
  /// @param[in] loc 文字列の位置情報
  /// @return 文字列の ID 番号
  ymuint32
  reg_str(const char* src_str,
	  const FileRegion& loc);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 字句解析器
  Iscas89Scanner* mScanner;

  // イベントハンドラのリスト
  list<Iscas89Handler*> mHandlerList;

  // 文字列のハッシュ
  Iscas89IdHash mIdHash;

  // 位置情報バッファ
  FileRegion mLoc1;

  // 入力IDのリスト
  vector<ymuint32> mInputList;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief ID 番号から IdCell を得る．
inline
IdCell*
Iscas89ParserImpl::id2cell(ymuint32 id) const
{
  return mIdHash.cell(id);
}

// @brief ID 番号から文字列を得る．
inline
const char*
Iscas89ParserImpl::id2str(ymuint32 id) const
{
  return mIdHash.str(id);
}

// @brief ID 番号から位置情報を得る．
inline
FileRegion
Iscas89ParserImpl::id2loc(ymuint32 id) const
{
  return mIdHash.loc(id);
}

END_NAMESPACE_YM_ISCAS89

#endif // ISCAS89PARSERIMPL_H
