
/// @file BtgMatch.sip
/// @brief BtgMatch の sip ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013-2014 Yusuke Matsunaga
/// All rights reserved.

class BtgMatch
{
%TypeHeaderCode
#include "utils/BtgMatch.h"
using nsYm::BtgMatch;
%End

public:

  /// @brief 空のコンストラクタ
  BtgMatch();

  /// @brief コンストラクタ
  /// @param[in] node1_num 節点グループ1の要素数
  /// @param[in] node2_num 節点グループ2の要素数
  BtgMatch(unsigned int node1_num,
	   unsigned int node2_num);

  /// @brief 節点数を設定する．
  /// @param[in] node1_num 節点グループ1の要素数
  /// @param[in] node2_num 節点グループ2の要素数
  /// @note 以前の内容はクリアされる．
  void
  resize(unsigned int node1_num,
	 unsigned int node2_num);

  /// @brief 節点グループ1の要素数を返す．
  unsigned int
  node1_num() const;

  /// @brief 節点グループ2の要素数を返す．
  unsigned int
  node2_num() const;

  /// @brief 枝をすべて取り除く
  void
  clear_edge();

  /// @brief 枝を追加する．
  /// @param[in] node1_id 節点グループ1のノード番号 ( 0 <= node1_id < node1_num() )
  /// @param[in] node2_id 節点グループ2のノード番号 ( 0 <= node2_id < node2_num() )
  /// @param[in] weight 枝の重み (省略されたらデフォルトは1)
  /// @note 既に同じ枝があったら上書きされる．
  void
  add_edge(unsigned int node1_id,
	   unsigned int node2_id,
	   unsigned int weight = 1);

  /// @brief 枝数を返す．
  unsigned int
  edge_num() const;

  /// @brief 枝の情報を返す．
  /// @param[in] edge_id 枝のID番号 ( 0 <= edge_id < edge_num() )
  /// @param[out] node1_id 節点グループ1のノード番号
  /// @param[out] node2_id 節点グループ2のノード番号
  /// @param[out] weight 枝の重み
  SIP_PYTUPLE
  edge_info(unsigned int edge_id) const;
  %MethodCode
  ymuint node1_id;
  ymuint node2_id;
  ymuint weight;
  sipCpp->edge_info(a0, node1_id, node2_id, weight);
  sipRes = sipBuildResult(NULL, "(iii)", node1_id, node2_id, weight);
  %End

  /// @brief 最大マッチングを求める．
  /// @param[in] edge_list マッチング結果の枝を格納するリスト
  /// @return マッチング結果の重みの総和を返す．
  SIP_PYTUPLE
  calc_match();
  %MethodCode
  using std::vector;
  vector<ymuint> edge_list;
  ymuint w_total = sipCpp->calc_match(edge_list);
  ymuint n = edge_list.size();
  PyObject* py_list = PyList_New(n);
  for (ymuint i = 0; i < n; ++ i) {
    PyObject* obj = sipBuildResult(NULL, "i", edge_list[i]);
    Py_INCREF(obj);
    PyList_SetItem(py_list, i, obj);
  }
  sipRes = sipBuildResult(NULL, "(iR)", w_total, py_list);
  %End

};
