#ifndef LIBYM_NETWORKS_CMN_CMNNODEDFFIN_H
#define LIBYM_NETWORKS_CMN_CMNNODEDFFIN_H

/// @file libym_networks/cmn/CmnNodeDffIn.h
/// @brief CmnNodeDffIn のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "CmnNodeOutput.h"


BEGIN_NAMESPACE_YM_CMN

//////////////////////////////////////////////////////////////////////
/// @class CmnNodeDffIn CmnNodeDffIn.h "CmnNodeDffIn.h"
/// @brief D-FF の入力を表す CmnNode の派生クラス
//////////////////////////////////////////////////////////////////////
class CmnNodeDffIn :
  public CmnNodeOutput
{
  friend class CmnMgrImpl;

protected:

  /// @brief コンストラクタ
  CmnNodeDffIn();

  /// @brief デストラクタ
  virtual
  ~CmnNodeDffIn();


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

  // 関連する D-FF
  const CmnDff* mDff;

};


//////////////////////////////////////////////////////////////////////
/// @calss CmnNodeDffData CmnNodeDffIn.h "CmnNodeDffIn.h"
/// @brief D-FF のデータ入力を表す CmnNode の派生クラス
//////////////////////////////////////////////////////////////////////
class CmnNodeDffData :
  public CmnNodeDffIn
{
  friend class CmnMgrImpl;

private:

  /// @brief コンストラクタ
  CmnNodeDffData();

  /// @brief デストラクタ
  virtual
  ~CmnNodeDffData();


public:

  /// @brief 出力ノードのサブタイプを得る．
  virtual
  tOutputType
  output_type() const;

};


//////////////////////////////////////////////////////////////////////
/// @calss CmnNodeDffClock CmnNodeDffIn.h "CmnNodeDffIn.h"
/// @brief D-FF のクロック入力を表す CmnNode の派生クラス
//////////////////////////////////////////////////////////////////////
class CmnNodeDffClock :
  public CmnNodeDffIn
{
  friend class CmnMgrImpl;

private:

  /// @brief コンストラクタ
  CmnNodeDffClock();

  /// @brief デストラクタ
  virtual
  ~CmnNodeDffClock();


public:

  /// @brief 出力ノードのサブタイプを得る．
  virtual
  tOutputType
  output_type() const;

};


//////////////////////////////////////////////////////////////////////
/// @calss CmnNodeDffClear CmnNodeDffIn.h "CmnNodeDffIn.h"
/// @brief D-FF のクリア入力を表す CmnNode の派生クラス
//////////////////////////////////////////////////////////////////////
class CmnNodeDffClear :
  public CmnNodeDffIn
{
  friend class CmnMgrImpl;

private:

  /// @brief コンストラクタ
  CmnNodeDffClear();

  /// @brief デストラクタ
  virtual
  ~CmnNodeDffClear();


public:

  /// @brief 出力ノードのサブタイプを得る．
  virtual
  tOutputType
  output_type() const;

};


//////////////////////////////////////////////////////////////////////
/// @calss CmnNodeDffPreset CmnNodeDffIn.h "CmnNodeDffIn.h"
/// @brief D-FF のプリセット入力を表す CmnNode の派生クラス
//////////////////////////////////////////////////////////////////////
class CmnNodeDffPreset :
  public CmnNodeDffIn
{
  friend class CmnMgrImpl;

private:

  /// @brief コンストラクタ
  CmnNodeDffPreset();

  /// @brief デストラクタ
  virtual
  ~CmnNodeDffPreset();


public:

  /// @brief 出力ノードのサブタイプを得る．
  virtual
  tOutputType
  output_type() const;

};

END_NAMESPACE_YM_CMN

#endif // LIBYM_NETWORKS_CMN_CMNNODEDFFIN_H
