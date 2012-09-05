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

  /// @brief コンストラクタ
  BlifNetworkImpl();

  /// @brief デストラクタ
  ~BlifNetworkImpl();


public:

  /// @brief model 名を得る．
  string
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
  /// @param[in] pos 位置番号 ( 0 <= pos < npi() )
  const BlifNode*
  pi(ymuint32 pos) const;

  /// @brief 外部出力数を得る．
  ymuint32
  po_num() const;

  /// @brief 外部出力を得る．
  /// @param[in] pos 位置番号 ( 0 <= pos < npo() )
  const BlifNode*
  po(ymuint32 pos) const;

  /// @brief ラッチ数を得る．
  ymuint32
  ff_num() const;

  /// @brief ラッチを得る．
  /// @param[in] pos 位置番号 ( 0 <= pos < nff() )
  const BlifNode*
  ff(ymuint32 pos) const;

  /// @brief 論理ノード数を得る．
  ymuint32
  logic_num() const;

  /// @brief 論理ノードを得る．
  /// @param[in] pos 位置番号 ( 0 <= pos < nlogic() )
  const BlifNode*
  logic(ymuint32 pos) const;


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
  // BlifNetworkReader のみが用いる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容をクリアする．
  void
  clear();

  /// @brief ノードを取り出す．
  /// @param[in] id ID 番号
  /// @note なければ作成する．
  BlifNode*
  get_node(ymuint32 id);

  /// @brief ノード名を設定する．
  /// @param[in] node 対象のノード
  /// @param[in] name 名前
  void
  set_node_name(BlifNode* node,
		const char* name);

  /// @brief ノードを外部入力に設定する．
  /// @param[in] node 対象のノード
  /// @note 既に他の型に設定されていたら false を返す．
  bool
  set_input_type(BlifNode* node);

  /// @brief ノードを論理ノードに設定する．
  /// @param[in] node 対象のノード
  /// @param[in] ni ファンイン数
  /// @param[in] nc キューブ数
  /// @param[in] cover_pat 全キューブのパタンをつなげたもの
  /// @param[in] opat 出力のパタン
  /// @note 既に他の型に設定されていたら false を返す．
  bool
  set_logic_type(BlifNode* node,
		 ymuint32 ni,
		 ymuint32 nc,
		 const char* cover_pat,
		 char opat);

  /// @brief ノードをラッチノードに設定する．
  /// @param[in] node 対象のノード
  /// @param[in] rval リセット値 ( '0', '1', ' ' のいづれか )
  /// @note 既に他の型に設定されていたら false を返す．
  bool
  set_latch_type(BlifNode* node,
		 char rval);

  /// @brief BlifNode のファンイン領域を確保する．
  /// @param[in] node 対象のノード
  /// @param[in] ni ファンイン数
  void
  alloc_fanin(BlifNode* node,
	      ymuint32 ni);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // メモリアロケータ
  SimpleAlloc mAlloc;

  // 名前 (.model)
  string mName;

  // ID をキーにしてノードを収めた配列
  // ID は穴が空いているかもしれない．
  vector<BlifNode*> mNodeArray;

  // 外部入力の配列
  vector<BlifNode*> mPIArray;

  // 外部出力の配列
  vector<BlifNode*> mPOArray;

  // ラッチの配列
  vector<BlifNode*> mFFArray;

  // 論理ノードの配列
  vector<BlifNode*> mLogicArray;

};


//////////////////////////////////////////////////////////////////////
//  BlifNetworkImpl のインライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief model 名を得る．
inline
string
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
// @param[in] pos 位置番号 ( 0 <= pos < npi() )
inline
const BlifNode*
BlifNetworkImpl::pi(ymuint32 pos) const
{
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
// @param[in] pos 位置番号 ( 0 <= pos < npo() )
inline
const BlifNode*
BlifNetworkImpl::po(ymuint32 pos) const
{
  return mPOArray[pos];
}

// @brief ラッチ数を得る．
inline
ymuint32
BlifNetworkImpl::ff_num() const
{
  return mFFArray.size();
}

// @brief ラッチを得る．
// @param[in] pos 位置番号 ( 0 <= pos < nff() )
inline
const BlifNode*
BlifNetworkImpl::ff(ymuint32 pos) const
{
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
// @param[in] pos 位置番号 ( 0 <= pos < nlogic() )
// @note 論理ノードはトポロジカル順に整列している．
inline
const BlifNode*
BlifNetworkImpl::logic(ymuint32 pos) const
{
  return mLogicArray[pos];
}

END_NAMESPACE_YM_NETWORKS_BLIF

#endif // BLIFNETWORKIMPL_H
