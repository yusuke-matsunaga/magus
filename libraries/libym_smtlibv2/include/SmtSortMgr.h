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

  /// @brief 初期化する．
  void
  clear();

  /// @brief 名前を登録する．
  /// @param[in] name 名前
  /// @param[in] arg_num 引数の数
  /// @retval true 登録が成功した．
  /// @retval false 登録が失敗した．同名で異なる宣言がすでに登録されている．
  bool
  reg_name(const ShString& name,
	   ymuint arg_num);

  /// @brief 名前から宣言を探す．
  /// @param[in] name 名前
  /// @return 宣言が登録されている場合には引数の数を返す．
  /// @return 登録されていなければ -1 を返す．
  ymint
  find_name(const ShString& name) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 名前をキーにして引数の数を保持するハッシュ表
  hash_map<ShString, ymint32> mHash;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
inline
SmtSortMgr::SmtSortMgr()
{
}

// @brief デストラクタ
inline
SmtSortMgr::~SmtSortMgr()
{
}

// @brief 初期化する．
inline
void
SmtSortMgr::clear()
{
  mHash.clear();
}

// @brief 名前を登録する．
// @param[in] name 名前
// @param[in] arg_num 引数の数
// @retval true 登録が成功した．
// @retval false 登録が失敗した．同名で異なる宣言がすでに登録されている．
inline
bool
SmtSortMgr::reg_name(const ShString& name,
		     ymuint arg_num)
{
  hash_map<ShString, ymint32>::iterator p = mHash.find(name);
  if ( p != mHash.end() ) {
    if ( p->second != arg_num ) {
      return false;
    }
    return true;
  }
  mHash.insert(make_pair(name, arg_num));
}

// @brief 名前から宣言を探す．
// @param[in] name 名前
// @return 宣言が登録されている場合には引数の数を返す．
// @return 登録されていなければ -1 を返す．
inline
ymint
SmtSortMgr::find_name(const ShString& name) const
{
  hash_map<ShString, ymint32>::iterator p = mHash.find(name);
  if ( p != mHash.end() ) {
    return p->second;
  }
  return -1;
}

END_NAMESPACE_YM_SMTLIBV2

#endif // SMTSORTMGR_H
