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
#include "ym_utils/SimpleAlloc.h"
#include "ym_utils/ShString.h"

#include "SmtLibScanner.h"


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

  /// @brief ファイルを開く
  /// @param[in] filename ファイル名
  /// @retval true 成功した．
  /// @retval false 失敗した．
  bool
  open(const string& filename);

  /// @brief S式を一つ読み込む．
  /// @param[out] error エラーが起きたら true を格納する．
  /// @return 読み込んだ S式を表すノードを返す．
  /// @note 末尾まで読んでいたら NULL を返す．
  SmtLibNode*
  read(bool& error);

  /// @brief 今までに生成したすべてのオブジェクトを解放する．
  void
  clear();


private:
  //////////////////////////////////////////////////////////////////////
  // read() の中で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief S式の読み込みを行う．
  /// @param[out] node 読み込んだ S式を格納するノード
  /// @param[out] loc S式のファイル上の位置
  /// @return トークンの型を返す．
  /// @note node は NULL の場合もある．
  tTokenType
  read_sexp(SmtLibNode*& node,
	    FileRegion& loc);

  /// @brief NUM タイプのノードを生成する．
  /// @param[in] loc ファイル上の位置
  /// @param[in] val 値
  SmtLibNode*
  new_num(const FileRegion& loc,
	  const ShString& val);

  /// @brief DEC タイプのノードを生成する．
  /// @param[in] loc ファイル上の位置
  /// @param[in] val 値
  SmtLibNode*
  new_dec(const FileRegion& loc,
	  const ShString& val);

  /// @brief HEX タイプのノードを生成する．
  /// @param[in] loc ファイル上の位置
  /// @param[in] val 値
  SmtLibNode*
  new_hex(const FileRegion& loc,
	  const ShString& val);

  /// @brief BIN タイプのノードを生成する．
  /// @param[in] loc ファイル上の位置
  /// @param[in] val 値
  SmtLibNode*
  new_bin(const FileRegion& loc,
	  const ShString& val);

  /// @brief STRING タイプのノードを生成する．
  /// @param[in] loc ファイル上の位置
  /// @param[in] val 値
  SmtLibNode*
  new_string(const FileRegion& loc,
	     const ShString& val);

  /// @brief SYMBOL タイプのノードを生成する．
  /// @param[in] loc ファイル上の位置
  /// @param[in] val 値
  SmtLibNode*
  new_symbol(const FileRegion& loc,
	     const ShString& val);

  /// @brief KEYWORD タイプのノードを生成する．
  /// @param[in] loc ファイル上の位置
  /// @param[in] val 値
  SmtLibNode*
  new_keyword(const FileRegion& loc,
	      const ShString& val);

  /// @brief LIST タイプのノードを生成する．
  /// @param[in] loc ファイル上の位置
  /// @param[in] child_list 子供のノードのリスト
  SmtLibNode*
  new_list(const FileRegion& loc,
	   const list<SmtLibNode*>& child_list);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // SmtLibNode のメモリ確保用アロケータ
  SimpleAlloc mAlloc;

  // 字句解析器
  SmtLibScanner mScanner;

};

END_NAMESPACE_YM_SMTLIBV2

#endif // LIBYM_SMTLIBV2_PARSER_SMTLIBPARSER_H
