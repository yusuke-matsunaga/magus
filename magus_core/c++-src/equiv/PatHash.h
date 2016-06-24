﻿#ifndef PATHASH_H
#define PATHASH_H

/// @file PatHash.h
/// @brief PatHash のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018 Yusuke Matsunaga
/// All rights reserved.


#include "FraigHash.h"


BEGIN_NAMESPACE_EQUIV

class FraigMgr;
class FraigHandle;

//////////////////////////////////////////////////////////////////////
/// @class PatHash PatHash.h "PatHash.h"
/// @brief FraigNode のパタンハッシュ表を実装するためのクラス
//////////////////////////////////////////////////////////////////////
class PatHash :
  public FraigHash
{
public:

  /// @brief コンストラクタ
  PatHash();

  /// @brief デストラクタ
  ~PatHash();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 同じパタンを持つノードを探す．
  /// @param[in] node 対象のノード
  /// @param[in] mgr FraigMgr
  /// @param[out] ans 答のハンドル
  /// @retval true ノードが見つかった．
  /// @retval false ノードが見つからなかった．
  bool
  find(FraigNode* node,
       FraigMgr& mgr,
       FraigHandle& ans);

  /// @brief ノードを追加する．
  /// @param[in] node 追加するノード
  void
  add(FraigNode* node);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

END_NAMESPACE_EQUIV

#endif // PATHASH_H
