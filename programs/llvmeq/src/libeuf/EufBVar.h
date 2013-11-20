#ifndef EUFBVAR_H
#define EUFBVAR_H

/// @file EufBVar.h
/// @brief EufBVar のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "EufVar.h"


BEGIN_NAMESPACE_YM_LLVMEQ

//////////////////////////////////////////////////////////////////////
/// @class EufBVar EufBVar.h "EufBVar.h"
/// @brief ブール変数を表すクラス
//////////////////////////////////////////////////////////////////////
class EufBVar :
  public EufVar
{
  friend class EufVarMgr;

private:

  /// @brief コンストラクタ
  /// @param[in] id ID番号
  /// @param[in] vid SatSolver 用の変数番号
  /// @param[in] name 変数名
  EufBVar(ymuint id,
	  VarId vid,
	  const string& name);

  /// @brief デストラクタ
  virtual
  ~EufBVar();


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

};

END_NAMESPACE_YM_LLVMEQ

#endif // EUFVAR_H
