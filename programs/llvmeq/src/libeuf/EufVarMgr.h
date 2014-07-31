#ifndef EUFVARMGR_H
#define EUFVARMGR_H

/// @file EufVarMgr.h
/// @brief EufVarMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "llvmeq_nsdef.h"
#include "YmUtils/UnitAlloc.h"
#include "YmLogic/VarId.h"


BEGIN_NAMESPACE_YM_LLVMEQ

class EufVar;

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

  /// @brief 変数を探す．
  /// @param[in] name 変数名
  /// @note 見つからなければ NULL を返す．
  EufNode*
  find(const string& name) const;

  /// @brief 変数を生成する．
  /// @param[in] id ID番号
  /// @param[in] vid SatSolver 用の変数番号
  /// @param[in] name 変数名
  /// @param[in] bool_flag ブール変数の時 true にするフラグ
  EufNode*
  new_variable(ymuint id,
	       VarId vid,
	       const string& name,
	       bool bool_flag);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ハッシュテーブルを拡大する．
  void
  expand(ymuint req_size);

  /// @brief 名前からハッシュ値を計算する．
  ymuint
  hash(const string& name) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // EufVar 用のアロケータ
  UnitAlloc mAlloc;

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
