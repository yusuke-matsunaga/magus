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

#include "TokenType.h"
#include "SmtLibScanner.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

class SmtLibNode;

//////////////////////////////////////////////////////////////////////
/// @class SmtLibParser SmtLibParser.h "SmtLibParser.h"
/// @brief SmtLib ファイルのパーサーの実装クラス
//////////////////////////////////////////////////////////////////////
class SmtLibParser
{
public:
  //////////////////////////////////////////////////////////////////////
  // 型の定義
  //////////////////////////////////////////////////////////////////////

  /// @brief read() の結果を表す列挙型
  enum tResult {
    /// @brief 成功
    kOK,
    /// @brief エラー
    kError,
    /// @brief EOF
    kEOF,
    /// @brief unexpected EOF (括弧が閉じられていない)
    kOpen
  };


public:

  /// @brief コンストラクタ
  SmtLibParser();

  /// @brief デストラクタ
  ~SmtLibParser();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 初期化する．
  /// @param[in] ido 入力データ
  /// @note この関数を呼ぶと以前のパース結果の SmtLibNode は破壊されるので注意
  void
  init(IDO* ido);

  /// @brief S式を一つ読み込む．
  /// @param[out] root 読んだS式の根のノード
  /// @return 結果を返す．
  tResult
  read(SmtLibNode*& root);

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
	  ymint32 val);

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
  /// @param[in] num 子供の数
  /// @param[in] child 子供のノード
  SmtLibNode*
  new_list(const FileRegion& loc,
	   ymuint num,
	   const SmtLibNode* child);


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
