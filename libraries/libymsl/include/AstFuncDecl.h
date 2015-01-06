#ifndef ASTFUNCDECL_H
#define ASTFUNCDECL_H

/// @file AstFuncDecl.h
/// @brief AstFuncDecl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "AstStatement.h"
#include "YmUtils/ShString.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class AstFuncDecl AstFuncDecl.h "AstFuncDecl.h"
/// @brief 関数宣言を表すクラス
//////////////////////////////////////////////////////////////////////
class AstFuncDecl :
  public AstStatement
{
  friend class YmslModule;

public:

  /// @brief コンストラクタ
  /// @param[in] name 関数名
  /// @param[in] type 型
  /// @param[in] param_list パラメータリスト
  /// @param[in] stmt_list 本体の文
  /// @param[in] loc ファイル位置
  AstFuncDecl(ShString name,
	      ValueType type,
	      AstVarList* param_list,
	      AstStmtList* stmt_list,
	      const FileRegion& loc);

  /// @brief デストラクタ
  virtual
  ~AstFuncDecl();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前を得る．
  ShString
  name() const;

  /// @brief インデックス番号を返す．
  ymuint
  index() const;

  /// @brief 出力の型を返す．
  ValueType
  type() const;

  /// @brief パラメータリストを返す．
  const vector<AstVarDecl*>&
  param_list() const;

  /// @brief 本体のリストを返す．
  const vector<AstStatement*>&
  stmt_list() const;


public:
  //////////////////////////////////////////////////////////////////////
  // AstStatement の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 命令コードのサイズを計算する．
  virtual
  ymuint
  calc_size();

  /// @brief 命令コードを生成する．
  /// @param[in] driver ドライバ
  /// @param[in] code_list 命令コードの格納先
  /// @param[inout] addr 命令コードの現在のアドレス
  ///
  /// addr の値は更新される．
  virtual
  void
  compile(YmslDriver& driver,
	  YmslCodeList& code_list,
	  Ymsl_INT& addr);

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

  // 名前
  ShString mName;

  // インデックス番号
  ymuint mIndex;

  // 出力の型
  ValueType mType;

  // パラメータリスト
  vector<AstVarDecl*> mParamList;

  // 本体の文リスト
  vector<AstStatement*> mStmtList;

};

END_NAMESPACE_YM_YMSL

#endif // ASTFUNCDECL_H
