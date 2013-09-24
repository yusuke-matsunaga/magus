#ifndef YM_LOGIC_ZDDVECTOR_H
#define YM_LOGIC_ZDDVECTOR_H

/// @file ym_logic/ZddVector.h
/// @brief ZddVector のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_logic/zdd_nsdef.h"
#include "ym_logic/VarId.h"


BEGIN_NAMESPACE_YM_ZDD

//////////////////////////////////////////////////////////////////////
/// @class ZddVector ZddVector.h "ym_logic/ZddVector.h"
/// @brief Zdd のベクタを表すクラス
//////////////////////////////////////////////////////////////////////
class ZddVector :
  public vector<Zdd>
{
public:

  /// @brief 空のコンストラクタ
  /// @param[in] mgr ZddMgr
  explicit
  ZddVector(ZddMgr& mgr);

  /// @brief 要素数を指定したコンストラクタ
  /// @param[in] mgr ZddMgr
  /// @param[in] n 要素数
  ZddVector(ZddMgr& mgr,
	    ymuint n);

  /// @brief デストラクタ
  ~ZddVector();


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
  /// @param[out] support サポート変数集合を格納するベクタ
  /// @return サポートの要素数
  ymuint
  support(VarVector& sup) const;

  /// @brief サポート変数集合の計算 (VarList)
  /// @param[out] support サポート変数集合を格納するベクタ
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

#endif // YM_LOGIC_ZDDVECTOR_H
