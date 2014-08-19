#ifndef EUFCON_H
#define EUFCON_H

/// @file EufCon.h
/// @brief EufCon のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "EufBin.h"


BEGIN_NAMESPACE_YM_LLVMEQ

//////////////////////////////////////////////////////////////////////
/// @class EufCon EufCon.h "EufCon.h"
/// @brief conjunctive ノードを表すクラス
//////////////////////////////////////////////////////////////////////
class EufCon :
  public EufBin
{
  friend class EufBinMgr;

private:

  /// @brief コンストラクタ
  /// @param[in] id ID番号
  /// @param[in] vid SatSolver 用の変数番号
  /// @param[in] left, right 左辺と右辺の式
  EufCon(ymuint id,
	 VarId vid,
	 EufNode* left,
	 EufNode* right);

  /// @brief デストラクタ
  virtual
  ~EufCon();


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

#endif // EUFCON_H
