#ifndef SMTSORTMGR_H
#define SMTSORTMGR_H

/// @file SmtSortMgr.h
/// @brief SmtSortMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_smtlibv2/smtlibv2_nsdef.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

//////////////////////////////////////////////////////////////////////
/// @class SmtSortMgr SmtSortMgr.h "SmtSortMgr.h"
/// @brief SmtSort を管理するクラス
//////////////////////////////////////////////////////////////////////
class SmtSortMgr
{
public:

  /// @brief コンストラクタ
  SmtSortMgr();

  /// @brief デストラクタ
  ~SmtSortMgr();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 型名を登録する．
  /// @param[in] name 型名
  /// @param[in] arg_num 引数の数
  /// @retval true 登録が成功した．
  /// @retval false 登録が失敗した．同名で異なる宣言がすでに登録されている．
  bool
  reg_sort(const SmtId* name,
	   ymuint arg_num);

  /// @brief SmtSort に変換する．
  /// @param[in] name 型名
  /// @param[in] elem_list 部品の型のリスト
  /// @return 登録されていなければ NULL を返す．
  const SmtSort*
  new_sort(const SmtId* name,
	   const vector<const SmtSort*>& elem_list);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 名前をキーにして引数の数を保持するハッシュ表
  hash_map<ymuint32, ymint32> mHash;

  // 登録されている宣言の数
  ymuint32 mNum;

  // ハッシュ表のサイズ
  ymuint32 mTableSize;

  // ハッシュ表
  SmtSort* mHashTable;

  // ハッシュ表を拡大する目安
  ymuint32 mNextLimit;

};

END_NAMESPACE_YM_SMTLIBV2

#endif // SMTSORTMGR_H
