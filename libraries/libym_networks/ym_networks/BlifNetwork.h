#ifndef YM_NETWORKS_BLIFNETWORK_H
#define YM_NETWROKS_BLIFNETWORK_H

/// @file ym_networks/BlifNetwork.h
/// @brief BlifNetwork のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ym_networks/blif_nsdef.h"
#include "ym_cell/cell_nsdef.h"


BEGIN_NAMESPACE_YM_NETWORKS_BLIF

class BlifNetworkImpl;

//////////////////////////////////////////////////////////////////////
/// @class BlifNetwork BlifNetwork.h "ym_networks/BlifNetwork.h"
/// @ingroup BlifGroup
/// @brief blif ファイルの表しているネットワーク
/// @sa BlifNode
//////////////////////////////////////////////////////////////////////
class BlifNetwork
{
public:

  /// @brief コンストラクタ
  BlifNetwork();

  /// @brief デストラクタ
  ~BlifNetwork();


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を読み出す外部インターフェイス
  //////////////////////////////////////////////////////////////////////

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
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 実装オブジェクト
  BlifNetworkImpl* mImpl;

};

END_NAMESPACE_YM_NETWORKS_BLIF

#endif // BLIFNETWORK_H
