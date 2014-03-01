
/// @file BdnNode.sip
/// @brief BdnNode の sip ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.

class BdnNode
{
%TypeHeaderCode
#include "networks/BdnNode.h"
using nsYm::BdnNode;
%End

public:

  /// @brief ID 番号の取得
  /// @return ID 番号を返す．
  /// @note ID 番号はノードの生成時に BdnMgr により自動的に割り振られる．
  /// @sa BdnMgr
  ymuint
  id() const;

  /// @brief タイプを得る．
  tType
  type() const;

  /// @brief 入力ノードの時に true を返す．
  bool
  is_input() const;

  /// @brief 出力ノードの時に true を返す．
  bool
  is_output() const;

  /// @brief 論理ノードの時に true を返す．
  bool
  is_logic() const;

  /// @brief ファンアウト数を得る．
  ymuint
  fanout_num() const;

  /// @brief ファンアウトリストを得る．
  const BdnFanoutList&
  fanout_list() const;

  /// @brief 出力ノードにファンアウトしているとき true を返す．
  bool
  pomark() const;

  /// @brief レベルを得る．
  ymuint
  level() const;

  /// @brief 入力ノードのサブタイプを得る．
  tInputType
  input_type() const;

  /// @brief 出力ノードのサブタイプを得る．
  tOutputType
  output_type() const;

  /// @brief 関連するポートを返す．
  /// @note kPRIMARY_INPUT および kPRIMARY_OUTPUT の時のみ意味を持つ．
  /// @note それ以外では NULL を返す．
  const BdnPort*
  port() const;

  /// @brief ポート中のビット位置を返す．
  /// @note kPRIMARY_INPUT および kPRIMARY_OUTPUT の時のみ意味を持つ．
  /// @note それ以外では 0 を返す．
  ymuint
  port_bitpos() const;

  /// @brief 入出力ノードの場合に相方のノードを返す．
  /// @note なければ NULL を返す．
  const BdnNode*
  alt_node() const;

  /// @brief 関連する D-FF を返す．
  /// @note D-FF に関連していない場合には NULL を返す．
  const BdnDff*
  dff() const;

  /// @brief 関連するラッチを返す．
  /// @note ラッチに関連していない場合には NULL を返す．
  const BdnLatch*
  latch() const;

  /// @brief ファンインのノードを得る．
  /// @note 出力ノードの場合のみ意味を持つ．
  const BdnNode*
  output_fanin() const;

  /// @brief ファンインのノードを得る．
  /// @note 出力ノードの場合のみ意味を持つ．
  BdnNode*
  output_fanin();

  /// @brief ファンインの極性を得る．
  /// @note 出力ノードの場合のみ意味を持つ．
  bool
  output_fanin_inv() const;

  /// @brief ファンインのハンドルを得る．
  BdnConstNodeHandle
  output_fanin_handle() const;

  /// @brief ファンインのハンドルを得る．
  BdnNodeHandle
  output_fanin_handle();

  /// @brief AND タイプのときに true を返す．
  bool
  is_and() const;

  /// @brief XOR タイプのときに true を返す．
  bool
  is_xor() const;

  /// @brief ファンインのノードを得る．
  /// @param[in] pos 入力番号(0 or 1)
  /// @return pos 番めのファンインのノード
  /// @note 該当するファンインがなければ NULL を返す．
  const BdnNode*
  fanin(ymuint pos) const;

  /// @brief ファンインのノードを得る．
  /// @param[in] pos 入力番号(0 or 1)
  /// @return pos 番めのファンインのノード
  /// @note 該当するファンインがなければ NULL を返す．
  BdnNode*
  fanin(ymuint pos);

  /// @brief ファンインの反転属性を得る．
  /// @param[in] pos 入力番号(0 or 1)
  bool
  fanin_inv(ymuint pos) const;

  /// @brief ファンインのハンドルを得る．
  /// @param[in] pos 入力番号(0 or 1)
  /// @return pos 番めのファンインのハンドルを返す．
  BdnConstNodeHandle
  fanin_handle(ymuint pos) const;

  /// @brief ファンインのハンドルを得る．
  /// @param[in] pos 入力番号(0 or 1)
  /// @return pos 番めのファンインのハンドルを返す．
  BdnNodeHandle
  fanin_handle(ymuint pos);

  /// @brief ファンイン0のノードを得る．
  /// @return 0番めのファンインのノード
  const BdnNode*
  fanin0() const;

  /// @brief ファンイン0のハンドルを得る．
  BdnConstNodeHandle
  fanin0_handle() const;

  /// @brief ファンイン0のハンドルを得る．
  BdnNodeHandle
  fanin0_handle();

  /// @brief ファンイン0のノードを得る．
  /// @return 0番めのファンインのノード
  BdnNode*
  fanin0();

  /// @brief ファンイン0の反転属性を得る．
  bool
  fanin0_inv() const;

  /// @brief ファンイン1のノードを得る．
  /// @return 1番めのファンインのノード
  /// @note 該当するファンインがなければ NULL を返す．
  const BdnNode*
  fanin1() const;

  /// @brief ファンイン1のノードを得る．
  /// @return 1番めのファンインのノード
  /// @note 該当するファンインがなければ NULL を返す．
  BdnNode*
  fanin1();

  /// @brief ファンイン1の反転属性を得る．
  bool
  fanin1_inv() const;

  /// @brief ファンイン1のハンドルを得る．
  BdnConstNodeHandle
  fanin1_handle() const;

  /// @brief ファンイン1のハンドルを得る．
  BdnNodeHandle
  fanin1_handle();

};