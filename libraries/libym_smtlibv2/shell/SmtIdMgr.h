#ifndef SMTIDMGR_H
#define SMTIDMGR_H

/// @file SmtIdMgr.h
/// @brief SmtIdMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_smtlibv2/smtlibv2_nsdef.h"
#include "ym_utils/ShString.h"
#include "ym_utils/SimpleAlloc.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

class SmtLibNode;
class SmtIdImpl;

//////////////////////////////////////////////////////////////////////
/// @class SmtIdMgr SmtIdMgr.h "SmtIdMgr.h"
/// @brief SmtId を管理するクラス
//////////////////////////////////////////////////////////////////////
class SmtIdMgr
{
public:

  /// @brief コンストラクタ
  SmtIdMgr();

  /// @brief デストラクタ
  ~SmtIdMgr();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 識別子を返す．
  /// @param[in] name 名前
  /// @param[in] index_list インデックスリスト
  const SmtId*
  new_id(const ShString& name,
	 const vector<ymint32>& index_list);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ハッシュ表を拡大する．
  /// @param[in] req_size 新しいサイズ
  void
  expand_table(ymuint req_size);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // メモリ確保用のオブジェクト
  SimpleAlloc mAlloc;

  // 登録されている要素数
  ymuint32 mNum;

  // ハッシュ表のサイズ
  ymuint32 mTableSize;

  // ハッシュ表
  SmtIdImpl** mHashTable;

  // ハッシュ表を拡大する目安
  ymuint32 mNextLimit;

};

END_NAMESPACE_YM_SMTLIBV2

#endif // SMTIDMGR_H
