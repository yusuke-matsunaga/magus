#ifndef YMSLPARSER_H
#define YMSLPARSER_H

/// @file YmslParser.h
/// @brief YmslParser のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ymsl_int.h"
#include "YmUtils/IDO.h"
#include "YmUtils/FileRegion.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class YmslParser YmslParser.h "YmslParser.h"
/// @brief YMSL 用の構文解析器
//////////////////////////////////////////////////////////////////////
class YmslParser
{
public:

  /// @brief コンストラクタ
  YmslParser();

  /// @brief デストラクタ
  ~YmslParser();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 読み込む．
  /// @param[in] ido 入力データ
  /// @return 成功したら true を返す．
  bool
  read(IDO& ido);

  /// @brief yylex とのインターフェイス
  /// @param[out] lvalp 値を格納する変数
  /// @param[out] llocp 位置情報を格納する変数
  /// @return 読み込んだトークンの id を返す．
  int
  yylex(YmslAst*& lval,
	FileRegion& lloc);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief + 式を作る．
  /// @param[in] left, right オペランド
  YmslAst*
  new_AstPlus(YmslAst* left,
	      YmslAst* right);

  /// @brief - 式を作る．
  /// @param[in] left, right オペランド
  YmslAst*
  new_AstMinus(YmslAst* left,
	       YmslAst* right);

  /// @brief * 式を作る．
  /// @param[in] left, right オペランド
  YmslAst*
  new_AstMult(YmslAst* left,
	      YmslAst* right);

  /// @brief / 式を作る．
  /// @param[in] left, right オペランド
  YmslAst*
  new_AstDiv(YmslAst* left,
	     YmslAst* right);

  /// @brief % 式を作る．
  /// @param[in] left, right オペランド
  YmslAst*
  new_AstMod(YmslAst* left,
	     YmslAst* right);

  /// @brief 識別子式を作る．
  /// @param[in] val 値
  /// @param[in] loc ファイル位置
  YmslAst*
  new_AstSymbol(const char* val,
		const FileRegion& loc);

  /// @brief 文字列定数式を作る．
  /// @param[in] val 値
  /// @param[in] loc ファイル位置
  YmslAst*
  new_AstString(const char* val,
		const FileRegion& loc);

  /// @brief 整数定数式を作る．
  /// @param[in] val 値
  /// @param[in] loc ファイル位置
  YmslAst*
  new_AstInt(int val,
	     const FileRegion& loc);

  /// @brief 浮動小数点定数式を作る．
  /// @param[in] val 値
  /// @param[in] loc ファイル位置
  YmslAst*
  new_AstFloat(double val,
	       const FileRegion& loc);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 字句解析器
  YmslScanner* mScanner;

  // 構文木の根
  YmslAst* mRoot;

};

END_NAMESPACE_YM_YMSL


#endif // YMSLPARSER_H
