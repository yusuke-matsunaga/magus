#ifndef EUFVAR_H
#define EUFVAR_H

/// @file EufVar.h
/// @brief EufVar のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "EufNode.h"


BEGIN_NAMESPACE_YM_LLVMEQ

//////////////////////////////////////////////////////////////////////
/// @class EufVar EufVar.h "EufVar.h"
/// @brief EUF 式の変数を表すクラス
//////////////////////////////////////////////////////////////////////
class EufVar :
  public EufNode
{
  friend class EufVarMgr;

protected:

  /// @brief コンストラクタ
  /// @param[in] id ID番号
  /// @param[in] vid SatSolver 用の変数番号
  /// @param[in] name 変数名
  EufVar(ymuint id,
	 VarId vid,
	 const string& name);

  /// @brief デストラクタ
  virtual
  ~EufVar();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を得る．
  virtual
  tType
  type() const;

  /// @brief Boolean 型の時 true を返す．
  /// @note 具体的には kCon, kDis, kNeg, kBVar の時 true を返す．
  virtual
  bool
  is_boolean() const;

  /// @brief 識別子名を得る．
  /// @note type() が kFunc, kVar の時のみ有効
  virtual
  string
  id_name() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 名前
  string mName;

  // ハッシュで用いるリンク
  EufVar* mLink;

};

END_NAMESPACE_YM_LLVMEQ

#endif // EUFVAR_H
