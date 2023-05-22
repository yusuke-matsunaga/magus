
/// @file libym_techmap/lutmap/Cut.cc
/// @brief Cut の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
//
/// Copyright (C) 2005-2011, 2015, 2018, 2022 Yusuke Matsunaga
/// All rights reserved.

#include "Cut.h"
#include "SbjNode.h"


BEGIN_NAMESPACE_LUTMAP

BEGIN_NONAMESPACE

// valmap に終端のノード番号をキーとしてビットベクタ値を登録する．
// その時の node の値を計算する．
std::uint64_t
eval_node(
  const SbjNode* node,
  unordered_map<SizeType, std::uint64_t>& valmap
)
{
  if ( node == nullptr ) {
    return 0ULL;
  }

  // まずすでに評価済みかどうか調べる．
  // 葉のノードの場合もここに登録されている．
  std::uint64_t ans;
  if ( valmap.count(node->id()) == 0 ) {
    // 未登録の場合は必ず論理ノード
    ASSERT_COND( node->is_logic() );

    // 子供の値を評価する．
    std::uint64_t val0 = eval_node(node->fanin(0), valmap);
    if ( node->fanin_inv(0) ) {
      val0 ^= 0xFFFFFFFFFFFFFFFFULL;
    }

    std::uint64_t val1 = eval_node(node->fanin(1), valmap);
    if ( node->fanin_inv(0) ) {
      val1 ^= 0xFFFFFFFFFFFFFFFFULL;
    }

    // 自分の値を計算する．
    if ( node->is_xor() ) {
      ans = val0 ^ val1;
    }
    else {
      ans = val0 & val1;
    }

    // 登録しておく．
    valmap.emplace(node->id(), ans);
  }
  else {
    ans = valmap.at(node->id());
  }

  return ans;
}

// カットの表している論理関数を評価する．
std::uint64_t
eval_cut(
  const Cut* cut,
  const vector<std::uint64_t>& vals
)
{
  SizeType ni = cut->input_num();
  ASSERT_COND( ni == vals.size() );

  unordered_map<SizeType, std::uint64_t> valmap;
  for ( SizeType i = 0; i < ni; ++ i ) {
    auto inode = cut->input(i);
    valmap.emplace(inode->id(), vals[i]);
  }
  return eval_node(cut->root(), valmap);
}

END_NONAMESPACE

// @brief 論理シミュレーションを行う．
std::uint64_t
Cut::eval(
  const vector<std::uint64_t>& vals
) const
{
  SizeType ni = input_num();
  ASSERT_COND( ni == vals.size() );

  // ノードの ID 番号をキーにして値を保持するハッシュ表
  unordered_map<SizeType, std::uint64_t> valmap;
  // 葉のノードの値を登録する．
  for ( SizeType i = 0; i < ni; ++ i ) {
    valmap.emplace(input(i)->id(), vals[i]);
  }

  // 評価を行う．
  return eval_node(root(), valmap);
}

// @brief 論理関数を表す真理値表を得る．
TvFunc
Cut::make_tv(
  bool inv
) const
{
  return make_tv(inv, vector<bool>(input_num(), false));
}

// @brief 論理関数を表す真理値表を得る．
TvFunc
Cut::make_tv(
  bool oinv,
  const vector<bool>& iinv
) const
{
  SizeType ni = input_num();
  SizeType np = 1 << ni;

  vector<int> tv(np);

  // 1 の値と 0 の値
  // inv == true の時には逆にする．
  int v1 = oinv ? 0 : 1;
  int v0 = oinv ? 1 : 0;

  // 真理値表の各変数の値を表すビットベクタ
  // 6入力以上の場合には1語に収まらないので複数回にわけて処理する．
  vector<std::uint64_t> vals(ni, 0ULL);

  std::uint64_t s = 1ULL;
  int p0 = 0;
  for ( int p = 0; p < np; ++ p ) {
    for ( int i = 0; i < ni; ++ i ) {
      std::uint64_t mask = 1U << i;
      std::uint64_t mask1 = iinv[i] ? 0U : mask;
      if ( (p & mask) == mask1 ) {
	vals[i] |= s;
      }
    }
    s <<= 1;
    if ( s == 0ULL ) {
      // 64 パタン目
      std::uint64_t tmp = eval_cut(this, vals);
      for ( int p1 = p0; p1 < p; ++ p1 ) {
	if ( tmp & (1ULL << (p1 - p0)) ) {
	  tv[p1] = v1;
	}
	else {
	  tv[p1] = v0;
	}
      }
      s = 1ULL;
      p0 = p + 1;
      for ( int i = 0; i < ni; ++ i ) {
	vals[i] = 0ULL;
      }
    }
  }
  if ( s != 1ULL ) {
    // 処理されていない残りがあった．
    std::uint64_t tmp = eval_cut(this, vals);
    for ( int p1 = p0; p1 < np; ++ p1 ) {
      if ( tmp & (1ULL << (p1 - p0)) ) {
	tv[p1] = v1;
      }
      else {
	tv[p1] = v0;
      }
    }
  }

  return TvFunc(ni, tv);
}

// デバッグ用の表示関数
void
Cut::print(
  ostream& s
) const
{
  if ( root() == nullptr ) {
    s << "null cut" << endl;
  }
  else {
    s << "Node[" << root()->id() << "] : ";
    string comma = "";
    for ( int i = 0; i < input_num(); i ++ ) {
      const SbjNode* node = mInputs[i];
      s << comma << "Node[" << node->id() << "]";
      comma = ", ";
    }
    s << endl;
  }
}

END_NAMESPACE_LUTMAP
