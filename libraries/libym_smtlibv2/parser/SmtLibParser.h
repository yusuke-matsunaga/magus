#ifndef LIBYM_SMTLIBV2_PARSER_SMTLIBPARSER_H
#define LIBYM_SMTLIBV2_PARSER_SMTLIBPARSER_H

/// @file libym_cell/mislib/MislibParser.h
/// @brief MislibParser のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: MislibParser.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_smtlibv2/smtlibv2_nsdef.h"
#include "ym_utils/MsgHandler.h"
#include "ym_utils/FileRegion.h"
#include "ym_utils/Alloc.h"
#include "ym_utils/ShString.h"

#include "SmtLibLex.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

class SmtLibPt
{
  int foo;
};

//////////////////////////////////////////////////////////////////////
/// @class SmtLibParser SmtLibParser.h "SmtLibParser.h"
/// @brief SmtLib ファイルのパーサーの実装クラス
//////////////////////////////////////////////////////////////////////
class SmtLibParser
{
public:

  /// @brief コンストラクタ
  SmtLibParser();

  /// @brief デストラクタ
  ~SmtLibParser();


public:

  /// @brief smtlib ファイルを読み込んでライブラリを生成する．
  /// @param[in] filename ファイル名
  /// @return 生成したライブラリを返す．
  /// @note 読み込みが失敗したら NULL を返す．
  void
  read(const string& filename);

  /// @brief メッセージマネージャの取得
  MsgMgr&
  msg_mgr();


public:
  //////////////////////////////////////////////////////////////////////
  // smtlib_grammer.yy で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 今までに生成したすべてのオブジェクトを解放する．
  void
  clear();

  /// @brief 字句解析を行う．
  /// @param[out] lval トークンの値を格納する変数
  /// @param[out] lloc トークンの位置を格納する変数
  /// @return トークンの型を返す．
  int
  scan(SmtLibPt*& lval,
       FileRegion& lloc);

  /// @brief エラーメッセージを出力する．
  /// @note 副作用で mError が true にセットされる．
  void
  error(const FileRegion& loc,
	const char* msg);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部でのみ使われるメンバ関数
  //////////////////////////////////////////////////////////////////////

private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // SmtLibPt のメモリ確保用アロケータ
  SimpleAlloc mAlloc;

  // 字句解析器
  SmtLibLex mLex;

  // メッセージハンドラの管理者
  MsgMgr mMsgMgr;

  // 読み込み時のエラーの有無を示すフラグ
  bool mError;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief メッセージマネージャの取得
inline
MsgMgr&
SmtLibParser::msg_mgr()
{
  return mMsgMgr;
}

END_NAMESPACE_YM_SMTLIBV2

#endif // LIBYM_SMTLIBV2_PARSER_SMTLIBPARSER_H
