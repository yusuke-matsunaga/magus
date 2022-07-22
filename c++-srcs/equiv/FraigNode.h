﻿#ifndef FRAIGNODE_H
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
  friend class StructHash;
  friend class PatHash;

public:

  /// @brief コンストラクタ
  FraigNode();

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
    mFlags |= (1U << kSftI);
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
    return static_cast<bool>((mFlags >> kSftI) & 1U);
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
    return static_cast<bool>((mFlags >> (kSftP0 + pos)) & 1U);
  }

  /// @brief 1番めのファンインの極性を得る．
  bool
  fanin0_inv() const
  {
    return static_cast<bool>((mFlags >> kSftP0) & 1U);
  }

  /// @brief 2番めのファンインの極性を得る．
  bool
  fanin1_inv() const
  {
    return static_cast<bool>((mFlags >> kSftP1) & 1U);
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
  )
  {
    ymuint64* dst = mPat + start;
    ymuint64* dst_end = mPat + end;
    ymuint64* src1 = mFanins[0]->mPat + start;
    ymuint64* src2 = mFanins[1]->mPat + start;
    if ( fanin0_inv() ) {
      if ( fanin1_inv() ) {
	for ( ; dst != dst_end; ++ dst, ++src1, ++src2) {
	  *dst = ~(*src1 | *src2);
	}
      }
      else {
	for ( ; dst != dst_end; ++ dst, ++src1, ++src2) {
	  *dst = ~*src1 & *src2;
	}
      }
    }
    else {
      if ( fanin1_inv() ) {
	for ( ; dst != dst_end; ++ dst, ++src1, ++src2) {
	  *dst = *src1 & ~*src2;
	}
      }
      else {
	for ( ; dst != dst_end; ++ dst, ++src1, ++src2) {
	  *dst = *src1 & *src2;
	}
      }
    }
    calc_hash(start, end);
  }

  /// @brief 0 の値を取るとき true を返す．
  bool
  check_0mark() const
  {
    return static_cast<bool>((mFlags >> kSft0) & 1U);
  }

  /// @brief 1 の値を取るとき true を返す．
  bool
  check_1mark() const
  {
    return static_cast<bool>((mFlags >> kSft1) & 1U);
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
    return static_cast<bool>((mFlags >> kSftH) & 1U);
  }


public:
  //////////////////////////////////////////////////////////////////////
  // 等価グループに関するアクセス関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 削除済みのとき true を返す．
  bool
  check_dmark() const
  {
    return static_cast<bool>((mFlags >> kSftD) & 1U);
  }

  /// @brief 要素数が2以上の等価候補グループの代表なら true を返す．
  bool
  check_rep() const
  {
    return mRepNode == this && mEqLink != nullptr;
  }

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
    return static_cast<bool>((mFlags >> kSftP) & 1U);
  }

  /// @brief 代表ハンドルを返す．
  FraigHandle
  rep_handle() const
  {
    return FraigHandle(mRepNode, rep_inv());
  }

  /// @brief 次の等価候補ノードを得る．
  FraigNode*
  next_eqnode()
  {
    return mEqLink;
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
    mFlags |= (1U << kSftP);
  }

  /// @brief 等価候補ノードを追加する．
  void
  add_eqnode(
    FraigNode* node ///< [in] ノード
  )
  {
    mEqTail->mEqLink = node;
    mEqTail = node;
    node->mRepNode = this;
    node->mEqLink = nullptr;
  }

  /// @brief 削除済みの印をつける．
  void
  set_dmark()
  {
    mFlags |= (1U << kSftD);
  }


public:
  //////////////////////////////////////////////////////////////////////
  // ハッシュ用のリンクアクセス関数
  //////////////////////////////////////////////////////////////////////

  FraigNode*&
  link(
    SizeType link_pos ///< [in] リンクの種類 ( 0 or 1 )
  )
  {
    return link_pos == 0 ? mLink1 : mLink2;
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
    mFlags |= (1U << kSft0);
  }

  /// @brief 1 の値を取ったことを記録する．
  void
  set_1mark()
  {
    mFlags |= (1U << kSft1);
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // CNF 上の変数番号
  SatLiteral mVarId;

  // ファンインのノード
  FraigNode* mFanins[2];

  // 0/1マーク，極性などの情報をパックしたもの
  ymuint32 mFlags;

  // シミュレーションパタン
  ymuint64* mPat{nullptr};

  // mPat のハッシュ値
  SizeType mHash{0UL};

  // 構造ハッシュ用のリンクポインタ
  FraigNode* mLink1{nullptr};

  // シグネチャハッシュ用のリンクポインタ
  FraigNode* mLink2{nullptr};

  // 代表ノード情報
  FraigNode* mRepNode{nullptr};

  // 次の等価候補ノード
  FraigNode* mEqLink{nullptr};

  // 等価候補リストの末尾のノード
  FraigNode* mEqTail{nullptr};


private:
  //////////////////////////////////////////////////////////////////////
  // クラスメンバ
  //////////////////////////////////////////////////////////////////////

  // ハッシュ用の素数配列
  static
  ymuint64 mPrimes[];


private:
  //////////////////////////////////////////////////////////////////////
  // mFlags で用いるシフト定数
  //////////////////////////////////////////////////////////////////////

  // 入力フラグ
  static
  const int kSftI  = 0;

  // ファンイン0 の極性
  static
  const int kSftP0 = 1;

  // ファンイン1 の極性
  static
  const int kSftP1 = 2;

  // 0 になったことがあるかどうか
  static
  const int kSft0  = 3;

  // 1 になったことがあるかどうか
  static
  const int kSft1  = 4;

  // ハッシュパタンの極性
  static
  const int kSftH  = 5;

  // 等価グループ中の極性
  static
  const int kSftP  = 6;

  // 削除マーク
  static
  const int kSftD  = 7;

};

END_NAMESPACE_FRAIG

#endif // FRAIGNODE_H
