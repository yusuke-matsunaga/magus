#ifndef EUFBIN_H
#define EUFBIN_H

/// @file EufBin.h
/// @brief EufBin のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "EufNode.h"


BEGIN_NAMESPACE_YM_LLVMEQ

//////////////////////////////////////////////////////////////////////
/// @class EufBin EufBin.h "EufBin.h"
/// @brief EUF 式の二項演算子を表す基底クラス
//////////////////////////////////////////////////////////////////////
class EufBin :
  public EufNode
{
  friend class EufBinMgr;

protected:

  /// @brief コンストラクタ
  /// @param[in] id ID番号
  /// @param[in] vid SatSolver 用の変数番号
  /// @param[in] left, right 左辺と右辺の式
  EufBin(ymuint id,
	 VarId vid,
	 EufNode* left,
	 EufNode* right);

  /// @brief デストラクタ
  virtual
  ~EufBin();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 左辺の式を得る．
  /// @note type() が kCon, kDis, kNeg, kEq の時のみ有効
  virtual
  EufNode*
  left() const;

  /// @brief 右辺の式を得る．
  /// @note type() が kCon, kDis, kEq の時のみ有効
  virtual
  EufNode*
  right() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 左辺
  EufNode* mLeft;

  // 右辺
  EufNode* mRight;

  // ハッシュ用のリンクポインタ
  EufBin* mLink;

};

END_NAMESPACE_YM_LLVMEQ

#endif // EUFBIN_H
