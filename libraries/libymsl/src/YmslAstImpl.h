#ifndef YMSLASTIMPL_H
#define YMSLASTIMPL_H

/// @file YmslAstImpl.h
/// @brief YmslAstImpl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmslAst.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class YmslAstImpl YmslAstImpl.h "YmslAstImpl.h"
/// @brief YmslAst の実装クラス
//////////////////////////////////////////////////////////////////////
class YmslAstImpl :
  public YmslAst
{
public:

  /// @brief コンストラクタ
  /// @param[in] loc ファイル位置
  YmslAstImpl(const FileRegion& loc);

  /// @brief デストラクタ
  virtual
  ~YmslAstImpl();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置を得る．
  virtual
  const FileRegion&
  file_region() const;

  /// @brief 文字列型の値を返す．
  virtual
  const char*
  str_val() const;

  /// @brief 整数型の値を返す．
  virtual
  int
  int_val() const;

  /// @brief 浮動小数点型の値を返す．
  virtual
  double
  float_val() const;

  /// @brief 子供の数を返す．
  virtual
  ymuint
  child_num() const;

  /// @brief 子供を返す．
  /// @param[in] pos 位置( 0 <= pos < child_num() )
  virtual
  YmslAst*
  child(ymuint pos) const;

  /// @brief 子供を追加する．
  virtual
  void
  add_child(YmslAst* child);


public:
  //////////////////////////////////////////////////////////////////////
  // print() で用いる便利関数
  //////////////////////////////////////////////////////////////////////

  /// @brief インデント用の空白を出力する
  /// @param[in] s 出力ストリーム
  /// @param[in] indent インデント量
  static
  void
  print_indent(ostream& s,
	       ymuint indent);

  /// @brief ステートメントリストを出力する
  /// @param[in] s 出力ストリーム
  /// @param[in] statement_list ステートメントリスト
  /// @param[in] indent インデント量
  static
  void
  print_statement_list(ostream& s,
		       YmslAst* statement_list,
		       ymuint indent);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mLoc;

};

END_NAMESPACE_YM_YMSL


#endif // YMSLASTIMPL_H
