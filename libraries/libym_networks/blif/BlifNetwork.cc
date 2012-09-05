
/// @file BlifNetwork.cc
/// @brief BlifNetwork の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ym_networks/BlifNetwork.h"
#include "BlifNetworkImpl.h"


BEGIN_NAMESPACE_YM_NETWORKS_BLIF

//////////////////////////////////////////////////////////////////////
// クラス BlifNetwork
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
BlifNetwork::BlifNetwork() :
  mImpl(new BlifNetworkImpl)
{
}

// @brief デストラクタ
BlifNetwork::~BlifNetwork()
{
  delete mImpl;
}

// @brief model 名を得る．
string
BlifNetwork::name() const
{
  return mImpl->name();
}

// @brief ノードの ID 番号の最大値 + 1 を求める．
ymuint32
BlifNetwork::max_node_id() const
{
  return mImpl->max_node_id();
}

// @brief ID 番号からノードを得る．
// @param[in] id ID 番号 ( 0 <= id < max_node_id() )
// @note 使われていない ID の場合には NULL が返される．
const BlifNode*
BlifNetwork::node(ymuint32 id) const
{
  return mImpl->node(id);
}

// @brief 外部入力数を得る．
ymuint32
BlifNetwork::pi_num() const
{
  return mImpl->pi_num();
}

// @brief 外部入力を得る．
// @param[in] pos 位置番号 ( 0 <= pos < npi() )
const BlifNode*
BlifNetwork::pi(ymuint32 pos) const
{
  return mImpl->pi(pos);
}

// @brief 外部出力数を得る．
ymuint32
BlifNetwork::po_num() const
{
  return mImpl->po_num();
}

// @brief 外部出力を得る．
// @param[in] pos 位置番号 ( 0 <= pos < npo() )
const BlifNode*
BlifNetwork::po(ymuint32 pos) const
{
  return mImpl->po(pos);
}

// @brief ラッチ数を得る．
ymuint32
BlifNetwork::ff_num() const
{
  return mImpl->ff_num();
}

// @brief ラッチを得る．
// @param[in] pos 位置番号 ( 0 <= pos < nff() )
const BlifNode*
BlifNetwork::ff(ymuint32 pos) const
{
  return mImpl->ff(pos);
}

// @brief 論理ノード数を得る．
ymuint32
BlifNetwork::logic_num() const
{
  return mImpl->logic_num();
}

// @brief 論理ノードを得る．
// @param[in] pos 位置番号 ( 0 <= pos < nlogic() )
const BlifNode*
BlifNetwork::logic(ymuint32 pos) const
{
  return mImpl->logic(pos);
}

// @brief blif 形式のファイルを読み込む．
// @param[in] filename ファイル名
// @param[in] cell_library セルライブラリ
// @retval true 正常に読み込めた
// @retval false 読み込み中にエラーが起こった．
bool
BlifNetwork::read_blif(const string& filename,
		       const CellLibrary* cell_library)
{
  return mImpl->read_blif(filename, cell_library);
}

// @brief 内容を blif 形式で出力する．
// @param[in] s 出力先のストリーム
void
BlifNetwork::write_blif(ostream& s) const
{
  mImpl->write_blif(s);
}


END_NAMESPACE_YM_NETWORKS_BLIF
