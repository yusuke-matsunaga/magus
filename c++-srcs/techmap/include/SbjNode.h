#ifndef SBJNODE_H
#define SBJNODE_H

/// @file SbjNode.h
/// @brief SbjNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016, 2018, 2022 Yusuke Matsunaga
/// All rights reserved.

#include "sbj_nsdef.h"
#include "SbjHandle.h"
#include "SbjEdge.h"


BEGIN_NAMESPACE_SBJ

//////////////////////////////////////////////////////////////////////
/// @brief SbjNode の型を表す列挙型
//////////////////////////////////////////////////////////////////////
enum class SbjNodeType {
  /// @brief 入力ノード
  Input = 0,
  /// @brief 出力ノード
  Output = 1,
  /// @brief 論理ノード(ANDタイプ)
  And = 2,
  /// @brief 論理ノード(XORタイプ)
  Xor = 3
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
  SbjNode(
    SizeType id,   ///< [in] ID番号
    bool bipol,    ///< [in] 両極性が利用可能な時に true になるフラグ
    SizeType subid ///< [in] 入力/出力番号
  );

  /// @brief コンストラクタ(出力)
  SbjNode(
    SizeType id,    ///< [in] ID番号
    SizeType subid, ///< [in] 入力/出力番号
    SbjHandle input ///< [in]  ファンインのハンドル
  );

  /// @brief コンストラクタ(AND/XOR)
  SbjNode(
    SizeType id,      ///< [in] ID番号
    SbjNodeType type, ///< [in] ノードタイプ
    SbjHandle input0, ///< [in] ファンイン0のハンドル
    SbjHandle input1  ///< [in] ファンイン1のハンドル
  );

  /// @brief デストラクタ
  ~SbjNode() = default;


public:
  //////////////////////////////////////////////////////////////////////
  /// @name 構造に関する情報を取り出す関数
  /// @{

  /// @brief ID 番号の取得
  /// @return ID 番号を返す．
  ///
  /// @sa SbjGraph
  SizeType
  id() const
  {
    return mId;
  }

  /// @brief ID を表す文字列の取得
  /// @note デバッグ時にしか意味を持たない
  string
  id_str() const;

  /// @brief タイプを得る．
  SbjNodeType
  type() const
  {
    return static_cast<SbjNodeType>(mFlags & kTypeMask);
  }

  /// @brief 入力ノードの時に true を返す．
  bool
  is_input() const
  {
    return type() == SbjNodeType::Input;
  }

  /// @brief 両極性が利用可能な入力ノードの時に true を返す．
  ///
  /// is_input() == false の時の値は不定
  bool
  is_bipol() const
  {
    return static_cast<bool>((mFlags >> kBiPolShift) & 1U);
  }

  /// @brief 出力ノードの時に true を返す．
  bool
  is_output() const
  {
    return type() == SbjNodeType::Output;
  }

  /// @brief 論理ノードの時に true を返す．
  bool
  is_logic() const
  {
    return (type() == SbjNodeType::And) || (type() == SbjNodeType::Xor);
  }

  /// @brief AND ノードの時に true を返す．
  bool
  is_and() const
  {
    return type() == SbjNodeType::And;
  }

  /// @brief XOR ノードの時に true を返す．
  bool
  is_xor() const
  {
    return type() == SbjNodeType::Xor;
  }

  /// @brief サブID (入力／出力番号)を得る．
  /// @note 入力ノード/出力ノードの場合のみ意味を持つ．
  SizeType
  subid() const
  {
    ASSERT_COND( is_input() || is_output() );

    return mSubId;
  }

  /// @brief 出力ノードの場合のファンインのハンドルを得る．
  SbjHandle
  output_fanin_handle() const
  {
    ASSERT_COND( is_output() );

    return mFanins[0];
  }

  /// @brief 出力ノードの場合のファンインを得る．
  const SbjNode*
  output_fanin() const
  {
    ASSERT_COND( is_output() );

    return mFanins[0].node();
  }

  /// @brief 出力ノードの場合のファンインの極性を返す．
  /// @return 反転していたら true を返す．
  bool
  output_fanin_inv() const
  {
    ASSERT_COND( is_output() );

    return mFanins[0].inv();
  }

  /// @brief ファンインのハンドルを得る．
  /// @return pos 番めのファンインのハンドル
  ///
  /// 該当するファンインがなければ空のハンドルを返す．
  SbjHandle
  fanin_handle(
    SizeType pos ///< [in] 入力番号(0 or 1)
  ) const
  {
    ASSERT_COND( is_logic() );
    ASSERT_COND( pos == 0 || pos == 1 );

    return mFanins[pos];
  }

  /// @brief ファンイン0のハンドルを得る．
  /// @return ファンイン0のハンドル
  ///
  /// 該当するファンインがなければ空のハンドルを返す．
  SbjHandle
  fanin0_handle() const
  {
    ASSERT_COND( is_logic() );

    return mFanins[0];
  }

  /// @brief ファンイン1のハンドルを得る．
  /// @return ファンイン1のハンドル
  ///
  /// 該当するファンインがなければ空のハンドルを返す．
  SbjHandle
  fanin1_handle() const
  {
    ASSERT_COND( is_logic() );

    return mFanins[1];
  }

  /// @brief ファンインのノードを得る．
  /// @return pos 番めのファンインのノード
  ///
  /// 該当するファンインがなければ nullptr を返す．
  const SbjNode*
  fanin(
    SizeType pos ///< [in] 入力番号(0 or 1)
  ) const
  {
    ASSERT_COND( is_logic() );
    ASSERT_COND( pos == 0 || pos == 1 );

    return mFanins[pos].node();
  }

  /// @brief ファンイン0のノードを得る．
  /// @return ファンイン0のノード
  ///
  /// 該当するファンインがなければ nullptr を返す．
  const SbjNode*
  fanin0() const
  {
    ASSERT_COND( is_logic() );

    return mFanins[0].node();
  }

  /// @brief ファンイン1のノードを得る．
  /// @return ファンイン1のノード
  ///
  /// 該当するファンインがなければ nullptr を返す．
  const SbjNode*
  fanin1() const
  {
    ASSERT_COND( is_logic() );

    return mFanins[1].node();
  }

  /// @brief ファンインの反転属性を得る．
  bool
  fanin_inv(
    SizeType pos ///< [in] 入力番号(0 or 1)
  ) const
  {
    ASSERT_COND( is_logic() );
    ASSERT_COND( pos == 0 || pos == 1 );

    return mFanins[pos].inv();
  }

  /// @brief ファンイン0の反転属性を得る．
  bool
  fanin0_inv() const
  {
    ASSERT_COND( is_logic() );

    return mFanins[0].inv();
  }

  /// @brief ファンイン1の反転属性を得る．
  bool
  fanin1_inv() const
  {
    ASSERT_COND( is_logic() );

    return mFanins[1].inv();
  }

  /// @brief ファンアウト数を得る．
  SizeType
  fanout_num() const
  {
    return mFanoutList.size();
  }

  /// @brief ファンアウトのノードを得る．
  const SbjNode*
  fanout(
    SizeType pos ///< [in] 位置番号 ( 0 <= pos < fanout_num() )
  ) const
  {
    ASSERT_COND( pos >= 0 && pos < fanout_num() );
    return mFanoutList[pos].to();
  }

  /// @brief ファンアウトリストを得る．
  const vector<SbjEdge>&
  fanout_list() const
  {
    return mFanoutList;
  }

  /// @brief 出力ノードにファンアウトしているとき true を返す．
  bool
  pomark() const
  {
    return static_cast<bool>((mFlags >> kPoShift) & 1U);
  }

  /// @brief レベルを得る．
  SizeType
  level() const
  {
    return static_cast<int>(mFlags >> kLevelShift);
  }

  /// @}
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ID 番号
  SizeType mId;

  // 入力番号/出力番号
  SizeType mSubId;

  // タイプ (+いくつかのフラグ)
  // 0 -  1: タイプ(SbjNodeType)
  // 2     : bipol (Inputのみ)
  // 3     : pomark(外部出力にファンアウトしている)
  // 4 - 31: レベル
  std::uint32_t mFlags;

  // ファンインのハンドルの配列
  SbjHandle mFanins[2];

  // ファンアウトのリスト
  vector<SbjEdge> mFanoutList;


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
  constexpr SizeType kTypeMask = 3U;
  static
  constexpr SizeType kPoMask = 1U << kPoShift;

};

END_NAMESPACE_SBJ

#endif // SBJNODE_H
