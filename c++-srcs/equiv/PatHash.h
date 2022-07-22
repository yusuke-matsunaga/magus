#ifndef PATHASH_H
#define PATHASH_H

/// @file PatHash.h
/// @brief PatHash のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018, 2022 Yusuke Matsunaga
/// All rights reserved.

#include "FraigHash.h"


BEGIN_NAMESPACE_FRAIG

class FraigMgrImpl;
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
  /// @retval true ノードが見つかった．
  /// @retval false ノードが見つからなかった．
  bool
  find(
    FraigNode* node,   ///< [in] 対象のノード
    FraigMgrImpl& mgr, ///< [in] FraigMgr
    FraigHandle& ans   ///< [out] 答のハンドル
  );

  /// @brief ノードを追加する．
  void
  add(
    FraigNode* node ///< [in] 追加するノード
  );

};

END_NAMESPACE_FRAIG

#endif // PATHASH_H
