#ifndef YM_NETWORKS_BLIFNODE_H
#define YM_NETWORKS_BLIFNODE_H

/// @file ym_networks/BlifNode.h
/// @brief BlifNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ym_networks/blif_nsdef.h"


BEGIN_NAMESPACE_YM_NETWORKS_BLIF

//////////////////////////////////////////////////////////////////////
/// @class BlifNode BlifNode.h "ym_networks/BlifNode.h"
/// @ingroup BlifGroup
/// @brief blif ファイルの表すネットワークのノード
/// @sa BlifNetwork
//////////////////////////////////////////////////////////////////////
class BlifNode
{
  friend class BlifNetworkImpl;
  friend class BlifNetworkHandler;

public:

  /// @brief ノードの種類
  enum tType {
    /// @brief 未定義
    kUndef,
    /// @brief 外部入力
    kInput,
    /// @brief 論理(カバー)
    kLogic,
    /// @brief ゲート(セル)
    kGate,
    /// @brief ラッチ
    kLatch
  };


private:

  /// @brief コンストラクタ
  /// @param[in] id ID 番号
  BlifNode(ymuint32 id);

  /// @brief デストラクタ
  ~BlifNode();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ID を返す．
  ymuint32
  id() const;

  /// @brief 名前を返す．
  const char*
  name() const;

  /// @brief 型を返す．
  tType
  type() const;

  /// @brief ファンイン数を得る．
  ymuint32
  fanin_num() const;

  /// @brief ファンインを求める．
  /// @param[in] pos 入力位置 ( 0 <= pos < ni() )
  const BlifNode*
  fanin(ymuint32 pos) const;

  /// @brief カバーのキューブ数を得る．
  ymuint32
  cube_num() const;

  /// @brief 入力キューブのパタンを得る．
  /// @param[in] c_pos キューブの位置 ( 0 <= c_pos < nc() )
  /// @param[in] i_pos 入力位置 ( 0 <= i_pos < ni() )
  /// @note 意味のあるパタンは '0' '1' '-'
  char
  cube_pat(ymuint32 c_pos,
	   ymuint32 i_pos) const;

  /// @brief 出力キューブを表すパタンを得る．
  /// @note 意味のあるパタンは '0' '1'
  char
  opat() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  // ここのメモリ領域はすべて BlifNetwork::mAlloc が管理する．
  //////////////////////////////////////////////////////////////////////

  // ID 番号
  ymuint32 mId;

  // 名前
  const char* mName;

  // 型
  tType mType;

  // ファンイン数
  ymuint32 mFaninNum;

  // ファンインの配列
  BlifNode** mFanins;

  // キューブ数
  ymuint32 mCubeNum;

  // カバーを表す文字の配列
  // 中身は '0', '1', '-' のいづれか
  // サイズは ni() * nc()
  // end-of-string マーカはないので注意
  const char* mCover;

  // 出力キューブを表す文字　'0' or '1'
  char mOpat;

};


//////////////////////////////////////////////////////////////////////
// BlifNode のインライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief ID を返す．
inline
ymuint32
BlifNode::id() const
{
  return mId;
}

// @brief 名前を返す．
inline
const char*
BlifNode::name() const
{
  return mName;
}

// @brief 型を返す．
inline
BlifNode::tType
BlifNode::type() const
{
  return mType;
}

// @brief ファンイン数を得る．
inline
ymuint32
BlifNode::fanin_num() const
{
  return mFaninNum;
}

// @brief ファンインを求める．
// @param[in] pos 入力位置 ( 0 <= pos < ni() )
inline
const BlifNode*
BlifNode::fanin(ymuint32 pos) const
{
  return mFanins[pos];
}

// @brief カバーのキューブ数を得る．
inline
ymuint32
BlifNode::cube_num() const
{
  return mCubeNum;
}

// @brief 入力キューブのパタンを得る．
// @param[in] c_pos キューブの位置 ( 0 <= c_pos < nc() )
// @param[in] i_pos 入力位置 ( 0 <= i_pos < ni() )
inline
char
BlifNode::cube_pat(ymuint32 c_pos,
		   ymuint32 i_pos) const
{
  return mCover[c_pos * fanin_num() + i_pos];
}

// @brief 出力キューブを表すパタンを得る．
inline
char
BlifNode::opat() const
{
  return mOpat;
}

END_NAMESPACE_YM_NETWORKS_BLIF

#endif // YM_NETWORKS_BLIFNODE_H
