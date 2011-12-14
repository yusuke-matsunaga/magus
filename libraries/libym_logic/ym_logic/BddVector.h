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
  /// @name 要素に対する演算
  /// @{

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

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name ノード数の計数などの関数
  /// @{

  /// @brief BDD のリストが使っているノード数を数える．
  ymuint64
  node_count() const;

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name サポート関係の関数
  /// @{

  /// @brief サポート変数集合の計算 (VarVector)
  /// @param[out] support サポート変数集合を格納するベクタ
  /// @return サポートの要素数
  ymuint
  support(VarVector& sup) const;

  /// @brief サポート変数集合の計算 (VarList)
  /// @param[out] support サポート変数集合を格納するベクタ
  /// @return サポートの要素数
  ymuint
  support(VarList& sup) const;

  /// @brief サポート変数集合の計算 (BddVarSet)
  /// @return サポート変数集合
  BddVarSet
  support() const;

  /// @brief サポート変数集合の要素数の計算
  /// @return サポート変数集合の要素数
  ymuint
  support_size() const;

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name ノード数の計数などの関数
  /// @{

  /// @brief 内容を書き出す
  /// @param[in] s 出力ストリーム
  /// @return ノード数を返す．
  ymuint64
  print(ostream& s) const;

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name ノード数の計数などの関数
  /// @{

  /// @brief BDD の内容をダンプする．
  /// @param[in] s 出力ストリーム
  void
  dump(BinO& s) const;

  /// @brief バイナリファイルに保存されたBDDを読み込む．
  /// @param[in] s 入力ストリーム
  void
  restore(BinI& s);

  /// @}
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // BddMgr
  BddMgr& mMgr;

};

END_NAMESPACE_YM_BDD

#endif // YM_LOGIC_BDDVECTOR_H
