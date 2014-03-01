#ifndef EUFFUNCMGR_H
#define EUFFUNCMGR_H

/// @file EufFuncMgr.h
/// @brief EufFuncMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "llvmeq_nsdef.h"
#include "utils/UnitAlloc.h"


BEGIN_NAMESPACE_YM_LLVMEQ

class EufFunc;

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

  /// @brief 関数を探す
  /// @param[in] name 関数名
  /// @param[in] arg_list 引数のリスト
  /// @note 見つからなければ NULL を返す．
  EufNode*
  find(const string& name,
       const vector<EufNode*>& arg_list) const;

  /// @brief 関数を生成する．
  /// @param[in] id ID番号
  /// @param[in] name 関数名
  /// @param[in] arg_list 引数のリスト
  EufNode*
  new_function(ymuint id,
	       const string& name,
	       const vector<EufNode*>& arg_list);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ハッシュテーブルを拡大する．
  void
  expand(ymuint req_size);

  /// @brief ハッシュ値を計算する．
  /// @param[in] name 関数名
  /// @param[in] arg_list 引数のリスト
  ymuint
  hash(const string& name,
       const vector<EufNode*>& arg_list) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // EufFunc 用のアロケータ
  UnitAlloc mAlloc;

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
