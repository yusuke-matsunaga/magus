#ifndef SUPOP_H
#define SUPOP_H

/// @file SupOp.h
/// @brief SupOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "Op.h"


BEGIN_NAMESPACE_YM_CNFDD

//////////////////////////////////////////////////////////////////////
/// @class SupOp SupOp.h "SupOp.h"
/// @brief サポートを求める演算を行うクラス
//////////////////////////////////////////////////////////////////////
class SupOp :
  public Op
{
public:

  /// @brief コンストラクタ
  /// @param[in] mgr マネージャ
  SupOp(CNFddMgrImpl& mgr);

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
  apply(const vector<CNFddEdge>& edge_list);

  /// @brief サポート変数のリストを得る．
  /// @param[out] var_list 結果を格納する変数
  ymuint
  to_list(vector<VarId>& var_list);

  /// @brief サポート変数のリストを得る．
  /// @param[out] var_list 結果を格納する変数
  ymuint
  to_list(list<VarId>& var_list);

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
  apply_sub(CNFddEdge e);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // サポート変数に印をつけるための作業領域
  vector<ymuint8> mMarks;

  // サポート変数(のレベル)のリスト
  vector<ymuint32> mSupList;

};

END_NAMESPACE_YM_CNFDD

#endif // SUPOP_H
