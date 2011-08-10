#ifndef CMNNODEDFFIN_H
#define CMNNODEDFFIN_H

/// @file CmnNodeDffIn.h
/// @brief CmnNodeDffIn のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "CmnNodeOutput.h"


BEGIN_NAMESPACE_YM_NETWORKS_CMN

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
  /// @param[in] dff 親の D-FF
  CmnNodeDffIn(CmnDff* dff);

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
  /// @param[in] dff 親の D-FF
  CmnNodeDffData(CmnDff* dff);

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
  /// @param[in] dff 親の D-FF
  CmnNodeDffClock(CmnDff* dff);

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
  /// @param[in] dff 親の D-FF
  CmnNodeDffClear(CmnDff* dff);

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
  /// @param[in] dff 親の D-FF
  CmnNodeDffPreset(CmnDff* dff);

  /// @brief デストラクタ
  virtual
  ~CmnNodeDffPreset();


public:

  /// @brief 出力ノードのサブタイプを得る．
  virtual
  tOutputType
  output_type() const;

};

END_NAMESPACE_YM_NETWORKS_CMN

#endif // CMNNODEDFFIN_H
