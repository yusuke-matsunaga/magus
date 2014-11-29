#ifndef YMSLASTUNIOP_H
#define YMSLASTUNIOP_H

/// @file YmslAstUniOp.h
/// @brief YmslAstUniOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmslAstImpl.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class YmslAstUniOp YmslAstUniOp.h "YmslAstUniOp.h"
/// @brief 単項演算子を表す YmslAst
//////////////////////////////////////////////////////////////////////
class YmslAstUniOp :
  public YmslAstImpl
{
public:

  /// @brief コンストラクタ
  /// @param[in] child 子供のノード
  /// @param[in] loc ファイル位置
  YmslAstUniOp(YmslAst* child,
	       const FileRegion& loc);

  /// @brief デストラクタ
  virtual
  ~YmslAstUniOp();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 子供の数を返す．
  virtual
  ymuint
  child_num() const;

  /// @brief 子供を返す．
  /// @param[in] pos 位置( 0 <= pos < child_num() )
  virtual
  YmslAst*
  child(ymuint pos) const;

  /// @brief 内容を表示する．(デバッグ用)
  /// @param[in] s 出力ストリーム
  virtual
  void
  print(ostream& s) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 子供のノード
  YmslAst* mChild;

};


//////////////////////////////////////////////////////////////////////
/// @class YmslAstUniPlus YmslAstUniPlus.h "YmslAstUniPlus.h"
/// @brief 単項プラスを表すクラス
//////////////////////////////////////////////////////////////////////
class YmslAstUniPlus :
  public YmslAstUniOp
{
public:

  /// @brief コンストラクタ
  /// @param[in] child 子供のノード
  /// @param[in] loc ファイル位置
  YmslAstUniPlus(YmslAst* child,
		 const FileRegion& loc);

  /// @brief デストラクタ
  virtual
  ~YmslAstUniPlus();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を得る．
  virtual
  AstType
  type() const;

};


//////////////////////////////////////////////////////////////////////
/// @class YmslAstUniMinus YmslAstUniOp.h "YmslAstUniOp.h"
/// @brief 単項マイナスを表すクラス
//////////////////////////////////////////////////////////////////////
class YmslAstUniMinus :
  public YmslAstUniOp
{
public:

  /// @brief コンストラクタ
  /// @param[in] child 子供のノード
  /// @param[in] loc ファイル位置
  YmslAstUniMinus(YmslAst* child,
		  const FileRegion& loc);

  /// @brief デストラクタ
  virtual
  ~YmslAstUniMinus();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を得る．
  virtual
  AstType
  type() const;

};


//////////////////////////////////////////////////////////////////////
/// @class YmslAstLogNot YmslAstUniOp.h "YmslAstUniOp.h"
/// @brief 論理否定を表すクラス
//////////////////////////////////////////////////////////////////////
class YmslAstLogNot :
  public YmslAstUniOp
{
public:

  /// @brief コンストラクタ
  /// @param[in] child 子供のノード
  /// @param[in] loc ファイル位置
  YmslAstLogNot(YmslAst* child,
		const FileRegion& loc);

  /// @brief デストラクタ
  virtual
  ~YmslAstLogNot();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を得る．
  virtual
  AstType
  type() const;

};


//////////////////////////////////////////////////////////////////////
/// @class YmslAstBitNeg YmslAstUniOp.h "YmslAstUniOp.h"
/// @brief ビット否定を表すクラス
//////////////////////////////////////////////////////////////////////
class YmslAstBitNeg :
  public YmslAstUniOp
{
public:

  /// @brief コンストラクタ
  /// @param[in] child 子供のノード
  /// @param[in] loc ファイル位置
  YmslAstBitNeg(YmslAst* child,
		 const FileRegion& loc);

  /// @brief デストラクタ
  virtual
  ~YmslAstBitNeg();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を得る．
  virtual
  AstType
  type() const;

};

END_NAMESPACE_YM_YMSL


#endif // YMSLASTUNIOP_H
