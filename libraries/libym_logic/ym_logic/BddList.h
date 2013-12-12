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
  explicit
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
  /// @param[out] sup サポート変数集合を格納するベクタ
  /// @return サポートの要素数
  ymuint
  support(VarVector& sup) const;

  /// @brief サポート変数集合の計算 (VarList)
  /// @param[out] sup サポート変数集合を格納するベクタ
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
  dump(ODO& s) const;

  /// @brief バイナリファイルに保存されたBDDを読み込む．
  /// @param[in] s 入力ストリーム
  void
  restore(IDO& s);

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

#endif // YM_LOGIC_BDDLIST_H
