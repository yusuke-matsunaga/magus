#ifndef LIBYM_NETWORKS_CMN_CMNNODEDFFOUT_H
#define LIBYM_NETWORKS_CMN_CMNNODEDFFOUT_H

/// @file libym_networks/cmn/CmnNodeDffOut.h
/// @brief CmnNodeDffOut のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "CmnNodeInput.h"


BEGIN_NAMESPACE_YM_CMN

//////////////////////////////////////////////////////////////////////
/// @class CmnNodeDffOut CmnNodeDffOut.h "CmnNodeDffOut.h"
/// @brief DFF の出力を表す CmnNode の派生クラス
//////////////////////////////////////////////////////////////////////
class CmnNodeDffOut :
  public CmnNodeInput
{
  friend class CmnMgrImpl;

protected:

  /// @brief コンストラクタ
  CmnNodeDffOut();

  /// @brief デストラクタ
  virtual
  ~CmnNodeDffOut();


public:

  /// @brief 関連する D-FF を返す．
  /// @note D-FF に関連していない場合には NULL を返す．
  virtual
  const CmnDff*
  dff() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 関連するDFF
  const CmnDff* mDff;

};


//////////////////////////////////////////////////////////////////////
/// @calss CmnNodeDffOut1 CmnNodeDffOut.h "CmnNodeDffOut.h"
/// @brief DFF の肯定出力を表す CmnNode の派生クラス
//////////////////////////////////////////////////////////////////////
class CmnNodeDffOut1 :
  public CmnNodeDffOut
{
  friend class CmnMgrImpl;

private:

  /// @brief コンストラクタ
  CmnNodeDffOut1();

  /// @brief デストラクタ
  virtual
  ~CmnNodeDffOut1();


public:

  /// @brief 入力ノードのサブタイプを得る．
  virtual
  tInputType
  input_type() const;

};


//////////////////////////////////////////////////////////////////////
/// @calss CmnNodeDffOut2 CmnNodeDffOut.h "CmnNodeDffOut.h"
/// @brief DFF の肯定出力を表す CmnNode の派生クラス
//////////////////////////////////////////////////////////////////////
class CmnNodeDffOut2 :
  public CmnNodeDffOut
{
  friend class CmnMgrImpl;

private:

  /// @brief コンストラクタ
  CmnNodeDffOut2();

  /// @brief デストラクタ
  virtual
  ~CmnNodeDffOut2();


public:

  /// @brief 入力ノードのサブタイプを得る．
  virtual
  tInputType
  input_type() const;

};

END_NAMESPACE_YM_CMN

#endif // LIBYM_NETWORKS_CMN_CMNNODEDFFOUT_H
