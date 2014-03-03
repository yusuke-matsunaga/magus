
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
using nsYm::BdnFanoutList;
using nsYm::BdnEdge;
%End

public:

  /// @brief ノードの型
  enum tType {
    /// @brief 論理ノード
    kLOGIC        = 0,
    /// @brief 外部入力ノード
    kINPUT        = 1,
    /// @brief 外部出力ノード
    kOUTPUT       = 2,
  };

  /// @brief 入力ノードのサブタイプ
  enum tInputType {
    /// @brief 外部入力
    kPRIMARY_INPUT  = 0,
    /// @brief D-FFの出力
    kDFF_OUTPUT     = 1,
    /// @brief ラッチの出力
    kLATCH_OUTPUT   = 2
  };

  /// @brief 出力ノードのサブタイプ
  enum tOutputType {
    /// @brief 外部出力
    kPRIMARY_OUTPUT = 0,
    /// @brief D-FFのデータ
    kDFF_DATA       = 1,
    /// @brief D-FFのクロック
    kDFF_CLOCK      = 2,
    /// @brief D-FFのクリア信号
    kDFF_CLEAR      = 3,
    /// @brief D-FFのプリセット信号
    kDFF_PRESET     = 4,
    /// @brief ラッチのデータ
    kLATCH_DATA     = 5,
    /// @brief ラッチのイネーブル
    kLATCH_ENABLE   = 6,
    /// @brief ラッチのクリア信号
    kLATCH_CLEAR    = 7,
    /// @brief ラッチのプリセット信号
    kLATCH_PRESET   = 8
  };

  /// @brief ID 番号の取得
  /// @return ID 番号を返す．
  /// @note ID 番号はノードの生成時に BdnMgr により自動的に割り振られる．
  /// @sa BdnMgr
  unsigned int
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
  unsigned int
  fanout_num() const;

  /// @brief ファンアウトリストを得る．
  SIP_PYLIST
  fanout_list() const;
  %MethodCode
  const BdnFanoutList& fo_list = sipCpp->fanout_list();
  ymuint n = fo_list.size();
  sipRes = PyList_New(n);
  ymuint i = 0;
  for (BdnFanoutList::const_iterator p = fo_list.begin();
       p != fo_list.end(); ++ p, ++ i) {
    BdnEdge* e = const_cast<BdnEdge*>(*p);
    PyObject* obj = sipConvertFromNewType(e, sipType_BdnEdge, NULL);
    PyList_SetItem(sipRes, i, obj);
  }
  %End

  /// @brief 出力ノードにファンアウトしているとき true を返す．
  bool
  pomark() const;

  /// @brief レベルを得る．
  unsigned int
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
  unsigned int
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

  /// @brief ファンインの極性を得る．
  /// @note 出力ノードの場合のみ意味を持つ．
  bool
  output_fanin_inv() const;

  /// @brief ファンインのハンドルを得る．
  BdnConstNodeHandle
  output_fanin_handle() const;

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
  fanin(unsigned int pos) const;

  /// @brief ファンインの反転属性を得る．
  /// @param[in] pos 入力番号(0 or 1)
  bool
  fanin_inv(unsigned int pos) const;

  /// @brief ファンインのハンドルを得る．
  /// @param[in] pos 入力番号(0 or 1)
  /// @return pos 番めのファンインのハンドルを返す．
  BdnConstNodeHandle
  fanin_handle(unsigned int pos) const;

  /// @brief ファンイン0のノードを得る．
  /// @return 0番めのファンインのノード
  const BdnNode*
  fanin0() const;

  /// @brief ファンイン0のハンドルを得る．
  BdnConstNodeHandle
  fanin0_handle() const;

  /// @brief ファンイン0の反転属性を得る．
  bool
  fanin0_inv() const;

  /// @brief ファンイン1のノードを得る．
  /// @return 1番めのファンインのノード
  /// @note 該当するファンインがなければ NULL を返す．
  const BdnNode*
  fanin1() const;

  /// @brief ファンイン1の反転属性を得る．
  bool
  fanin1_inv() const;

  /// @brief ファンイン1のハンドルを得る．
  BdnConstNodeHandle
  fanin1_handle() const;


private:

  BdnNode();

  ~BdnNode();

};
