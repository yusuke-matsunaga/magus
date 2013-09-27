#ifndef EUFVARMGR_H
#define EUFVARMGR_H

/// @file EufVarMgr.h
/// @brief EufVarMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "llvmeq_nsdef.h"


BEGIN_NAMESPACE_YM_LLVMEQ

//////////////////////////////////////////////////////////////////////
/// @class EufVarMgr EufVarMgr.h "EufVarMgr.h"
/// @brief EufVar を管理するクラス
//////////////////////////////////////////////////////////////////////
class EufVarMgr
{
public:

  /// @brief コンストラクタ
  EufVarMgr();

  /// @brief デストラクタ
  ~EufVarMgr();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 変数を生成する．
  /// @param[in] name 変数名
  /// @note 同じ名前の変数が存在したらそれを返す．
  EufVar*
  new_variable(const string& name);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ハッシュ中の要素数
  ymuint32 mNum;

  // ハッシュテーブルのサイズ
  ymuint32 mTableSize;

  // ハッシュテーブル
  EufVar** mTable;

  // ハッシュを拡張する目安
  ymuint32 mNextLimit;

};

END_NAMESPACE_YM_LLVMEQ

#endif // EUFVARMGR_H
