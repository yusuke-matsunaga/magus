#ifndef VIDLITMAP_H
#define VIDLITMAP_H

/// @file VidLitMap.h
/// @brief VidLitMap のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "LitMap.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class VidLitMap VidLitMap.h "VidLitMap.h"
/// @brief VidMap を用いた LitMap
//////////////////////////////////////////////////////////////////////
class VidLitMap :
  public LitMap
{
public:

  /// @brief コンストラクタ
  /// @param[in] node 対象のノード
  /// @param[in] vid_map 変数番号のマップ
  VidLitMap(const TpgNode* node,
	    const VidMap& vid_map);

  /// @brief デストラクタ
  virtual
  ~VidLitMap();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力数を返す．
  virtual
  ymuint
  input_size() const;

  /// @brief 入力のリテラルを返す．
  /// @param[in] pos 入力位置 ( 0 <= pos < input_size() )
  virtual
  Literal
  input(ymuint pos) const;

  /// @brief 出力のリテラルを返す．
  virtual
  Literal
  output() const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ノード
  const TpgNode* mNode;

  // 変数番号のマップ
  const VidMap& mVidMap;

};

END_NAMESPACE_YM_SATPG

#endif // VIDLITMAP_H
