#ifndef BLIFNETWORKIMPL_H
#define BLIFNETWORKIMPL_H

/// @file BlifNetworkImpl.h
/// @brief BlifNetworkImpl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ym_networks/blif_nsdef.h"
#include "ym_cell/cell_nsdef.h"
#include "ym_utils/SimpleAlloc.h"


BEGIN_NAMESPACE_YM_NETWORKS_BLIF

class BlifNetworkHandler;

//////////////////////////////////////////////////////////////////////
/// @class BlifNetworkImpl BlifNetworkImpl.h "BlifNetworkImpl.h"
/// @brief BlifNetwork の実装クラス
//////////////////////////////////////////////////////////////////////
class BlifNetworkImpl
{
  friend class BlifNetworkHandler;

public:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ/デストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ
  BlifNetworkImpl();

  /// @brief デストラクタ
  ~BlifNetworkImpl();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  // ほとんど BlifNetwork と同じインターフェイスを持つ．
  //////////////////////////////////////////////////////////////////////

  /// @brief model 名を得る．
  const char*
  name() const;

  /// @brief ノードの ID 番号の最大値 + 1 を求める．
  ymuint32
  max_node_id() const;

  /// @brief ID 番号からノードを得る．
  /// @param[in] id ID 番号 ( 0 <= id < max_node_id() )
  /// @note 使われていない ID の場合には NULL が返される．
  const BlifNode*
  node(ymuint32 id) const;

  /// @brief 外部入力数を得る．
  ymuint32
  pi_num() const;

  /// @brief 外部入力を得る．
  /// @param[in] pos 位置番号 ( 0 <= pos < pi_num() )
  const BlifNode*
  pi(ymuint32 pos) const;

  /// @brief 外部出力数を得る．
  ymuint32
  po_num() const;

  /// @brief 外部出力を得る．
  /// @param[in] pos 位置番号 ( 0 <= pos < po_num() )
  const BlifNode*
  po(ymuint32 pos) const;

  /// @brief ラッチ数を得る．
  ymuint32
  ff_num() const;

  /// @brief ラッチを得る．
  /// @param[in] pos 位置番号 ( 0 <= pos < ff_num() )
  const BlifNode*
  ff(ymuint32 pos) const;

  /// @brief 論理ノード数を得る．
  ymuint32
  logic_num() const;

  /// @brief 論理ノードを得る．
  /// @param[in] pos 位置番号 ( 0 <= pos < logic_num() )
  const BlifNode*
  logic(ymuint32 pos) const;

  /// @brief ゲートノード数を得る．
  ymuint32
  gate_num() const;

  /// @brief ゲートノードを得る．
  /// @param[in] pos 位置番号 ( 0 <= pos < logic_num() )
  const BlifNode*
  gate(ymuint32 pos) const;


public:
  //////////////////////////////////////////////////////////////////////
  // blif 形式のファイルとの間で入出力を行なう関数
  //////////////////////////////////////////////////////////////////////

  /// @brief blif 形式のファイルを読み込む．
  /// @param[in] filename ファイル名
  /// @param[in] cell_library セルライブラリ
  /// @retval true 正常に読み込めた
  /// @retval false 読み込み中にエラーが起こった．
  bool
  read_blif(const string& filename,
	    const CellLibrary* cell_library = NULL);

  /// @brief 内容を blif 形式で出力する．
  /// @param[in] s 出力先のストリーム
  void
  write_blif(ostream& s) const;


private:
  //////////////////////////////////////////////////////////////////////
  // BlifNetworkHandler のみが用いる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容をクリアする．
  void
  clear();

  /// @brief .model 名の設定
  void
  set_model(const char* name);

  /// @brief 外部入力ノードを生成する．
  /// @param[in] node_id ノードID
  /// @param[in] node_name ノード名
  void
  new_input(ymuint32 node_id,
	    const char* node_name);

  /// @brief 外部出力ノードの番号を登録する．
  /// @param[in] node_id ノードID
  void
  new_output(ymuint32 node_id);

  /// @brief ラッチノードを生成する．
  /// @param[in] node_id ノードID
  /// @param[in] node_name ノード名
  /// @param[in] inode_id ファンインのID番号
  /// @param[in] rval リセット値 ( '0', '1', ' ' のいずれか )
  void
  new_latch(ymuint32 node_id,
	    const char* node_name,
	    ymuint32 inode_id,
	    char rval);

  /// @brief 論理ノードを生成する．
  /// @param[in] node_id ノードID
  /// @param[in] node_name ノード名
  /// @param[in] inode_id_array ファンインのID番号の配列
  /// @param[in] nc キューブ数
  /// @param[in] cover_pat 全キューブのパタンをつなげたもの
  /// @param[in] opat 出力のパタン
  void
  new_logic(ymuint32 node_id,
	    const char* node_name,
	    const vector<ymuint32>& inode_id_array,
	    ymuint32 nc,
	    const char* cover_pat,
	    char opat);

  /// @brief ゲートノードを生成する．
  /// @param[in] node_id ノードID
  /// @param[in] node_name ノード名
  /// @param[in] inode_id_array ファンインのID番号の配列
  /// @param[in] cell セルへのポインタ
  void
  new_gate(ymuint32 node_id,
	   const char* node_name,
	   const vector<ymuint32>& inode_id_array,
	   const Cell* cell);

  /// @brief ノードをセットする．
  /// @param[in] node_id ノードID
  /// @param[in] node 対象のノード
  void
  set_node(ymuint32 node_id,
	   BlifNode* ndoe);

  /// @brief 文字列領域を確保する．
  const char*
  alloc_string(const char* src_str);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // メモリアロケータ
  SimpleAlloc mAlloc;

  // 名前 (.model)
  // 領域は mAlloc で確保する．
  const char* mName;

  // ID をキーにしてノードを収めた配列
  // ID は穴が空いているかもしれない．
  vector<BlifNode*> mNodeArray;

  // 外部入力の配列
  vector<BlifNode*> mPIArray;

  // 外部出力ノードのIDの配列
  vector<ymuint32> mPOArray;

  // ラッチノードの配列
  vector<BlifNode*> mFFArray;

  // 論理ノードの配列
  vector<BlifNode*> mLogicArray;

  // ゲートノードの配列
  vector<BlifNode*> mGateArray;

};


//////////////////////////////////////////////////////////////////////
//  BlifNetworkImpl のインライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief model 名を得る．
inline
const char*
BlifNetworkImpl::name() const
{
  return mName;
}

// @brief ノードの ID 番号の最大値 + 1 を求める．
inline
ymuint32
BlifNetworkImpl::max_node_id() const
{
  return mNodeArray.size();
}

// @brief ID 番号からノードを得る．
// @param[in] id ID 番号
// @note 使われていない ID の場合には NULL が返される．
inline
const BlifNode*
BlifNetworkImpl::node(ymuint32 id) const
{
  assert_cond( id < max_node_id(), __FILE__, __LINE__);
  return mNodeArray[id];
}

// @brief 外部入力数を得る．
inline
ymuint32
BlifNetworkImpl::pi_num() const
{
  return mPIArray.size();
}

// @brief 外部入力を得る．
// @param[in] pos 位置番号 ( 0 <= pos < pi_num() )
inline
const BlifNode*
BlifNetworkImpl::pi(ymuint32 pos) const
{
  assert_cond( pos < pi_num(), __FILE__, __LINE__);
  return mPIArray[pos];
}

// @brief 外部出力数を得る．
inline
ymuint32
BlifNetworkImpl::po_num() const
{
  return mPOArray.size();
}

// @brief 外部出力を得る．
// @param[in] pos 位置番号 ( 0 <= pos < po_num() )
inline
const BlifNode*
BlifNetworkImpl::po(ymuint32 pos) const
{
  assert_cond( pos < po_num(), __FILE__, __LINE__);
  return node(mPOArray[pos]);
}

// @brief ラッチ数を得る．
inline
ymuint32
BlifNetworkImpl::ff_num() const
{
  return mFFArray.size();
}

// @brief ラッチを得る．
// @param[in] pos 位置番号 ( 0 <= pos < ff_num() )
inline
const BlifNode*
BlifNetworkImpl::ff(ymuint32 pos) const
{
  assert_cond( pos < ff_num(), __FILE__, __LINE__);
  return mFFArray[pos];
}

// @brief 論理ノード数を得る．
inline
ymuint32
BlifNetworkImpl::logic_num() const
{
  return mLogicArray.size();
}

// @brief 論理ノードを得る．
// @param[in] pos 位置番号 ( 0 <= pos < logic_num() )
// @note 論理ノードはトポロジカル順に整列している．
inline
const BlifNode*
BlifNetworkImpl::logic(ymuint32 pos) const
{
  assert_cond( pos < logic_num(), __FILE__, __LINE__);
  return mLogicArray[pos];
}

// @brief ゲートノード数を得る．
inline
ymuint32
BlifNetworkImpl::gate_num() const
{
  return mGateArray.size();
}

// @brief ゲートノードを得る．
// @param[in] pos 位置番号 ( 0 <= pos < logic_num() )
inline
const BlifNode*
BlifNetworkImpl::gate(ymuint32 pos) const
{
  assert_cond( pos < gate_num(), __FILE__, __LINE__);
  return mGateArray[pos];
}

END_NAMESPACE_YM_NETWORKS_BLIF

#endif // BLIFNETWORKIMPL_H
