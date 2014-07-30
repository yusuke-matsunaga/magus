#ifndef YMYMLOGIC_ZDDLIST_H
#define YMYMLOGIC_ZDDLIST_H

/// @file YmLogic/ZddList.h
/// @brief ZddList のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmLogic/zdd_nsdef.h"
#include "YmLogic/VarId.h"


BEGIN_NAMESPACE_YM_ZDD

//////////////////////////////////////////////////////////////////////
/// @class ZddList ZddList.h "YmLogic/ZddList.h"
/// @brief Zdd のベクタを表すクラス
//////////////////////////////////////////////////////////////////////
class ZddList :
  public vector<Zdd>
{
public:

  /// @brief 空のコンストラクタ
  /// @param[in] mgr ZddMgr
  explicit
  ZddList(ZddMgr& mgr);

  /// @brief デストラクタ
  ~ZddList();


public:
  //////////////////////////////////////////////////////////////////////
  /// @name 要素に対する演算
  /// @{

  /// @brief intersection を求める．
  Zdd
  intersection() const;

  /// @brief union を求める．
  Zdd
  set_union() const;

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
  /// @param[out] sup サポート変数集合を格納するベクタ
  /// @return サポートの要素数
  ymuint
  support(VarVector& sup) const;

  /// @brief サポート変数集合の計算 (VarList)
  /// @param[out] sup サポート変数集合を格納するベクタ
  /// @return サポートの要素数
  ymuint
  support(VarList& sup) const;

  /// @brief サポート変数集合の要素数の計算
  /// @return サポート変数集合の要素数
  ymuint
  support_size() const;

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name 出力関係の関数
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
  /// @name バイナリ入出力の関数
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

  // ZddMgr
  ZddMgr& mMgr;

};

END_NAMESPACE_YM_ZDD

#endif // YMYMLOGIC_ZDDLIST_H
