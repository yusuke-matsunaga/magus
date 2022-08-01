#ifndef FRAIGHANDLE_H
#define FRAIGHANDLE_H

/// @file FraigHandle.h
/// @brief FraigHandle のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018, 2022 Yusuke Matsunaga
/// All rights reserved.

#include "fraig_nsdef.h"


BEGIN_NAMESPACE_FRAIG

class FraigNode;

//////////////////////////////////////////////////////////////////////
/// @class FraigHandle FraigHandle.h "ym/FraigHandle.h"
/// @brief Fraig の枝を表すクラス
//////////////////////////////////////////////////////////////////////
class FraigHandle
{
public:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ/デストラクタと生成/内容の設定
  //////////////////////////////////////////////////////////////////////

  /// @brief 空のコンストラクタ
  FraigHandle() = default;

  /// @brief 内容を設定したコンストラクタ
  FraigHandle(
    FraigNode* node, ///< [in] ノード
    bool inv         ///< [in] 反転している時に true とするフラグ
  ) : FraigHandle{reinterpret_cast<ympuint>(node) | inv}
  {
  }

  /// @brief 定数０のハンドルを返す．
  static
  FraigHandle
  zero()
  {
    return FraigHandle{nullptr, false};
  }

  /// @brief 定数1のハンドルを返す．
  static
  FraigHandle
  one()
  {
    return FraigHandle{nullptr, true};
  }

  /// @brief デストラクタ
  ~FraigHandle() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 内部の情報を取得するメンバ関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 否定の枝を返す．
  FraigHandle
  operator~() const
  {
    return FraigHandle{mPackedData ^ 1UL};
  }

  /// @brief 極性をかけ合わせる．
  FraigHandle
  operator^(
    bool inv ///< [in] 極性
  ) const
  {
    return FraigHandle{mPackedData ^ static_cast<bool>(inv)};
  }

  /// @brief ノードを得る．
  FraigNode*
  node() const
  {
    return reinterpret_cast<FraigNode*>(mPackedData & ~3UL);
  }

  /// @brief 極性を得る．
  /// @return 反転しているとき true を返す．
  bool
  inv() const
  {
    return static_cast<bool>(mPackedData & 1UL);
  }

  /// @brief 定数0を指しているとき true を返す．
  bool
  is_zero() const
  {
    return mPackedData == 0UL;
  }

  /// @brief 定数1を指しているとき true を返す．
  bool
  is_one() const
  {
    return mPackedData == 1UL;
  }

  /// @brief 定数を指しているとき true を返す．
  ///
  /// = is_zero() || is_one()
  bool
  is_const() const
  {
    return (mPackedData & ~1UL) == 0UL;
  }

  /// @brief 外部入力ノードへのハンドルのとき true を返す．
  bool
  is_input() const;

  /// @brief 外部入力ノードへのハンドルのとき，入力番号を返す．
  ///
  /// is_input() == true の時のみ意味を持つ．
  SizeType
  input_id() const;

  /// @brief ANDノードへのハンドルのとき true を返す．
  bool
  is_and() const;

  /// @brief pos で指示されたファンインのハンドルを得る．
  ///
  /// is_and() == true の時のみ意味を持つ．
  FraigHandle
  fanin_handle(
    SizeType pos ///< [in] 位置 ( 0 or 1 )
  ) const;

  /// @brief fanin0 のハンドルを得る．
  ///
  /// is_and() == true の時のみ意味を持つ．
  FraigHandle
  fanin0_handle() const;

  /// @brief fanin1 のハンドルを得る．
  ///
  /// is_and() == true の時のみ意味を持つ．
  FraigHandle
  fanin1_handle() const;

  /// @brief ハッシュ値を返す．
  SizeType
  hash() const
  {
    return static_cast<SizeType>((mPackedData * mPackedData) >> 20);
  }

  /// @brief 等価比較関数
  bool
  operator==(
    FraigHandle src2 ///< [in] オペランド
  ) const
  {
    return mPackedData == src2.mPackedData;
  }

  /// @brief 非等価比較演算
  bool
  operator!=(
    FraigHandle src2  ///< [in] オペランド2
  )
  {
    return !operator==(src2);
  }


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を直接指定したコンストラクタ
  explicit
  FraigHandle(
    ympuint data ///< [in] 内容
  ) : mPackedData{data}
  {
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // すべてのデータをまとめたもの
  ympuint mPackedData{0UL};

};

/// @relates FraigHandle
/// @brief 内容を出力する関数
ostream&
operator<<(
  ostream& s,     ///< [in] 出力先のストリーム
  FraigHandle src ///< [in] ハンドル
);

END_NAMESPACE_FRAIG

#endif // FRAIGHANDLE_H
