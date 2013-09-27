#ifndef EUFEQMGR_H
#define EUFEQMGR_H

/// @file EufEqMgr.h
/// @brief EufEqMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "llvmeq_nsdef.h"


BEGIN_NAMESPACE_YM_LLVMEQ

//////////////////////////////////////////////////////////////////////
/// @class EufEqMgr EufEqMgr.h "EufEqMgr.h"
/// @brief EufEq を管理するクラス
//////////////////////////////////////////////////////////////////////
class EufEqMgr
{
public:

  /// @brief コンストラクタ
  EufEqMgr();

  /// @brief デストラクタ
  ~EufEqMgr();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 等価式を生成する．
  /// @param[in] left, right 左辺式，右辺式
  /// @note 同じ内容の等価式が存在したらそれを返す．
  EufEq*
  new_equality(EufNode* left,
	       EufNode* right);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ハッシュ中の要素数
  ymuint32 mNum;

  // ハッシュテーブルのサイズ
  ymuint32 mTableSize;

  // ハッシュテーブル
  EufEq** mTable;

  // ハッシュを拡張する目安
  ymuint32 mNextLimit;

};

END_NAMESPACE_YM_LLVMEQ

#endif // EUFEQMGR_H
