#ifndef FRAIGNODE_H
#define FRAIGNODE_H

/// @file FraigNode.h
/// @brief FraigNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018, 2022 Yusuke Matsunaga
/// All rights reserved.

#include "fraig_nsdef.h"
#include "FraigHandle.h"


BEGIN_NAMESPACE_FRAIG

//////////////////////////////////////////////////////////////////////
/// @class FraigNode FraigNode.h "FraigNode.h"
/// @brief Fraig のノードを表すクラス
//////////////////////////////////////////////////////////////////////
class FraigNode
{
  friend class FraigMgrImpl;
  friend class PatHash;

public:

  /// @brief コンストラクタ
  FraigNode();

  /// @brief AND用のコンストラクタ
  FraigNode(
    FraigHandle handle1, ///< [in] 入力1のハンドル
    FraigHandle handle2  ///< [in] 入力2のハンドル
  );

  /// @brief デストラクタ
  ~FraigNode();


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を設定する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力番号をセットする．
  void
  set_input(
    SizeType id
  )
  {
    mFlags[BIT_I] = true;
    mFanins[0] = reinterpret_cast<FraigNode*>(id);
  }

  /// @brief ファンインをセットする．
  void
  set_fanin(
    FraigHandle handle1, ///< [in] 入力1のハンドル
    FraigHandle handle2  ///< [in] 入力2のハンドル
  );


public:
  //////////////////////////////////////////////////////////////////////
  // 変数番号に関するアクセス関数
  //////////////////////////////////////////////////////////////////////

  /// @brief CNF 上の変数番号を返す．
  SatLiteral
  varid() const
  {
    return mVarId;
  }


public:
  //////////////////////////////////////////////////////////////////////
  // 入力ノードの時のアクセス関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力ノードの時に true を返す．
  bool
  is_input() const
  {
    return mFlags[BIT_I];
  }

  /// @brief 入力番号を返す．
  SizeType
  input_id() const
  {
    return reinterpret_cast<ympuint>(mFanins[0]);
  }


public:
  //////////////////////////////////////////////////////////////////////
  // AND ノードの時のアクセス関数
  //////////////////////////////////////////////////////////////////////

  /// @brief AND の時に true を返す．
  bool
  is_and() const
  {
    return !is_input();
  }

  /// @brief ファンインを得る．
  FraigNode*
  fanin(
    SizeType pos ///< [in] 位置 ( 0 or 1 )
  ) const
  {
    // 安全のため pos の値を補正しておく．
    pos &= 1;
    return mFanins[pos];
  }

  /// @brief 1番めのファンインを得る．
  FraigNode*
  fanin0() const
  {
    return mFanins[0];
  }

  /// @brief 2番めのファンインを得る．
  FraigNode*
  fanin1() const
  {
    return mFanins[1];
  }

  /// @brief ファンインの極性を得る．
  bool
  fanin_inv(
    SizeType pos ///< [in] 位置 ( 0 or 1 )
  ) const
  {
    // 安全のため pos の値を補正しておく．
    pos &= 1;
    return mFlags[BIT_INV0 + pos];
  }

  /// @brief 1番めのファンインの極性を得る．
  bool
  fanin0_inv() const
  {
    return mFlags[BIT_INV0];
  }

  /// @brief 2番めのファンインの極性を得る．
  bool
  fanin1_inv() const
  {
    return mFlags[BIT_INV1];
  }

  /// @brief ファンインのハンドルを得る．
  FraigHandle
  fanin_handle(
    SizeType pos ///< [in] 位置 ( 0 or 1 )
  ) const
  {
    return FraigHandle(fanin(pos), fanin_inv(pos));
  }

  /// @brief 1番め初のファンインのハンドルを得る．
  FraigHandle
  fanin0_handle() const
  {
    return FraigHandle(fanin0(), fanin0_inv());
  }

  /// @brief 2番めのファンインのハンドルを得る．
  FraigHandle
  fanin1_handle() const
  {
    return FraigHandle(fanin1(), fanin1_inv());
  }


public:
  //////////////////////////////////////////////////////////////////////
  // シミュレーション・パタンに関するアクセス関数
  //////////////////////////////////////////////////////////////////////

  /// @brief パタンを初期化する．
  void
  resize_pat(
    SizeType size ///< [in] サイズ
  );

  /// @brief パタンをセットする．
  void
  set_pat(
    SizeType start,             ///< [in] 開始位置
    SizeType end,               ///< [in] 終了位置
    const vector<ymuint64>& pat ///< [in] パタンのベクタ
  );

  /// @brief パタンを計算する．
  ///
  /// ANDノード用
  void
  calc_pat(
    SizeType start, ///< [in] 開始位置
    SizeType end    ///< [in] 終了位置
  );

  /// @brief パタンの先頭を返す．
  const ymuint64*
  pat() const
  {
    return mPat;
  }

  /// @brief パタンの末尾を返す．
  const ymuint64*
  pat_end() const
  {
    return mPat + mPatUsed;
  }

  /// @brief 0 の値を取るとき true を返す．
  bool
  check_0mark() const
  {
    return mFlags[BIT_0];
  }

  /// @brief 1 の値を取るとき true を返す．
  bool
  check_1mark() const
  {
    return mFlags[BIT_1];
  }

  /// @brief パタンのハッシュ値を返す．
  SizeType
  pat_hash() const
  {
    return mHash;
  }

  /// @brief ハッシュ値の極性を返す．
  bool
  pat_hash_inv() const
  {
    return mFlags[BIT_H];
  }


public:
  //////////////////////////////////////////////////////////////////////
  // 等価グループに関するアクセス関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 代表ノードを返す．
  FraigNode*
  rep_node() const
  {
    return mRepNode;
  }

  /// @brief 代表ノードに対する極性を返す．
  bool
  rep_inv() const
  {
    return mFlags[BIT_R];
  }

  /// @brief 代表ハンドルを返す．
  FraigHandle
  rep_handle() const
  {
    return FraigHandle(mRepNode, rep_inv());
  }

  /// @brief 等価候補の代表をセットする．
  void
  set_rep(
    FraigNode* rep_node, ///< [in] 代表ノード
    bool inv             ///< [in] 反転フラグ
  )
  {
    mRepNode = rep_node;
    if ( inv ) {
      set_rep_inv();
    }
  }

  /// @brief 極性反転の印をつける．
  void
  set_rep_inv()
  {
    mFlags[BIT_R] = true;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // 下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ハッシュ値を計算する．
  void
  calc_hash(
    SizeType start, ///< [in] 開始位置
    SizeType end    ///< [in] 終了位置
  );

  /// @brief 0 の値を取ったことを記録する．
  void
  set_0mark()
  {
    mFlags[BIT_0] = true;
  }

  /// @brief 1 の値を取ったことを記録する．
  void
  set_1mark()
  {
    mFlags[BIT_1] = true;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // CNF 上の変数番号
  SatLiteral mVarId;

  // ファンインのノード
  FraigNode* mFanins[2]{nullptr, nullptr};

  // 0/1マーク，極性などの情報をパックしたもの
  bitset<7> mFlags{0};

  // シミュレーションパタン
  ymuint64* mPat{nullptr};

  // mPat のハッシュ値
  SizeType mHash{0UL};

  // シグネチャハッシュ用のリンクポインタ
  FraigNode* mLink2{nullptr};

  // 代表ノード情報
  FraigNode* mRepNode{nullptr};


private:
  //////////////////////////////////////////////////////////////////////
  // クラスメンバ
  //////////////////////////////////////////////////////////////////////

  // ハッシュ用の素数配列
  static
  ymuint64 mPrimes[];

  // 実際のパタンサイズ
  static
  SizeType mPatSize;

  // 使用中のパタンサイズ
  static
  SizeType mPatUsed;


private:
  //////////////////////////////////////////////////////////////////////
  // mFlags で用いるシフト定数
  //////////////////////////////////////////////////////////////////////

  // 入力フラグ
  static
  const int BIT_I  = 0;

  // ファンイン0 の極性
  static
  const int BIT_INV0 = 1;

  // ファンイン1 の極性
  static
  const int BIT_INV1 = 2;

  // 0 になったことがあるかどうか
  static
  const int BIT_0  = 3;

  // 1 になったことがあるかどうか
  static
  const int BIT_1  = 4;

  // ハッシュパタンの極性
  static
  const int BIT_H  = 5;

  // 等価グループ中の極性
  static
  const int BIT_R  = 6;

};

END_NAMESPACE_FRAIG

#endif // FRAIGNODE_H
