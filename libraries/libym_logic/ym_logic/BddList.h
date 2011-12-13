#ifndef YM_LOGIC_BDDLIST_H
#define YM_LOGIC_BDDLIST_H

/// @file ym_logic/BddList.h
/// @brief BddList のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_logic/bdd_nsdef.h"


BEGIN_NAMESPACE_YM_BDD
#if 1
//////////////////////////////////////////////////////////////////////
/// @class BddList BddList.h "ym_logic/BddList.h"
/// @brief Bdd のリストを表すクラス
//////////////////////////////////////////////////////////////////////
class BddList :
  public vector<Bdd>
{
public:

  /// @brief 空のコンストラクタ
  /// @param[in] mgr BddMgr
  BddList(BddMgr& mgr);

  /// @brief デストラクタ
  ~BddList();


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

#endif // YM_LOGIC_BDDLIST_H
