#ifndef NETWORKS_BLIFNODE_H
#define NETWORKS_BLIFNODE_H

/// @file YmNetworks/BlifNode.h
/// @brief BlifNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmNetworks/blif_nsdef.h"
#include "YmCell/cell_nsdef.h"


BEGIN_NAMESPACE_YM_NETWORKS_BLIF

//////////////////////////////////////////////////////////////////////
/// @class BlifNode BlifNode.h "YmNetworks/BlifNode.h"
/// @ingroup BlifGroup
/// @brief blif ファイルの表すネットワークのノード
/// @sa BlifNetwork
//////////////////////////////////////////////////////////////////////
class BlifNode
{
public:

  /// @brief ノードの種類
  enum tType {
    /// @brief 外部入力
    kInput,
    /// @brief 論理(カバー)
    kLogic,
    /// @brief ゲート(セル)
    kGate,
    /// @brief ラッチ
    kLatch
  };


public:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ/デストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ
  BlifNode() { }

  /// @brief デストラクタ
  virtual
  ~BlifNode() { }


public:
  //////////////////////////////////////////////////////////////////////
  // 全タイプ共通の外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ノードID を返す．
  virtual
  ymuint32
  id() const = 0;

  /// @brief 名前を返す．
  virtual
  const char*
  name() const = 0;

  /// @brief 型を返す．
  virtual
  tType
  type() const = 0;

  /// @brief 内容を blif 形式で出力する．
  /// @param[in] s 出力先のストリーム
  virtual
  void
  write_blif(ostream& s) const = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // 論理タイプ/ゲートタイプに共通の外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ファンイン数を得る．
  virtual
  ymuint32
  fanin_num() const = 0;

  /// @brief ファンインのノードIDを返す．
  /// @param[in] pos 入力位置 ( 0 <= pos < fanin_num() )
  virtual
  ymuint32
  fanin_id(ymuint32 pos) const = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // 論理タイプの外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief カバーのキューブ数を得る．
  virtual
  ymuint32
  cube_num() const = 0;

  /// @brief 入力キューブのパタンを得る．
  /// @param[in] c_pos キューブの位置 ( 0 <= c_pos < nc() )
  /// @param[in] i_pos 入力位置 ( 0 <= i_pos < fanin_num() )
  /// @note 意味のあるパタンは '0' '1' '-'
  virtual
  char
  cube_pat(ymuint32 c_pos,
	   ymuint32 i_pos) const = 0;

  /// @brief 出力キューブを表すパタンを得る．
  /// @note 意味のあるパタンは '0' '1'
  virtual
  char
  opat() const = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // ゲートタイプの外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief セルを返す．
  virtual
  const Cell*
  cell() const = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // ラッチタイプの外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力ノードのID番号を返す．
  virtual
  ymuint32
  inode_id() const = 0;

  /// @brief リセット値を返す．
  virtual
  char
  reset_val() const = 0;

};

END_NAMESPACE_YM_NETWORKS_BLIF

#endif // NETWORKS_BLIFNODE_H
