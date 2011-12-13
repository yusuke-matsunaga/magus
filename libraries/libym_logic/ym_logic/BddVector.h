#ifndef YM_LOGIC_BDDVECTOR_H
#define YM_LOGIC_BDDVECTOR_H

/// @file ym_logic/BddVector.h
/// @brief BddVector のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_logic/bdd_nsdef.h"


BEGIN_NAMESPACE_YM_BDD
#if 1
//////////////////////////////////////////////////////////////////////
/// @class BddVector BddVector.h "ym_logic/BddVector.h"
/// @brief Bdd のベクタを表すクラス
//////////////////////////////////////////////////////////////////////
class BddVector :
  public vector<Bdd>
{
public:

  /// @brief 空のコンストラクタ
  /// @param[in] mgr BddMgr
  BddVector(BddMgr& mgr);

  /// @brief 要素数を指定したコンストラクタ
  /// @param[in] mgr BddMgr
  /// @param[in] n 要素数
  BddVector(BddMgr& mgr,
	    ymuint n);

  /// @brief デストラクタ
  ~BddVector();


public:
  //////////////////////////////////////////////////////////////////////
  // 要素に対する演算
  //////////////////////////////////////////////////////////////////////

  /// @brief 要素のBDDの論理積を求める．
  /// @return 生成された BDD
  Bdd
  and_op() const;

  /// @brief 要素のBDDの論理和を求める．
  /// @return 生成された BDD
  Bdd
  or_op() const;

  /// @brief 要素のBDDの排他的論理和を求める．
  /// @return 生成された BDD
  Bdd
  xor_op() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // BddMgr
  BddMgr& mMgr;

};
#endif
END_NAMESPACE_YM_BDD

#endif // YM_LOGIC_BDDVECTOR_H
