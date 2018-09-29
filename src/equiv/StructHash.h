#ifndef STRUCTHASH_H
#define STRUCTHASH_H

/// @file StructHash.h
/// @brief StructHash のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018 Yusuke Matsunaga
/// All rights reserved.


#include "FraigHash.h"
#include "FraigHandle.h"


BEGIN_NAMESPACE_EQUIV

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
  /// @param[in] handle1, handle2 ファンインのハンドル
  /// @param[out] ans 答のハンドル
  /// @retval true ノードが見つかった．
  /// @retval false ノードが見つからなかった．
  bool
  find(FraigHandle handle1,
       FraigHandle handle2,
       FraigHandle& ans);

  /// @brief ノードを追加する．
  /// @param[in] node 追加するノード
  void
  add(FraigNode* node);

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
  hash_func(FraigHandle handle1,
	    FraigHandle handle2);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// FraigNode のハッシュ関数
inline
SizeType
StructHash::hash_func(FraigHandle handle1,
		      FraigHandle handle2)
{
  return handle1.mPackedData + handle2.mPackedData;
}

END_NAMESPACE_EQUIV

#endif // STRUCTHASH_H
