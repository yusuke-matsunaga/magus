#ifndef TPGMAP_H
#define TPGMAP_H

/// @file TpgMap.h
/// @brief TpgMap のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "satpg_nsdef.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class TpgMap TpgMap.h "TpgMap.h"
/// @brief TgNode と TpgNode の対応付けを表すクラス
///
/// 基本的には一つの TgNode に一つの TpgNode が対応するが，
/// TgNode のタイプが cplx_logic だった場合には複数の TpgNode で構成
/// されることもある．
/// その場合には，個々の入力ごとに対応する TpgNode が異なることになる．
/// この２つの場合に対応するため仮想関数を用いる．
//////////////////////////////////////////////////////////////////////
class TpgMap
{
public:

  /// @brief デストラクタ
  virtual
  ~TpgMap() { }


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力に対応するノードを得る．
  /// @param[in] ipos 入力の位置番号
  virtual
  TpgNode*
  input(ymuint ipos) const = 0;

  /// @brief 入力に対応するノードのファンイン番号を得る．
  /// @param[in] ipos 入力の位置番号
  virtual
  ymuint
  input_pos(ymuint ipos) const = 0;

};


//////////////////////////////////////////////////////////////////////
/// @class TpgPrimMap TpgMap.h "TpgMap.h"
/// @brief 単純なノード用の TpgMap;
//////////////////////////////////////////////////////////////////////
class TpgPrimMap :
  public TpgMap
{
public:

  /// @brief コンストラクタ
  TpgPrimMap(TpgNode* node);

  /// @brief デストラクタ
  virtual
  ~TpgPrimMap();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力に対応するノードを得る．
  /// @param[in] ipos 入力の位置番号
  virtual
  TpgNode*
  input(ymuint ipos) const;

  /// @brief 入力に対応するノードのファンイン番号を得る．
  /// @param[in] ipos 入力の位置番号
  virtual
  ymuint
  input_pos(ymuint ipos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 根のノード
  TpgNode* mNode;

};


//////////////////////////////////////////////////////////////////////
/// @class TpgCplxMap TpgMap.h "TpgMap.h"
/// @brief 複数のノードとの対応づけを表す TpgMap の派生クラス
//////////////////////////////////////////////////////////////////////
class TpgCplxMap :
  public TpgMap
{
  friend class TpgNetwork;

public:

  /// @brief コンストラクタ
  TpgCplxMap();

  /// @brief デストラクタ
  virtual
  ~TpgCplxMap();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力に対応するノードを得る．
  /// @param[in] ipos 入力の位置番号
  virtual
  TpgNode*
  input(ymuint ipos) const;

  /// @brief 入力に対応するノードのファンイン番号を得る．
  /// @param[in] ipos 入力の位置番号
  virtual
  ymuint
  input_pos(ymuint ipos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 入力のノードの配列
  TpgNode** mInputNodeArray;

  // 入力のファンイン番号の配列
  ymuint32* mInputPosArray;

};

END_NAMESPACE_YM_SATPG

#endif // TPGMAP_H
