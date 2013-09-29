#ifndef EUFEQ_H
#define EUFEQ_H

/// @file EufEq.h
/// @brief EufEq のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "EufBin.h"


BEGIN_NAMESPACE_YM_LLVMEQ

//////////////////////////////////////////////////////////////////////
/// @class EufEq EufEq.h "EufEq.h"
/// @brief euqality ノードを表すクラス
//////////////////////////////////////////////////////////////////////
class EufEq :
  public EufBin
{
  friend class EufBinMgr;

private:

  /// @brief コンストラクタ
  /// @param[in] id ID番号
  /// @param[in] vid SatSolver 用の変数番号
  /// @param[in] left, right 左辺と右辺の式
  EufEq(ymuint id,
	VarId vid,
	EufNode* left,
	EufNode* right);

  /// @brief デストラクタ
  virtual
  ~EufEq();


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

#endif // EUFEQ_H
