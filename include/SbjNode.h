#ifndef SBJNODE_H
#define SBJNODE_H

/// @file SbjNode.h
/// @brief SbjNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016, 2018 Yusuke Matsunaga
/// All rights reserved.


#include "sbj_nsdef.h"
#include "SbjHandle.h"
#include "SbjEdge.h"


BEGIN_NAMESPACE_YM_SBJ

//////////////////////////////////////////////////////////////////////
/// @brief SbjNode の型を表す列挙型
//////////////////////////////////////////////////////////////////////
enum class SbjNodeType {
  /// @brief 入力ノード
  Input = 0,
  /// @brief 出力ノード
  Output = 1,
  /// @brief 論理ノード(ANDタイプ)
  LogicAnd = 2,
  /// @brief 論理ノード(XORタイプ)
  LogicXor = 3
};


//////////////////////////////////////////////////////////////////////
/// @class SbjNode SbjGraph.h "SbjGraph.h"
/// @brief サブジェクトグラフを構成するプリミティブノードを表すクラス
///
/// ノードの種類は
/// - 入力ノード
/// - 出力ノード
/// - 論理ノード
/// の 3種類がある．
///
/// 入力ノードの場合，どちらの極性も利用可能な場合がある．
///
/// 論理ノードの場合，常に2つのファンインを持つ．
/// ノードの論理タイプが AND と XOR の2種類があり，
/// さらに2つの入力の極性がある．
/// ただし，XOR タイプの場合には入力は反転させない．
///
/// @sa SbjEdge SbjGraph
//////////////////////////////////////////////////////////////////////
class SbjNode
{
public:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ / デストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ(入力)
  /// @param[in] id ID番号
  /// @param[in] bipol 両極性が利用可能な時に true になるフラグ
  /// @param[in] subid 入力/出力番号
  SbjNode(int id,
	  bool bipol,
	  int subid);

  /// @brief コンストラクタ(出力)
  /// @param[in] id ID番号
  /// @param[in] subid 入力/出力番号
  /// @param[in] input ファンインのハンドル
  SbjNode(int id,
	  int subid,
	  SbjHandle input);

  /// @brief コンストラクタ(AND/XOR)
  /// @param[in] id ID番号
  /// @param[in] type ノードタイプ
  /// @param[in] bipol 両極性が利用可能な時に true になるフラグ
  /// @param[in] subid 入力/出力番号
  /// @param[in] input0, input1 ファンインのハンドル
  SbjNode(int id,
	  SbjNodeType type,
	  SbjHandle input0,
	  SbjHandle input1);

  /// @brief デストラクタ
  ~SbjNode();


public:
  //////////////////////////////////////////////////////////////////////
  /// @name 構造に関する情報を取り出す関数
  /// @{

  /// @brief ID 番号の取得
  /// @return ID 番号を返す．
  ///
  /// @sa SbjGraph
  int
  id() const;

  /// @brief ID を表す文字列の取得
  /// @note デバッグ時にしか意味を持たない
  string
  id_str() const;

  /// @brief タイプを得る．
  SbjNodeType
  type() const;

  /// @brief 入力ノードの時に true を返す．
  bool
  is_input() const;

  /// @brief 両極性が利用可能な入力ノードの時に true を返す．
  ///
  /// is_input() == false の時の値は不定
  bool
  is_bipol() const;

  /// @brief 出力ノードの時に true を返す．
  bool
  is_output() const;

  /// @brief 論理ノードの時に true を返す．
  bool
  is_logic() const;

  /// @brief AND ノードの時に true を返す．
  bool
  is_and() const;

  /// @brief XOR ノードの時に true を返す．
  bool
  is_xor() const;

  /// @brief サブID (入力／出力番号)を得る．
  /// @note 入力ノード/出力ノードの場合のみ意味を持つ．
  int
  subid() const;

  /// @brief 出力ノードの場合のファンインのハンドルを得る．
  SbjHandle
  output_fanin_handle() const;

  /// @brief 出力ノードの場合のファンインを得る．
  const SbjNode*
  output_fanin() const;

  /// @brief 出力ノードの場合のファンインの極性を返す．
  /// @return 反転していたら true を返す．
  bool
  output_fanin_inv() const;

  /// @brief ファンインのハンドルを得る．
  /// @param[in] pos 入力番号(0 or 1)
  /// @return pos 番めのファンインのハンドル
  ///
  /// 該当するファンインがなければ空のハンドルを返す．
  SbjHandle
  fanin_handle(int pos) const;

  /// @brief ファンイン0のハンドルを得る．
  /// @return ファンイン0のハンドル
  ///
  /// 該当するファンインがなければ空のハンドルを返す．
  SbjHandle
  fanin0_handle() const;

  /// @brief ファンイン1のハンドルを得る．
  /// @return ファンイン1のハンドル
  ///
  /// 該当するファンインがなければ空のハンドルを返す．
  SbjHandle
  fanin1_handle() const;

  /// @brief ファンインのノードを得る．
  /// @param[in] pos 入力番号(0 or 1)
  /// @return pos 番めのファンインのノード
  ///
  /// 該当するファンインがなければ nullptr を返す．
  const SbjNode*
  fanin(int pos) const;

  /// @brief ファンイン0のノードを得る．
  /// @return ファンイン0のノード
  ///
  /// 該当するファンインがなければ nullptr を返す．
  const SbjNode*
  fanin0() const;

  /// @brief ファンイン1のノードを得る．
  /// @return ファンイン1のノード
  ///
  /// 該当するファンインがなければ nullptr を返す．
  const SbjNode*
  fanin1() const;

  /// @brief ファンインの反転属性を得る．
  /// @param[in] pos 入力番号 (0 or 1)
  bool
  fanin_inv(int pos) const;

  /// @brief ファンイン0の反転属性を得る．
  bool
  fanin0_inv() const;

  /// @brief ファンイン1の反転属性を得る．
  bool
  fanin1_inv() const;

  /// @brief ファンアウト数を得る．
  int
  fanout_num() const;

  /// @brief ファンアウトリストを得る．
  const list<SbjEdge>&
  fanout_list() const;

  /// @brief 出力ノードにファンアウトしているとき true を返す．
  bool
  pomark() const;

  /// @brief レベルを得る．
  int
  level() const;

  /// @}
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ID 番号
  int mId;

  // 入力番号/出力番号
  int mSubId;

  // タイプ (+いくつかのフラグ)
  // 0 -  1: タイプ(SbjNodeType)
  // 2     : bipol (Inputのみ)
  // 3     : pomark(外部出力にファンアウトしている)
  // 4 - 31: レベル
  ymuint32 mFlags;

  // ファンインのハンドルの配列
  SbjHandle mFanins[2];

  // ファンアウトのリスト
  list<SbjEdge> mFanoutList;


private:
  //////////////////////////////////////////////////////////////////////
  // 定数の定義
  //////////////////////////////////////////////////////////////////////

  // mFlags の演算で用いる定数
  static
  constexpr int kTypeShift = 0;
  static
  constexpr int kBiPolShift = 2;
  static
  constexpr int kPoShift = 3;
  static
  constexpr int kLevelShift = 4;

  static
  constexpr ymuint kTypeMask = 3U;
  static
  constexpr ymuint kPoMask = 1U << kPoShift;

};


//////////////////////////////////////////////////////////////////////
// inline 関数の定義
//////////////////////////////////////////////////////////////////////

// ID 番号を得る．
inline
int
SbjNode::id() const
{
  return mId;
}

// タイプを得る．
inline
SbjNodeType
SbjNode::type() const
{
  return static_cast<SbjNodeType>(mFlags & kTypeMask);
}

// 入力ノードの時に true を返す．
inline
bool
SbjNode::is_input() const
{
  return type() == SbjNodeType::Input;
}

// @brief 逆極性も利用可能な入力ノードの時に true を返す．
//
// is_input() == false の時の値は不定
inline
bool
SbjNode::is_bipol() const
{
  return static_cast<bool>((mFlags >> kBiPolShift) & 1U);
}

// 出力ノードの時に true を返す．
inline
bool
SbjNode::is_output() const
{
  return type() == SbjNodeType::Output;
}

// 論理ノードの時に true を返す．
inline
bool
SbjNode::is_logic() const
{
  return (type() == SbjNodeType::LogicAnd) || (type() == SbjNodeType::LogicXor);
}

// @brief AND ノードの時に true を返す．
inline
bool
SbjNode::is_and() const
{
  return type() == SbjNodeType::LogicAnd;
}

// @brief XOR ノードの時に true を返す．
inline
bool
SbjNode::is_xor() const
{
  return type() == SbjNodeType::LogicXor;
}

// @brief サブID (入力／出力番号)を得る．
inline
int
SbjNode::subid() const
{
  ASSERT_COND( is_input() || is_output() );

  return mSubId;
}

// @brief 出力ノードの場合のファンインのハンドルを得る．
inline
SbjHandle
SbjNode::output_fanin_handle() const
{
  ASSERT_COND( is_output() );

  return mFanins[0];
}

// @brief 出力ノードの場合のファンインを得る．
inline
const SbjNode*
SbjNode::output_fanin() const
{
  ASSERT_COND( is_output() );

  return mFanins[0].node();
}

// @brief 出力ノードの極性を得る．
inline
bool
SbjNode::output_fanin_inv() const
{
  ASSERT_COND( is_output() );

  return mFanins[0].inv();
}

// @brief ファンインのハンドルを得る．
// @param[in] pos 入力番号(0 or 1)
// @return pos 番めのファンインのハンドル
//
// 該当するファンインがなければ空のハンドルを返す．
inline
SbjHandle
SbjNode::fanin_handle(int pos) const
{
  ASSERT_COND( is_logic() );
  ASSERT_COND( pos == 0 || pos == 1 );

  return mFanins[pos];
}

// @brief ファンイン0のハンドルを得る．
// @return ファンイン0のハンドル
//
// 該当するファンインがなければ空のハンドルを返す．
inline
SbjHandle
SbjNode::fanin0_handle() const
{
  ASSERT_COND( is_logic() );

  return mFanins[0];
}

// @brief ファンイン1のハンドルを得る．
// @return ファンイン1のハンドル
//
// 該当するファンインがなければ空のハンドルを返す．
inline
SbjHandle
SbjNode::fanin1_handle() const
{
  ASSERT_COND( is_logic() );

  return mFanins[1];
}

// @brief ファンインのノードを得る．
inline
const SbjNode*
SbjNode::fanin(int pos) const
{
  ASSERT_COND( is_logic() );
  ASSERT_COND( pos == 0 || pos == 1 );

  return mFanins[pos].node();
}

// @brief ファンイン0のノードを得る．
// @return ファンイン0のノード
//
// 該当するファンインがなければ nullptr を返す．
inline
const SbjNode*
SbjNode::fanin0() const
{
  ASSERT_COND( is_logic() );

  return mFanins[0].node();
}

// @brief ファンイン1のノードを得る．
// @return ファンイン1のノード
//
// 該当するファンインがなければ nullptr を返す．
inline
const SbjNode*
SbjNode::fanin1() const
{
  ASSERT_COND( is_logic() );

  return mFanins[1].node();
}

// @brief ファンインの反転属性を得る．
// @param[in] pos 入力番号 (0 or 1)
inline
bool
SbjNode::fanin_inv(int pos) const
{
  ASSERT_COND( is_logic() );
  ASSERT_COND( pos == 0 || pos == 1 );

  return mFanins[pos].inv();
}

// @brief ファンイン0の反転属性を得る．
inline
bool
SbjNode::fanin0_inv() const
{
  ASSERT_COND( is_logic() );

  return mFanins[0].inv();
}

// @brief ファンイン1の反転属性を得る．
inline
bool
SbjNode::fanin1_inv() const
{
  ASSERT_COND( is_logic() );

  return mFanins[1].inv();
}

// @brief ファンアウト数を得る．
inline
int
SbjNode::fanout_num() const
{
  return mFanoutList.size();
}

// @brief ファンアウトリストを得る．
inline
const list<SbjEdge>&
SbjNode::fanout_list() const
{
  return mFanoutList;
}

// @brief 出力ノードにファンアウトしているとき true を返す．
inline
bool
SbjNode::pomark() const
{
  return static_cast<bool>((mFlags >> kPoShift) & 1U);
}

// @brief レベルを得る．
inline
int
SbjNode::level() const
{
  return static_cast<int>(mFlags >> kLevelShift);
}

END_NAMESPACE_YM_SBJ

#endif // SBJNODE_H
