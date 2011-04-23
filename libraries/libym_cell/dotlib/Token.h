#ifndef LIBYM_CELL_DOTLIB_TOKEN_H
#define LIBYM_CELL_DOTLIB_TOKEN_H

/// @file libym_cell/dotlib/Token.h
/// @brief Token のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "dotlib_nsdef.h"
#include "ym_utils/FileRegion.h"


BEGIN_NAMESPACE_YM_CELL_DOTLIB

//////////////////////////////////////////////////////////////////////
/// @class Token Token.h "Token.h"
/// @brief トークンを表すクラス
//////////////////////////////////////////////////////////////////////
class Token
{
public:

  /// @brief 空のコンストラクタ
  /// @note エラートークンを表す．
  Token();

  /// @brief 内容を指定したコンストラクタ
  /// @param[in] type 型
  /// @param[in] value 値
  /// @param[in] loc ファイル上の位置
  Token(tTokenType type,
	const string& value = string(),
	const FileRegion& loc = FileRegion());

  /// @brief デストラクタ
  ~Token();


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を取得する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を得る．
  tTokenType
  type() const;

  /// @brief 値を得る．
  string
  value() const;

  /// @brief ファイル上の位置を得る．
  FileRegion
  loc() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 型
  tTokenType mType;

  // 値
  string mValue;

  // ファイル上の位置
  FileRegion mLoc;

};

/// @relates Token
/// @brief 内容をストリームに出力する．
ostream&
operator<<(ostream& s,
	   Token token);


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 空のコンストラクタ
// @note エラートークンを表す．
inline
Token::Token() :
  mType(ERROR)
{
}

// @brief 内容を指定したコンストラクタ
// @param[in] type 型
// @param[in] value 値
// @param[in] loc ファイル上の位置
inline
Token::Token(tTokenType type,
	     const string& value,
	     const FileRegion& loc) :
  mType(type),
  mValue(value),
  mLoc(loc)
{
}

// @brief デストラクタ
inline
Token::~Token()
{
}

// @brief 型を得る．
inline
tTokenType
Token::type() const
{
  return mType;
}

// @brief 値を得る．
inline
string
Token::value() const
{
  return mValue;
}

// @brief ファイル上の位置を得る．
inline
FileRegion
Token::loc() const
{
  return mLoc;
}

END_NAMESPACE_YM_CELL_DOTLIB

#endif // LIBYM_CELL_DOTLIB_TOKEN_H
