#ifndef YM_UTILS_MULTISETGENBASE_H
#define YM_UTILS_MULTISETGENBASE_H

/// @file MultiSetGenBase.h
/// @brief MultiSetGenBase のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ymtools.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class MultiSetGenBase MultiSetGenBase.h "MultiSetGenBase.h"
/// @brief MultiSetCombiGen と MultiSetPermGen の親クラス
//////////////////////////////////////////////////////////////////////
class MultiSetGenBase
{
public:

  /// @brief コンストラクタ
  /// @param[in] num_array 各要素の重複度を納めた配列
  /// @param[in] k 選び出す要素数
  MultiSetGenBase(const vector<ymuint>& num_array,
		  ymuint k);

  /// @brief デストラクタ
  ~MultiSetGenBase();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 要素の種類の数を得る．
  ymuint
  group_num() const;

  /// @brief 各要素の重複度を得る．
  /// @param[in] grp ( 0 <= grp < group_num() )
  ymuint
  n(ymuint grp) const;

  /// @brief 選択する要素数を返す．
  ymuint
  k() const;

  /// @brief 初期化する．
  void
  init();

  /// @brief 要素の取得
  /// @param[in] pos 取り出す要素の位置
  ymuint
  operator()(ymuint pos) const;


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスから用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容をコピーする関数
  /// @param[in] src コピー元のオブジェクト
  void
  copy(const MultiSetGenBase& src);

  /// @brief 要素の参照の取得
  /// @param[in] pos 取り出す要素の位置 (最初の位置は 0)
  /// @return pos 番目の要素への参照
  ymuint&
  elem(ymuint pos);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 要素の重複度
  vector<ymuint32> mNumArray;

  // 選択する要素数
  ymuint32 mK;

  // 現在の要素
  vector<ymuint32> mElem;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] num_array 各要素の重複度を納めた配列
// @param[in] k 選び出す要素数
inline
MultiSetGenBase::MultiSetGenBase(const vector<ymuint>& num_array,
				 ymuint k) :
  mNumArray(num_array),
  mK(k),
  mElem(k)
{
  init();
}

// @brief デストラクタ
inline
MultiSetGenBase::~MultiSetGenBase()
{
}

// @brief 要素の種類の数を得る．
inline
ymuint
MultiSetGenBase::group_num() const
{
  return mNumArray.size();
}

// @brief 各要素の重複度を得る．
// @param[in] grp ( 0 <= grp < group_num() )
inline
ymuint
MultiSetGenBase::n(ymuint grp) const
{
  assert_cond( grp < group_num(), __FILE__, __LINE__);
  return mNumArray[grp];
}

// @brief 選択する要素数を返す．
inline
ymuint
MultiSetGenBase::k() const
{
  return mK;
}

// @brief 初期化する．
inline
void
MultiSetGenBase::init()
{
  ymuint pos = 0;
  ymuint count = 0;
  for (ymuint i = 0; i < mK; ++ i) {
    if ( count >= mNumArray[pos] ) {
      ++ pos;
      count = 0;
    }
    assert_cond( count < mNumArray[pos], __FILE__, __LINE__);
    mElem[i] = pos;
    ++ count;
  }
}

// @brief 要素の取得
// @param[in] pos 取り出す要素の位置
inline
ymuint
MultiSetGenBase::operator()(ymuint pos) const
{
  return mElem[pos];
}

// @brief 内容をコピーする関数
// @param[in] src コピー元のオブジェクト
inline
void
MultiSetGenBase::copy(const MultiSetGenBase& src)
{
  mNumArray = src.mNumArray;
  mK = src.mK;
  mElem = src.mElem;
}

// @brief 要素の参照の取得
// @param[in] pos 取り出す要素の位置 (最初の位置は 0)
// @return pos 番目の要素への参照
inline
ymuint&
MultiSetGenBase::elem(ymuint pos)
{
  return mElem[pos];
}

END_NAMESPACE_YM

#endif // YM_UTILS_MULTISETGENBASE_H
