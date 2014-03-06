#ifndef SIMFFR_H
#define SIMFFR_H

/// @file SimFFR.h
/// @brief SimFFR のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012-2014 Yusuke Matsunaga
/// All rights reserved.


#include "fsimx2_nsdef.h"


BEGIN_NAMESPACE_YM_SATPG_FSIMX2

class SimNode;
class FsimFault;

//////////////////////////////////////////////////////////////////////
/// @class SimFFR SimFFR.h "SimFFR.h"
/// @brief Fanout-Free-Region を表すクラス
//////////////////////////////////////////////////////////////////////
class SimFFR
{
public:

  /// @brief コンストラクタ
  SimFFR();

  /// @brief デストラクタ
  ~SimFFR();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 根のノードをセットする．
  void
  set_root(SimNode* root);

  /// @brief undet_list をもとにもどす．
  void
  init_undet_list();

  /// @brief 根のノードを得る．
  SimNode*
  root() const;

  /// @brief この FFR に属する故障のリストを返す．
  vector<FsimFault*>&
  fault_list();

  /// @brief この FFR に属する未検出の故障のリストを返す．
  vector<FsimFault*>&
  undet_list();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 根のノード
  SimNode* mRoot;

  // この FFR に属する故障のリスト
  vector<FsimFault*> mFaultList;

  // 未検出の故障のリスト
  vector<FsimFault*> mUndetList;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
inline
SimFFR::SimFFR()
{
}

// @brief デストラクタ
inline
SimFFR::~SimFFR()
{
}

// @brief 根のノードをセットする．
inline
void
SimFFR::set_root(SimNode* root)
{
  mRoot = root;
}

// @brief undet_list をもとにもどす．
inline
void
SimFFR::init_undet_list()
{
  mUndetList = mFaultList;
}

// @brief 根のノードを得る．
inline
SimNode*
SimFFR::root() const
{
  return mRoot;
}

// @brief この FFR に属する故障のリストを返す．
inline
vector<FsimFault*>&
SimFFR::fault_list()
{
  return mFaultList;
}

// @brief この FFR に属する未検出の故障のリストを返す．
inline
vector<FsimFault*>&
SimFFR::undet_list()
{
  return mUndetList;
}

END_NAMESPACE_YM_SATPG_FSIMX2

#endif // SIMFFR_H
