#ifndef SMTLIBPARSER_H
#define SMTLIBPARSER_H

/// @file SmtLibParser.h
/// @brief SmtLibParser のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_smtlibv2/smtlibv2_nsdef.h"
#include "ym_utils/IDO.h"
#include "ym_utils/FileRegion.h"
#include "ym_utils/SimpleAlloc.h"
#include "ym_utils/ShString.h"

#include "TokenType.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

class SmtLibScanner;
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
  /// @note 内部で clear() を呼ぶ．
  void
  init(IDO* ido);

  /// @brief 今までに生成したすべてのオブジェクトを解放する．
  void
  clear();

  /// @brief S式を一つ読み込む．
  /// @param[out] root 読んだS式の根のノード
  /// @return 結果を返す．
  tResult
  read(SmtLibNode*& root);


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
  SmtLibScanner* mScanner;

};

END_NAMESPACE_YM_SMTLIBV2

#endif // SMTLIBPARSER_H
