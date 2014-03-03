
/// @file BdnMgr.sip
/// @brief BdnMgr の sip ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.

%Feature COMMENT

class BdnMgr
{
%TypeHeaderCode
#include "networks/BdnMgr.h"
using nsYm::BdnMgr;
using nsYm::BdnNodeList;
%End

public:

  /// @brief コンストラクタ
  /// @note 空のネットワークに初期化される．
  BdnMgr();

  /// @brief 名前の取得
  const char*
  name() const;
  %MethodCode
  sipRes = sipCpp->name().c_str();
  %End

  /// @brief ポート数の取得
  unsigned int
  port_num() const;

  /// @brief ポートの取得
  /// @param[in] pos 位置番号 ( 0 <= pos < port_num() )
  const BdnPort*
  port(unsigned int pos) const;

  /// @brief D-FFのIDの最大値 + 1 の取得
  unsigned int
  max_dff_id() const;

  /// @brief ID番号から D-FF を得る．
  /// @param[in] id ID番号 ( 0 <= id < max_dff_id() )
  /// @note 該当するD-FFが無い場合には NULL を返す．
  const BdnDff*
  dff(unsigned int id) const;

  /// @brief D-FF 数の取得
  unsigned int
  dff_num() const;

  /// @brief ラッチのIDの最大値 + 1 の取得
  unsigned int
  max_latch_id() const;

  /// @brief ID番号からラッチを得る．
  /// @param[in] id ID番号 ( 0 <= id < max_latch_id() )
  /// @note 該当するラッチが無い場合には NULL を返す．
  const BdnLatch*
  latch(unsigned int id) const;

  /// @brief ラッチ数の取得
  unsigned int
  latch_num() const;

  /// @brief ノードIDの最大値 + 1 の取得
  /// @return ノードIDの最大値 + 1 を返す．
  unsigned int
  max_node_id() const;

  /// @brief ID 番号をキーにノードを取り出す．
  const BdnNode*
  node(unsigned int id) const;

  /// @brief 入力ノード数の取得
  /// @return 入力ノード数を返す．
  unsigned int
  input_num() const;

  /// @brief 入力ノードのリストを得る．
  SIP_PYLIST
  input_list() const;
  %MethodCode
  const BdnNodeList& node_list = sipCpp->input_list();
  ymuint n = node_list.size();
  sipRes = PyList_New(n);
  ymuint i = 0;
  for (BdnNodeList::const_iterator p = node_list.begin();
       p != node_list.end(); ++ p, ++ i) {
    BdnNode* node = const_cast<BdnNode*>(*p);
    PyObject* obj1 = sipConvertFromNewType(node, sipType_BdnNode, NULL);
    PyList_SetItem(sipRes, i, obj1);
  }
  %End

  /// @brief 出力ノード数を得る．
  unsigned int
  output_num() const;

  /// @brief 出力ノードのリストを得る．
  SIP_PYLIST
  output_list() const;
  %MethodCode
  const BdnNodeList& node_list = sipCpp->output_list();
  ymuint n = node_list.size();
  sipRes = PyList_New(n);
  ymuint i = 0;
  for (BdnNodeList::const_iterator p = node_list.begin();
       p != node_list.end(); ++ p, ++ i) {
    BdnNode* node = const_cast<BdnNode*>(*p);
    PyObject* obj1 = sipConvertFromNewType(node, sipType_BdnNode, NULL);
    PyList_SetItem(sipRes, i, obj1);
  }
  %End

  /// @brief 論理ノード数を得る．
  unsigned int
  lnode_num() const;

  /// @brief 論理ノードのリストを得る．
  SIP_PYLIST
  lnode_list() const;
  %MethodCode
  const BdnNodeList& node_list = sipCpp->lnode_list();
  ymuint n = node_list.size();
  sipRes = PyList_New(n);
  ymuint i = 0;
  for (BdnNodeList::const_iterator p = node_list.begin();
       p != node_list.end(); ++ p, ++ i) {
    BdnNode* node = const_cast<BdnNode*>(*p);
    PyObject* obj1 = sipConvertFromNewType(node, sipType_BdnNode, NULL);
    PyList_SetItem(sipRes, i, obj1);
  }
  %End

  /// @brief 指定されたANDタイプの論理ノードが存在するか調べる．
  /// @param[in] inode1_handle 1番目の入力ノード+極性
  /// @param[in] inode2_handle 2番目の入力ノード+極性
  /// @return 該当のノードのハンドルを返す．
  BdnNodeHandle
  find_and(BdnNodeHandle inode1_handle,
	   BdnNodeHandle inode2_handle) const;

  /// @brief 指定されたXORタイプの論理ノードが存在するか調べる．
  /// @param[in] inode1_handle 1番目の入力ノード+極性
  /// @param[in] inode2_handle 2番目の入力ノード+極性
  /// @return 該当のノードのハンドルを返す．
  BdnNodeHandle
  find_xor(BdnNodeHandle inode1_handle,
	   BdnNodeHandle inode2_handle) const;

  /// @brief ソートされた論理ノードのリストを得る．
  /// @param[out] node_list ノードのリストの格納先
  /// @note 入力ノードと出力ノード，ラッチノードは含まない．
  SIP_PYLIST
  sort() const;
  %MethodCode
  std::vector<const BdnNode*> node_list;
  sipCpp->sort(node_list);
  ymuint n = node_list.size();
  sipRes = PyList_New(n);
  for (ymuint i = 0; i < n; ++ i) {
    BdnNode* node = const_cast<BdnNode*>(node_list[i]);
    PyObject* obj1 = sipConvertFromNewType(node, sipType_BdnNode, NULL);
    PyList_SetItem(sipRes, i, obj1);
  }
  %End

  /// @brief 逆順でソートされた論理ノードのリストを得る．
  /// @param[out] node_list ノードのリストの格納先
  /// @note 入力ノードと出力ノード，ラッチノードは含まない．
  SIP_PYLIST
  rsort() const;
  %MethodCode
  std::vector<const BdnNode*> node_list;
  sipCpp->rsort(node_list);
  ymuint n = node_list.size();
  sipRes = PyList_New(n);
  for (ymuint i = 0; i < n; ++ i) {
    BdnNode* node = const_cast<BdnNode*>(node_list[i]);
    PyObject* obj1 = sipConvertFromNewType(node, sipType_BdnNode, NULL);
    PyList_SetItem(sipRes, i, obj1);
  }
  %End

  /// @brief 最大段数を求める．
  /// @note 段数とは入力ノードから出力ノードへ至る経路中の論理ノードの数
  unsigned int
  level() const;

  /// @brief 空にする．
  void
  clear();

  /// @brief 名前を設定する．
  /// @param[in] name 新しい名前
  void
  set_name(const char* name);

  /// @brief どこにもファンアウトしていないノードを削除する．
  void
  clean_up();

  /// @brief 入力ポートを作る．
  /// @param[in] name 名前
  /// @param[in] bit_width ビット幅
  BdnPort*
  new_input_port(const char* name,
		 unsigned int bit_width);

  /// @brief 出力ポートを作る．
  /// @param[in] name 名前
  /// @param[in] bit_width ビット幅
  BdnPort*
  new_output_port(const char* name,
		  unsigned int bit_width);

  /// @brief ポートを作る．
  /// @param[in] name 名前
  /// @param[in] iovect ビットごとの方向を指定する配列
  /// @note iovect の要素の値の意味は以下の通り
  /// - 0 : なし
  /// - 1 : 入力のみ
  /// - 2 : 出力のみ
  /// - 3 : 入力と出力
  BdnPort*
  new_port(const char* name,
	   SIP_PYLIST);
  %MethodCode
  ymuint n = PyList_Size(a1);
  std::vector<ymuint> iovect(n);
  for (ymuint i = 0; i < n; ++ i) {
    PyObject* obj1 = PyList_GetItem(a1, i);
    iovect[i] = PyInt_AsLong(obj1);
  }
  sipRes = sipCpp->new_port(a0, iovect);
  %End

  /// @brief D-FF を作る．
  /// @param[in] name 名前
  /// @return 生成されたD-FFを返す．
  BdnDff*
  new_dff(const char* name = NULL);

  /// @brief ラッチを作る．
  /// @param[in] name 名前
  /// @return 生成されたラッチを返す．
  BdnLatch*
  new_latch(const char* name = NULL);

  /// @brief 出力ノードのファンインを変更する
  /// @param[in] node 変更対象の出力ノード
  /// @param[in] inode_handle ファンインのノード+極性
  void
  change_output_fanin(BdnNode* node,
		      BdnNodeHandle inode_handle);

  /// @brief AND ノードを作る．
  /// @param[in] inode1_handle 1番めの入力ノード+極性
  /// @param[in] inode2_handle 2番めの入力ノード+極性
  /// @return 作成したノードを返す．
  /// @note すでに構造的に同じノードがあればそれを返す．
  BdnNodeHandle
  new_and(BdnNodeHandle inode1_handle,
	  BdnNodeHandle inode2_handle);

  /// @brief AND ノードを作る．
  /// @param[in] inode_handle_list 入力ノード+極性のリスト
  /// @return 作成したノードを返す．
  /// @note すでに構造的に同じノードがあればそれを返す．
  BdnNodeHandle
  new_and(SIP_PYLIST);
  %MethodCode
  ymuint n = PyList_Size(a0);
  std::vector<BdnNodeHandle> inode_handle_list(n);
  for (ymuint i = 0; i < n; ++ i) {
    PyObject* obj1 = PyList_GetItem(a0, i);
    void* p = sipConvertToType(obj1, sipType_BdnNodeHandle, NULL, 0, NULL, &sipIsErr);
    if ( sipIsErr ) {
      // エラー
      Py_INCREF(Py_None);
      return Py_None;
    }
    BdnNodeHandle h(*reinterpret_cast<BdnNodeHandle*>(p));
    inode_handle_list[i] = h;
  }
  sipRes = new BdnNodeHandle(sipCpp->new_and(inode_handle_list));
  %End

  /// @brief NAND ノードを作る．
  /// @param[in] inode1_handle 1番めの入力ノード+極性
  /// @param[in] inode2_handle 2番めの入力ノード+極性
  /// @return 作成したノードを返す．
  /// @note すでに構造的に同じノードがあればそれを返す．
  BdnNodeHandle
  new_nand(BdnNodeHandle inode1_handle,
	   BdnNodeHandle inode2_handle);

  %If (!COMMENT)
  /// @brief NAND ノードを作る．
  /// @param[in] inode_handle_list 入力ノード+極性のリスト
  /// @return 作成したノードを返す．
  /// @note すでに構造的に同じノードがあればそれを返す．
  BdnNodeHandle
  new_nand(const vector<BdnNodeHandle>& inode_handle_list);

  /// @brief OR ノードを作る．
  /// @param[in] inode1_handle 1番めの入力ノード+極性
  /// @param[in] inode2_handle 2番めの入力ノード+極性
  /// @return 作成したノードを返す．
  /// @note すでに構造的に同じノードがあればそれを返す．
  BdnNodeHandle
  new_or(BdnNodeHandle inode1_handle,
	 BdnNodeHandle inode2_handle);

  /// @brief OR ノードを作る．
  /// @param[in] inode_handle_list 入力ノード+極性のリスト
  /// @return 作成したノードを返す．
  /// @note すでに構造的に同じノードがあればそれを返す．
  BdnNodeHandle
  new_or(const vector<BdnNodeHandle>& inode_handle_list);

  /// @brief NOR ノードを作る．
  /// @param[in] inode1_handle 1番めの入力ノード+極性
  /// @param[in] inode2_handle 2番めの入力ノード+極性
  /// @return 作成したノードを返す．
  /// @note すでに構造的に同じノードがあればそれを返す．
  BdnNodeHandle
  new_nor(BdnNodeHandle inode1_handle,
	  BdnNodeHandle inode2_handle);

  /// @brief NOR ノードを作る．
  /// @param[in] inode_handle_list 入力ノード+極性のリスト
  /// @return 作成したノードを返す．
  /// @note すでに構造的に同じノードがあればそれを返す．
  BdnNodeHandle
  new_nor(const vector<BdnNodeHandle>& inode_handle_list);

  /// @brief XOR ノードを作る．
  /// @param[in] inode1_handle 1番めの入力ノード+極性
  /// @param[in] inode2_handle 2番めの入力ノード+極性
  /// @return 作成したノードを返す．
  /// @note すでに構造的に同じノードがあればそれを返す．
  BdnNodeHandle
  new_xor(BdnNodeHandle inode1_handle,
	  BdnNodeHandle inode2_handle);

  /// @brief XOR ノードを作る．
  /// @param[in] inode_handle_list 入力ノード+極性のリスト
  /// @return 作成したノードを返す．
  /// @note すでに構造的に同じノードがあればそれを返す．
  BdnNodeHandle
  new_xor(const vector<BdnNodeHandle>& inode_handle_list);

  /// @brief XNOR ノードを作る．
  /// @param[in] inode1_handle 1番めの入力ノード+極性
  /// @param[in] inode2_handle 2番めの入力ノード+極性
  /// @return 作成したノードを返す．
  /// @note すでに構造的に同じノードがあればそれを返す．
  BdnNodeHandle
  new_xnor(BdnNodeHandle inode1_handle,
	   BdnNodeHandle inode2_handle);

  /// @brief XNOR ノードを作る．
  /// @param[in] inode_handle_list 入力ノード+極性のリスト
  /// @return 作成したノードを返す．
  /// @note すでに構造的に同じノードがあればそれを返す．
  BdnNodeHandle
  new_xnor(const vector<BdnNodeHandle>& inode_handle_list);

  /// @brief AND タイプに変更する．
  /// @param[in] node 対象のノード
  /// @param[in] inode1_handle 1番めの入力ノード+極性
  /// @param[in] inode2_handle 2番めの入力ノード+極性
  void
  change_and(BdnNode* node,
	     BdnNodeHandle inode1_handle,
	     BdnNodeHandle inode2_handle);

  /// @brief AND タイプに変更する．
  /// @param[in] node 対象のノード
  /// @param[in] inode_handle_list 入力ノード+極性のリスト
  void
  change_and(BdnNode* node,
	     const vector<BdnNodeHandle>& inode_handle_list);

  /// @brief NAND タイプに変更する．
  /// @param[in] node 対象のノード
  /// @param[in] inode1_handle 1番めの入力ノード+極性
  /// @param[in] inode2_handle 2番めの入力ノード+極性
  void
  change_nand(BdnNode* node,
	     BdnNodeHandle inode1_handle,
	     BdnNodeHandle inode2_handle);

  /// @brief NAND タイプに変更する．
  /// @param[in] node 対象のノード
  /// @param[in] inode_handle_list 入力ノード+極性のリスト
  void
  change_nand(BdnNode* node,
	      const vector<BdnNodeHandle>& inode_handle_list);

  /// @brief OR タイプに変更する．
  /// @param[in] node 対象のノード
  /// @param[in] inode1_handle 1番めの入力ノード+極性
  /// @param[in] inode2_handle 2番めの入力ノード+極性
  void
  change_or(BdnNode* node,
	    BdnNodeHandle inode1_handle,
	    BdnNodeHandle inode2_handle);

  /// @brief OR タイプに変更する．
  /// @param[in] node 対象のノード
  /// @param[in] inode_handle_list 入力ノード+極性のリスト
  void
  change_or(BdnNode* node,
	    const vector<BdnNodeHandle>& inode_handle_list);

  /// @brief NOR タイプに変更する．
  /// @param[in] node 対象のノード
  /// @param[in] inode1_handle 1番めの入力ノード+極性
  /// @param[in] inode2_handle 2番めの入力ノード+極性
  void
  change_nor(BdnNode* node,
	     BdnNodeHandle inode1_handle,
	     BdnNodeHandle inode2_handle);

  /// @brief NOR タイプに変更する．
  /// @param[in] node 対象のノード
  /// @param[in] inode_handle_list 入力ノード+極性のリスト
  void
  change_nor(BdnNode* node,
	     const vector<BdnNodeHandle>& inode_handle_list);

  /// @brief XOR タイプに変更する．
  /// @param[in] node 対象のノード
  /// @param[in] inode1_handle 1番めの入力ノード+極性
  /// @param[in] inode2_handle 2番めの入力ノード+極性
  void
  change_xor(BdnNode* node,
	     BdnNodeHandle inode1_handle,
	     BdnNodeHandle inode2_handle);

  /// @brief XOR タイプに変更する．
  /// @param[in] node 対象のノード
  /// @param[in] inode_handle_list 入力ノード+極性のリスト
  void
  change_xor(BdnNode* node,
	     const vector<BdnNodeHandle>& inode_handle_list);

  /// @brief XNOR タイプに変更する．
  /// @param[in] node 対象のノード
  /// @param[in] inode1_handle 1番めの入力ノード+極性
  /// @param[in] inode2_handle 2番めの入力ノード+極性
  void
  change_xnor(BdnNode* node,
	      BdnNodeHandle inode1_handle,
	      BdnNodeHandle inode2_handle);

  /// @brief XNOR タイプに変更する．
  /// @param[in] node 対象のノード
  /// @param[in] inode_handle_list 入力ノード+極性のリスト
  void
  change_xnor(BdnNode* node,
	      const vector<BdnNodeHandle>& inode_handle_list);
%End
};
