#ifndef YM_UTILS_MULTIGENBASE_H
#define YM_UTILS_MULTIGENBASE_H

/// @file ym_utils/MultiGenBase.h
/// @brief 組み合わせ生成器と順列生成器のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ymtools.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class MultiGenBase MultiGenBase.h "ym_utils/MultiGenBase.h"
/// @ingroup GeneratorGroup
/// @brief MultiCombiGen/MultiPermGen に共通な属性を表す基底クラス
//////////////////////////////////////////////////////////////////////
class MultiGenBase
{
public:

public:

  /// @brief コンストラクタ
  /// @param[in] nk_array 全要素数 n と選択する要素数 k のベクタ
  MultiGenBase(const vector<pair<ymuint, ymuint> >& nk_array);

  /// @brief コピーコンストラクタ
  /// @param[in] src コピー元のオブジェクト
  MultiGenBase(const MultiGenBase& src);

  /// @brief デストラクタ
  ~MultiGenBase();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief グループ数の取得
  /// @return グループ数
  ymuint
  group_num() const;

  /// @brief 全要素数の取得
  /// @param[in] grp グループ番号
  /// @return grp 番目のグループの全要素数
  ymuint
  n(ymuint grp) const;

  /// @brief 選択する要素数の取得
  /// @param[in] grp グループ番号
  /// @return grp 番目のグループの選択する要素数
  ymuint
  k(ymuint grp) const;

  /// @brief 初期化
  void
  init();

  /// @brief 要素の取得
  /// @param[in] grp グループ番号
  /// @param[in] pos 要素の位置
  /// @return grp 番目のグループの pos 番目の要素を取り出す．
  ymuint
  operator()(ymuint grp,
	     ymuint pos) const;

  /// @brief 末尾のチェック
  /// @return 末尾の時に true を返す．
  bool
  is_end() const;


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスから用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief コピーする．
  /// @param[in] src コピー元のオブジェクト
  void
  copy(const MultiGenBase& src);

  /// @brief 要素配列の初期化
  /// @param[in] grp グループ番号
  /// @note grp 番目のグループの要素配列を初期化する．
  void
  init_group(ymuint grp);

  /// @brief 要素配列の取得
  /// @param[in] grp グループ番号
  /// @return grp 番目のグループの要素配列
  vector<ymuint>&
  elem(ymuint grp);

  /// @brief grp 番目のグループが終了状態の時 true を返す．
  bool
  is_end_sub(ymuint grp) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 各グループごとの全要素数を選択する要素数を入れる配列
  vector<pair<ymuint, ymuint> > mNkArray;

  // 現在の要素(二重の配列なので少しめんどくさい)
  vector<vector<ymuint> > mElemArray;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// グループ数を得る．
inline
ymuint
MultiGenBase::group_num() const
{
  return mNkArray.size();
}

// grp 番目のグループの全要素数を得る．
inline
ymuint
MultiGenBase::n(ymuint grp) const
{
  return mNkArray[grp].first;
}

// grp 番目のグループの選択する要素数を得る．
inline
ymuint
MultiGenBase::k(ymuint grp) const
{
  return mNkArray[grp].second;
}

// grp 番目のグループの pos 番目の要素を取り出す．
inline
ymuint
MultiGenBase::operator()(ymuint grp,
			 ymuint pos) const
{
  return mElemArray[grp][pos];
}

// 末尾の時に true を返す．
inline
bool
MultiGenBase::is_end() const
{
  return is_end_sub(0);
}

// grp 番目のグループの要素配列を得る．
inline
vector<ymuint>&
MultiGenBase::elem(ymuint g)
{
  return mElemArray[g];
}

// grp 番目のグループが終了状態の時 true を返す．
inline
bool
MultiGenBase::is_end_sub(ymuint grp) const
{
  return mElemArray[grp][0] == n(grp);
}

END_NAMESPACE_YM

#endif // YM_UTILS_MULTIGENBASE_H
