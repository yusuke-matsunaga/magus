#ifndef YM_SMTLIBV2_SMTIDENTIFIER_H
#define YM_SMTLIBV2_SMTIDENTIFIER_H

/// @file ym_smtlibv2/SmtIdentifier.h
/// @brief SmtIdentifier のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_smtlibv2/smtlibv2_nsdef.h"
#include "ym_utils/ShString.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

//////////////////////////////////////////////////////////////////////
/// @class SmtIdentifier SmtIdentifier.h "ym_smtlibv2/SmtIdentifier.h"
/// @brief 識別子を表すクラス
//////////////////////////////////////////////////////////////////////
class SmtIdentifier
{
public:

  /// @brief 単純な形のコンストラクタ
  /// @param[in] name 名前
  explicit
  SmtIdentifier(const ShString& name);

  /// @brief インデックス付きのコンストラクタ
  /// @param[in] name 名前
  /// @param[in] index_list インデックスのリスト
  SmtIdentifier(const ShString& name,
		const vector<ymint32>& index_list);

  /// @brief デストラクタ
  ~SmtIdentifier();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前を返す．
  ShString
  name() const;

  /// @brief インデックスリストの要素数を返す．
  /// @note インデックスリストを持たない場合は 0 を返す．
  ymuint
  index_size() const;

  /// @brief インデックスを返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < index_size() )
  ymint32
  index(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 名前
  ShString mName;

  // インデックスのリスト
  vector<ymint32> mIndexList;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 単純な形のコンストラクタ
// @param[in] name 名前
inline
SmtIdentifier::SmtIdentifier(const ShString& name) :
  mName(name)
{
}

// @brief インデックス付きのコンストラクタ
// @param[in] name 名前
// @param[in] index_list インデックスのリスト
inline
SmtIdentifier::SmtIdentifier(const ShString& name,
			     const vector<ymint32>& index_list) :
  mName(name),
  mIndexList(index_list)
{
}

// @brief デストラクタ
inline
SmtIdentifier::~SmtIdentifier()
{
}

// @brief 名前を返す．
inline
ShString
SmtIdentifier::name() const
{
  return mName;
}

// @brief インデックスリストの要素数を返す．
// @note インデックスリストを持たない場合は 0 を返す．
inline
ymuint
SmtIdentifier::index_size() const
{
  return mIndexList.size();
}

// @brief インデックスを返す．
// @param[in] pos 位置番号 ( 0 <= pos < index_size() )
inline
ymint32
SmtIdentifier::index(ymuint pos) const
{
  assert_cond( pos < index_size(), __FILE__, __LINE__);
  return mIndexList[pos];
}

END_NAMESPACE_YM_SMTLIBV2

#endif // YM_SMTLIBV2_SMTIDENTIFIER_H
