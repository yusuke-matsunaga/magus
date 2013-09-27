#ifndef EUFFUNCMGR_H
#define EUFFUNCMGR_H

/// @file EufFuncMgr.h
/// @brief EufFuncMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "llvmeq_nsdef.h"


BEGIN_NAMESPACE_YM_LLVMEQ

//////////////////////////////////////////////////////////////////////
/// @class EufFuncMgr EufFuncMgr.h "EufFuncMgr.h"
/// @brief EufFunc を管理するクラス
//////////////////////////////////////////////////////////////////////
class EufFuncMgr
{
public:

  /// @brief コンストラクタ
  EufFuncMgr();

  /// @brief デストラクタ
  ~EufFuncMgr();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 関数を生成する．
  /// @param[in] name 関数名
  /// @param[in] arg_list 引数のリスト
  /// @note 同じ関数名で同じ引数のインスタンスがあればそれを返す．
  EufFunc*
  new_function(const string& name,
	       const vector<EufNode*>& arg_list);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ハッシュ中の要素数
  ymuint32 mNum;

  // ハッシュテーブルのサイズ
  ymuint32 mTableSize;

  // ハッシュテーブル
  EufFunc** mTable;

  // ハッシュを拡張する目安
  ymuint32 mNextLimit;

};

END_NAMESPACE_YM_LLVMEQ

#endif // EUFFUNCMGR_H
