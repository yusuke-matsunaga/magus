#ifndef MSOP_H
#define MSOP_H

/// @file MsOp.h
/// @brief MsOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ZddOp.h"
#include "CompTbl.h"


BEGIN_NAMESPACE_YM_ZDD

class ZddBinOp;

//////////////////////////////////////////////////////////////////////
/// @class MsOp MsOp.h "MsOp.h"
/// @brief minimal setsを求めるクラス
//////////////////////////////////////////////////////////////////////
class MsOp :
  public ZddOp
{
public:

  /// @brief コンストラクタ
  /// @param[in] mgr マネージャ
  /// @param[in] diff_op 集合差演算オブジェクト
  MsOp(ZddMgrImpl* mgr,
       ZddBinOp* diff_op);

  /// @brief デストラクタ
  virtual
  ~MsOp();


public:
  //////////////////////////////////////////////////////////////////////
  // メインの関数
  //////////////////////////////////////////////////////////////////////

  /// @brief minimum set 演算を行う関数
  /// @param[in] left オペランド
  virtual
  ZddEdge
  apply(ZddEdge left);

  /// @brief 次の GC で回収されるノードに関連した情報を削除する．
  virtual
  void
  sweep();


private:
  //////////////////////////////////////////////////////////////////////
  // 下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 実際に演算を行う関数
  ZddEdge
  apply_step(ZddEdge e);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 演算結果テーブル
  CompTbl1 mCompTbl;

  // 集合差演算
  ZddBinOp* mDiffOp;

};

END_NAMESPACE_YM_ZDD

#endif // MSOP_H
