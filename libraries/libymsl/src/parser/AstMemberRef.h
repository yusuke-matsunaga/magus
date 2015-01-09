#ifndef ASTMEMBERREF_H
#define ASTMEMBERREF_H

/// @file AstMemberRef.h
/// @brief AstMemberRef のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "AstExpr.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class AstMemberRef AstMemberRef.h "AstMemberRef.h"
/// @brief メンバ参照を表すクラス
//////////////////////////////////////////////////////////////////////
class AstMemberRef :
  public AstExpr
{
public:

  /// @brief コンストラクタ
  /// @param[in] id オブジェクト名
  /// @param[in] member メンバ名
  /// @param[in] loc ファイル位置
  AstMemberRef(AstExpr* id,
	       AstSymbol* member,
	       const FileRegion& loc);

  /// @brief デストラクタ
  virtual
  ~AstMemberRef();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 変数の参照を解決する．
  virtual
  void
  resolve_var(YmslScope* parent_scope);

  /// @brief 式の型を解析する．
  /// @return 引数の方が間違っていたら false を返す．
  ///
  /// 結果としてキャスト演算が挿入される場合もある．
  virtual
  bool
  type_analysis();

  /// @brief 式の型を返す．
  virtual
  const YmslType*
  type();

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
  virtual
  void
  print(ostream& s) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // オブジェクト名
  AstExpr* mId;

  // メンバ名
  AstSymbol* mMember;

};

END_NAMESPACE_YM_YMSL

#endif // ASTMEMBERREF_H
