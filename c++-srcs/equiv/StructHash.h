#ifndef STRUCTHASH_H
#define STRUCTHASH_H

/// @file StructHash.h
/// @brief StructHash のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018, 2022 Yusuke Matsunaga
/// All rights reserved.

#include "FraigHash.h"
#include "FraigHandle.h"


BEGIN_NAMESPACE_FRAIG

class FraigNode;

//////////////////////////////////////////////////////////////////////
/// @class StructHash StructHash.h "StructHash.h"
/// @brief FraigNode の構造ハッシュ表を実装するためのクラス
//////////////////////////////////////////////////////////////////////
class StructHash :
  public FraigHash
{
public:

  /// @brief コンストラクタ
  StructHash();

  /// @brief デストラクタ
  ~StructHash();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 同じ構造を持つノードを探す．
  /// @retval true ノードが見つかった．
  /// @retval false ノードが見つからなかった．
  bool
  find(
    FraigHandle handle1, ///< [in] 入力1のハンドル
    FraigHandle handle2, ///< [in] 入力2のハンドル
    FraigHandle& ans     ///< [out] 答のハンドルを格納する変数
  );

  /// @brief ノードを追加する．
  void
  add(
    FraigNode* node ///< [in] 追加するノード
  );

  /// @brief 内容を空にする．
  ///
  /// テーブルサイズはそのまま
  void
  clear();


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief FraigNode のハッシュ関数
  static
  SizeType
  hash_func(
    FraigHandle handle1, ///< [in] 入力1のハンドル
    FraigHandle handle2  ///< [in] 入力2のハンドル
  )
  {
    return handle1.hash() + handle2.hash() * 13;
  }

};

END_NAMESPACE_FRAIG

#endif // STRUCTHASH_H
