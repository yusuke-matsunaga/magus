#ifndef SUPOP_H
#define SUPOP_H

/// @file SupOp.h
/// @brief SupOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "BddOp.h"


BEGIN_NAMESPACE_YM_BDD

//////////////////////////////////////////////////////////////////////
/// @class SupOp SupOp.h "SupOp.h"
/// @brief サポートを求める演算を行うクラス
//////////////////////////////////////////////////////////////////////
class SupOp :
  public BddOp
{
public:

  /// @brief コンストラクタ
  /// @param[in] mgr マネージャ
  SupOp(BddMgrImpl* mgr);

  /// @brief デストラクタ
  virtual
  ~SupOp();


public:
  //////////////////////////////////////////////////////////////////////
  // メインの関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 演算を行う関数
  /// @param[in] edge_list 根の枝のリスト
  /// @return サポート数を返す．
  ymuint
  apply(const vector<BddEdge>& edge_list);

  /// @brief サポート変数のリストを得る．
  /// @param[out] var_list 結果を格納する変数
  ymuint
  to_list(vector<VarId>& var_list);

  /// @brief サポート変数のリストを得る．
  /// @param[out] var_list 結果を格納する変数
  ymuint
  to_list(list<VarId>& var_list);

  /// @brief サポート変数のリストを BDD の形で得る．
  BddEdge
  to_bdd();

  /// @brief SCC を求める．
  BddEdge
  scc(BddEdge e);

  /// @brief 次の GC で回収されるノードに関連した情報を削除する．
  virtual
  void
  sweep();


private:
  //////////////////////////////////////////////////////////////////////
  // 下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief apply() の下請け関数
  void
  count_sub(BddEdge e);

  /// @brief scc() の下請け関数
  void
  scc_step(BddEdge e,
	   ymuint spos);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // サポート変数に印をつけるための作業領域
  vector<ymuint8> mMarks;

  // サポート変数(のレベル)のリスト
  vector<ymuint32> mSupList;

};

END_NAMESPACE_YM_BDD

#endif // SUPOP_H
