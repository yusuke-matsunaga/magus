#ifndef ASTTOPLEVEL_H
#define ASTTOPLEVEL_H

/// @file AstToplevel.h
/// @brief AstToplevel のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "AstBlockStmt.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class AstToplevel AstToplevel.h "AstToplevel.h"
/// @brief AST のトップレベルを表すクラス
//////////////////////////////////////////////////////////////////////
class AstToplevel :
  public AstBlockStmt
{
public:

  /// @brief コンストラクタ
  /// @param[in] head_num ヘッダのリストの要素数
  /// @param[in] head_list ヘッダのリスト
  /// @param[in] stmt_num 文のリストの要素数
  /// @param[in] stmt_list 文のリスト
  /// @param[in] loc ファイル位置
  AstToplevel(ymuint head_num,
	      AstStatement** head_list,
	      ymuint stmt_num,
	      AstStatement** stmt_list,
	      const FileRegion& loc);

  /// @brief デストラクタ
  virtual
  ~AstToplevel();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 種類を返す．
  virtual
  Type
  stmt_type() const;

  /// @brief ヘッダのリストの要素数を返す．
  ///
  /// AstToplevel のみ有効
  virtual
  ymuint
  headlist_num() const;

  /// @brief ヘッダを返す．
  /// @param[in] pos 位置 ( 0 <= pos < headlist_num() )
  ///
  /// AstToplevel のみ有効
  virtual
  const AstStatement*
  headlist_elem(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ヘッダリストの要素数
  ymuint mHeadNum;

  // ヘッダリスト(ポインタ配列)
  AstStatement** mHeadList;

};

END_NAMESPACE_YM_YMSL

#endif // ASTTOPLEVEL_H
