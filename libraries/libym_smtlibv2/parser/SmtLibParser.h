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

  /// @brief 今までに生成したすべてのオブジェクトを解放する．
  void
  clear();


private:
  //////////////////////////////////////////////////////////////////////
  // read() の中で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief S式の読み込みを行う．
  /// @param[out] node 読み込んだ S式を格納するノード
  /// @return トークンの型を返す．
  tTokenType
  read_sexp(SmtLibNode*& node);

  /// @brief NUM タイプのノードを生成する．
  /// @param[in] loc ファイル上の位置
  /// @param[in] val 値
  SmtLibNode*
  new_num(const FileRegion& loc,
	  StrId val);

  /// @brief DEC タイプのノードを生成する．
  /// @param[in] loc ファイル上の位置
  /// @param[in] val 値
  SmtLibNode*
  new_dec(const FileRegion& loc,
	  StrId val);

  /// @brief HEX タイプのノードを生成する．
  /// @param[in] loc ファイル上の位置
  /// @param[in] val 値
  SmtLibNode*
  new_hex(const FileRegion& loc,
	  StrId val);

  /// @brief BIN タイプのノードを生成する．
  /// @param[in] loc ファイル上の位置
  /// @param[in] val 値
  SmtLibNode*
  new_bin(const FileRegion& loc,
	  StrId val);

  /// @brief STRING タイプのノードを生成する．
  /// @param[in] loc ファイル上の位置
  /// @param[in] val 値
  SmtLibNode*
  new_string(const FileRegion& loc,
	     StrId val);

  /// @brief SYMBOL タイプのノードを生成する．
  /// @param[in] loc ファイル上の位置
  /// @param[in] val 値
  SmtLibNode*
  new_symbol(const FileRegion& loc,
	     StrId val);

  /// @brief KEYWORD タイプのノードを生成する．
  /// @param[in] loc ファイル上の位置
  /// @param[in] val 値
  SmtLibNode*
  new_keyword(const FileRegion& loc,
	      StrId val);

  /// @brief LIST タイプのノードを生成する．
  /// @param[in] loc ファイル上の位置
  /// @param[in] child_list 子供のノードのリスト
  SmtLibNode*
  new_list(const FileRegion& loc,
	   const list<SmtLibNode*>& child_list);

  /// @brief エラーメッセージを出力する．
  /// @note 副作用で mError が true にセットされる．
  void
  error(const FileRegion& loc,
	const char* msg);

  /// @brief S式の内容を出力する．
  /// @note デバッグ用
  void
  dump(ostream& s,
       const SmtLibNode* node);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // SmtLibNode のメモリ確保用アロケータ
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
