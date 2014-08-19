#ifndef EUFNEG_H
#define EUFNEG_H

/// @file EufNeg.h
/// @brief EufNeg のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "EufBin.h"


BEGIN_NAMESPACE_YM_LLVMEQ

//////////////////////////////////////////////////////////////////////
/// @class EufNeg EufNeg.h "EufNeg.h"
/// @brief EUF 式の否定演算子を表すクラス
///
/// ちょっと無駄だけどコードを簡単にするために二項演算子の
/// 継承クラスとなっている．
//////////////////////////////////////////////////////////////////////
class EufNeg :
  public EufBin
{
  friend class EufBinMgr;

private:

  /// @brief コンストラクタ
  /// @param[in] id ID番号
  /// @param[in] vid SatSolver 用の変数番号
  /// @param[in] operand オペランド
  EufNeg(ymuint id,
	 VarId vid,
	 EufNode* operand);

  /// @brief デストラクタ
  virtual
  ~EufNeg();


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

#endif // EUFNEG_H
