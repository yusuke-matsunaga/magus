#ifndef SUPPORTLIST_H
#define SUPPORTLIST_H

/// @file SupportList.h
/// @brief SupportList のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "dg.h"


BEGIN_NAMESPACE_DG

//////////////////////////////////////////////////////////////////////
/// @class SupportList SupportList.h "SupportList.h"
/// @brief サポートリストを表すクラス
//////////////////////////////////////////////////////////////////////
class SupportList
{
public:

  /// @brief コンストラクタ
  SupportList() = default;

  /// @brief デストラクタ
  ~SupportList() = default;

  /// @brief 内容を指定したコンストラクタ(コピー版)
  SupportList(
    const vector<SizeType>& body
  ) : mBody{body}
  {
    sort(mBody.begin(), mBody.end());
  }

  /// @brief 内容を指定したコンストラクタ(ムーブ版)
  SupportList(
    vector<SizeType>&& body
  ) : mBody{std::move(body)}
  {
    sort(mBody.begin(), mBody.end());
  }


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief マージする．
  /// @return 結果のリストを返す．
  SupportList
  operator+(
    const SupportList& right ///< [in] オペランド
  ) const;

  /// @brief 残差を求める．
  /// @return 結果のリストを返す．
  SupportList
  operator-(
    const SupportList& right ///< [in] オペランド
  ) const;

  /// @brief インターセクションを求める．
  /// @return 結果のリストを返す．
  SupportList
  operator&(
    const SupportList& right ///< [in] オペランド
  ) const;

  /// @brief インターセクションが空でないか調べる．
  bool
  operator&&(
    const SupportList& right ///< [in] オペランド
  ) const;

  /// @brief 自身にマージ結果を代入する．
  /// @return 代入後の自身への参照を返す．
  SupportList&
  operator+=(
    const SupportList& right ///< [in] オペランド
  );

  /// @brief 残差を自分自身に代入する．
  /// @return 代入後の自身への参照を返す．
  SupportList&
  operator-=(
    const SupportList& right ///< [in] オペランド
  );

  /// @brief インターセクションを自分自身に代入する．
  /// @return 代入後の自身への参照を返す．
  SupportList&
  operator&=(
    const SupportList& right ///< [in] オペランド
  );

  /// @brief 空の時に true を返す．
  bool
  empty() const
  {
    return mBody.empty();
  }

  /// @brief 要素数を返す．
  SizeType
  size() const
  {
    return mBody.size();
  }

  /// @brief 先頭の要素を返す．
  SizeType
  top() const
  {
    ASSERT_COND( !mBody.empty() );
    return mBody[0];
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 本体
  vector<SizeType> mBody;

};

END_NAMESPACE_DG

#endif // SUPPORTLIST_H
