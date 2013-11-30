#ifndef COMBSET_H
#define COMBSET_H

/// @file CombSet.h
/// @brief CombSet のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_logic/zdd_nsdef.h"
#include "ym_logic/VarId.h"


BEGIN_NAMESPACE_YM_ZDD

//////////////////////////////////////////////////////////////////////
/// @class CombSet CombSet.h "CombSet.h"
/// @brief 組み合わせ集合を表す単純なクラス
//////////////////////////////////////////////////////////////////////
class CombSet
{
public:

  /// @brief コンストラクタ
  /// @param[in] num 要素数
  /// @note 空の集合となる．
  CombSet(ymuint num);

  /// @brief デストラクタ
  ~CombSet();


public:

  /// @brief 要素数を返す．
  ymuint
  num() const;

  /// @brief 空集合 {} にする．
  const CombSet&
  make_empty();

  /// @brief base set {{}} にする．
  const CombSet&
  make_base();

  /// @brief var の出現を反転させる．
  /// @param[in] var 変数番号
  const CombSet&
  swap(VarId var);

  /// @brief intersection を計算する．
  friend
  CombSet
  operator&(const CombSet& left,
	    const CombSet& right);

  /// @brief union を計算する．
  friend
  CombSet
  operator|(const CombSet& left,
	    const CombSet& right);

  /// @brief 集合差を計算する．
  friend
  CombSet
  operator-(const CombSet& left,
	    const CombSet& right);

  /// @brief cofactor0 を計算する．
  CombSet
  cofactor0(VarId var) const;

  /// @brief cofactor1 を計算する．
  CombSet
  cofactor1(VarId var) const;

  /// @brief 内容を出力する．
  void
  print(ostream& s) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 要素数
  ymuint32 mNum;

  // 本体
  vector<ymuint32> mBody;

};


END_NAMESPACE_YM_ZDD

#endif // COMBSET_H
