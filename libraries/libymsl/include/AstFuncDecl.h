#ifndef ASTFUNCDECL_H
#define ASTFUNCDECL_H

/// @file AstFuncDecl.h
/// @brief AstFuncDecl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "AstDeclBase.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class AstFuncDecl AstFuncDecl.h "AstFuncDecl.h"
/// @brief 関数宣言を表すクラス
//////////////////////////////////////////////////////////////////////
class AstFuncDecl :
  public AstDeclBase
{
  friend class YmslModule;

public:

  /// @brief コンストラクタ
  /// @param[in] name 関数名
  /// @param[in] type 型
  /// @param[in] param_list パラメータリスト
  /// @param[in] block 本文のブロック
  /// @param[in] loc ファイル位置
  AstFuncDecl(ShString name,
	      ValueType type,
	      AstVarDecl* param_list,
	      AstBlock* block,
	      const FileRegion& loc);

  /// @brief デストラクタ
  virtual
  ~AstFuncDecl();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief インデックス番号を返す．
  ymuint
  index() const;

  /// @brief 出力の型を返す．
  ValueType
  type() const;

  /// @brief パラメータリストを返す．
  const vector<AstVarDecl*>&
  param_list() const;

  /// @brief ブロックを返す．
  AstBlock*
  block() const;

  /// @brief 内容を表示する．(デバッグ用)
  /// @param[in] s 出力ストリーム
  /// @param[in] indent インデントレベル
  virtual
  void
  print(ostream& s,
	ymuint indent = 0) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // インデックス番号
  ymuint mIndex;

  // 出力の型
  ValueType mType;

  // パラメータリスト
  vector<AstVarDecl*> mParamList;

  // 本体のブロック
  AstBlock* mBlock;

};

END_NAMESPACE_YM_YMSL

#endif // ASTFUNCDECL_H
