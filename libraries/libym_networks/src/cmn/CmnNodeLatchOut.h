#ifndef CMNNODELATCHOUT_H
#define CMNNODELATCHOUT_H

/// @file CmnNodeLatchOut.h
/// @brief CmnNodeLatchOut のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "CmnNodeInput.h"


BEGIN_NAMESPACE_YM_NETWORKS_CMN

//////////////////////////////////////////////////////////////////////
/// @class CmnNodeLatchOut CmnNodeLatchOut.h "CmnNodeLatchOut.h"
/// @brief ラッチの出力を表す CmnNode の派生クラス
//////////////////////////////////////////////////////////////////////
class CmnNodeLatchOut :
  public CmnNodeInput
{
  friend class CmnMgrImpl;

protected:

  /// @brief コンストラクタ
  /// @param[in] latch 親のラッチ
  CmnNodeLatchOut(CmnLatch* latch);

  /// @brief デストラクタ
  virtual
  ~CmnNodeLatchOut();


public:

  /// @brief 関連するラッチを返す．
  /// @note ラッチに関連していない場合には NULL を返す．
  virtual
  const CmnLatch*
  latch() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 関連するラッチ
  const CmnLatch* mLatch;

};


//////////////////////////////////////////////////////////////////////
/// @calss CmnNodeLatchOut1 CmnNodeLatchOut.h "CmnNodeLatchOut.h"
/// @brief ラッチの肯定出力を表す CmnNode の派生クラス
//////////////////////////////////////////////////////////////////////
class CmnNodeLatchOut1 :
  public CmnNodeLatchOut
{
  friend class CmnMgrImpl;

private:

  /// @brief コンストラクタ
  /// @param[in] latch 親のラッチ
  CmnNodeLatchOut1(CmnLatch* latch);

  /// @brief デストラクタ
  virtual
  ~CmnNodeLatchOut1();


public:

  /// @brief 入力ノードのサブタイプを得る．
  virtual
  tInputType
  input_type() const;

};


//////////////////////////////////////////////////////////////////////
/// @calss CmnNodeLatchOut2 CmnNodeLatchOut.h "CmnNodeLatchOut.h"
/// @brief ラッチの肯定出力を表す CmnNode の派生クラス
//////////////////////////////////////////////////////////////////////
class CmnNodeLatchOut2 :
  public CmnNodeLatchOut
{
  friend class CmnMgrImpl;

private:

  /// @brief コンストラクタ
  /// @param[in] latch 親のラッチ
  CmnNodeLatchOut2(CmnLatch* latch);

  /// @brief デストラクタ
  virtual
  ~CmnNodeLatchOut2();


public:

  /// @brief 入力ノードのサブタイプを得る．
  virtual
  tInputType
  input_type() const;

};

END_NAMESPACE_YM_NETWORKS_CMN

#endif // CMNNODELATCHOUT_H
