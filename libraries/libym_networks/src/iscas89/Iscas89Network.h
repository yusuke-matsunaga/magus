#ifndef ISCAS89NETWORK_H
#define ISCAS89NETWORK_H

/// @file Iscas89Network.h
/// @brief Iscas89Network のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "iscas89_nsdef.h"
#include "utils/SimpleAlloc.h"


BEGIN_NAMESPACE_YM_ISCAS89

class Iscas89NetworkHandler;

//////////////////////////////////////////////////////////////////////
/// @class Iscas89Node Iscas89Network.h "Iscas89Network.h"
/// @brief iscas89 ファイルの表すネットワークのノード
/// @sa Iscas89Network
//////////////////////////////////////////////////////////////////////
class Iscas89Node
{
  friend class Iscas89Network;
  friend class Iscas89NetworkHandler;

public:

  /// @brief ノードの種類
  enum tType {
    /// @brief 未定義
    kUndef,
    /// @brief 外部入力
    kInput,
    /// @brief ゲート(DFFも含む)
    kGate
  };


private:

  /// @brief コンストラクタ
  /// @param[in] id ID 番号
  Iscas89Node(ymuint32 id);

  /// @brief デストラクタ
  ~Iscas89Node();


public:

  /// @brief ID を返す．
  ymuint32
  id() const;

  /// @brief 名前を返す．
  const char*
  name() const;

  /// @brief 型を返す．
  tType
  type() const;

  /// @brief ゲート型を返す．
  tIscas89GateType
  gate_type() const;

  /// @brief ファンイン数を得る．
  ymuint32
  fanin_num() const;

  /// @brief ファンインを求める．
  const Iscas89Node*
  fanin(ymuint32 pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  // ここのメモリ領域はすべて Iscas89Network::mAlloc が管理する．
  //////////////////////////////////////////////////////////////////////

  // ID 番号
  ymuint32 mId;

  // 名前
  const char* mName;

  // 型
  tType mType;

  // ゲートの型
  tIscas89GateType mGateType;

  // ファンイン数
  ymuint32 mNi;

  // ファンインの配列
  Iscas89Node** mFanins;

};


//////////////////////////////////////////////////////////////////////
/// @class Iscas89Network Iscas89Network.h "Iscas89Network.h"
/// @brief iscas89 ファイルの表しているネットワーク
//////////////////////////////////////////////////////////////////////
class Iscas89Network
{
  friend class Iscas89NetworkHandler;

public:

  /// @brief コンストラクタ
  Iscas89Network();

  /// @brief デストラクタ
  ~Iscas89Network();


public:

  /// @brief ノードの ID番号の最大値 + 1 を求める．
  ymuint32
  max_node_id() const;

  /// @brief ID番号からノードを得る．
  /// @param[in] id ID番号 ( 0 <= id < max_node_id() )
  /// @note 使われていない ID の場合には NULL が返される．
  const Iscas89Node*
  node(ymuint32 id) const;

  /// @brief 外部入力数を得る．
  ymuint32
  npi() const;

  /// @brief 外部入力を得る．
  /// @param[in] pos 位置番号 ( 0 <= pos < npi() )
  const Iscas89Node*
  pi(ymuint32 pos) const;

  /// @brief 外部出力数を得る．
  ymuint32
  npo() const;

  /// @brief 外部出力を得る．
  /// @param[in] pos 位置番号 ( 0 <= pos < npo() )
  const Iscas89Node*
  po(ymuint32 pos) const;

  /// @brief ラッチ数を得る．
  ymuint32
  nff() const;

  /// @brief ラッチを得る．
  /// @param[in] pos 位置番号 ( 0 <= pos < nff() )
  const Iscas89Node*
  ff(ymuint32 pos) const;

  /// @brief ゲート数を得る．
  ymuint32
  ngate() const;

  /// @brief ゲートを得る．
  /// @param[in] pos 位置番号 ( 0 <= pos < ngate() )
  const Iscas89Node*
  gate(ymuint32 pos) const;

  /// @brief 内容を iscas89 形式で出力する．
  /// @param[in] s 出力先のストリーム
  void
  write_iscas89(ostream& s) const;


private:
  //////////////////////////////////////////////////////////////////////
  // Iscas89NetworkHandler のみが用いる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容をクリアする．
  void
  clear();

  /// @brief ノードを取り出す．
  /// @param[in] id ID番号
  /// @note なければ作成する．
  Iscas89Node*
  get_node(ymuint32 id);

  /// @brief ノード名を設定する．
  /// @param[in] node 対象のノード
  /// @param[in] name 名前
  void
  set_node_name(Iscas89Node* node,
		const char* name);

  /// @brief ノードを外部入力に設定する．
  /// @param[in] node 対象のノード
  /// @note 既に他の型に設定されていたら false を返す．
  bool
  set_input_type(Iscas89Node* node);

  /// @brief ノードをゲートに設定する．
  /// @param[in] node 対象のノード
  /// @param[in] ni ファンイン数
  /// @param[in] type 型
  /// @note すでに他の型に設定されていたら false を返す．
  bool
  set_gate_type(Iscas89Node* node,
		ymuint32 ni,
		tIscas89GateType gate_type);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // メモリアロケータ
  SimpleAlloc mAlloc;

  // ID をキーにしてノードを収めた配列
  // ID は穴が空いているかもしれない．
  vector<Iscas89Node*> mNodeArray;

  // 外部入力の配列
  vector<Iscas89Node*> mPIArray;

  // 外部出力の配列
  vector<Iscas89Node*> mPOArray;

  // ラッチの配列
  vector<Iscas89Node*> mFFArray;

  // ゲートの配列
  vector<Iscas89Node*> mGateArray;

};


//////////////////////////////////////////////////////////////////////
// Iscas89Node のインライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief ID を返す．
inline
ymuint32
Iscas89Node::id() const
{
  return mId;
}

// @brief 名前を返す．
inline
const char*
Iscas89Node::name() const
{
  return mName;
}

// @brief 型を返す．
inline
Iscas89Node::tType
Iscas89Node::type() const
{
  return mType;
}

// @brief ゲート型を返す．
inline
tIscas89GateType
Iscas89Node::gate_type() const
{
  return mGateType;
}

// @brief ファンイン数を得る．
inline
ymuint32
Iscas89Node::fanin_num() const
{
  return mNi;
}

// @brief ファンインを求める．
inline
const Iscas89Node*
Iscas89Node::fanin(ymuint32 pos) const
{
  return mFanins[pos];
}


//////////////////////////////////////////////////////////////////////
// Iscas89Network のインライン関数
//////////////////////////////////////////////////////////////////////

// @brief ノードの ID番号の最大値 + 1 を求める．
inline
ymuint32
Iscas89Network::max_node_id() const
{
  return mNodeArray.size();
}

// @brief ID番号からノードを得る．
// @param[in] id ID番号 ( 0 <= id < max_node_id() )
// @note 使われていない ID の場合には NULL が返される．
inline
const Iscas89Node*
Iscas89Network::node(ymuint32 id) const
{
  return mNodeArray[id];
}

// @brief 外部入力数を得る．
inline
ymuint32
Iscas89Network::npi() const
{
  return mPIArray.size();
}

// @brief 外部入力を得る．
// @param[in] pos 位置番号 ( 0 <= pos < npi() )
inline
const Iscas89Node*
Iscas89Network::pi(ymuint32 pos) const
{
  return mPIArray[pos];
}

// @brief 外部出力数を得る．
inline
ymuint32
Iscas89Network::npo() const
{
  return mPOArray.size();
}

// @brief 外部出力を得る．
// @param[in] pos 位置番号 ( 0 <= pos < npo() )
inline
const Iscas89Node*
Iscas89Network::po(ymuint32 pos) const
{
  return mPOArray[pos];
}

// @brief ラッチ数を得る．
inline
ymuint32
Iscas89Network::nff() const
{
  return mFFArray.size();
}

// @brief ラッチを得る．
// @param[in] pos 位置番号 ( 0 <= pos < nff() )
inline
const Iscas89Node*
Iscas89Network::ff(ymuint32 pos) const
{
  return mFFArray[pos];
}

// @brief ゲート数を得る．
inline
ymuint32
Iscas89Network::ngate() const
{
  return mGateArray.size();
}

// @brief ゲートを得る．
// @param[in] pos 位置番号 ( 0 <= pos < ngate() )
inline
const Iscas89Node*
Iscas89Network::gate(ymuint32 pos) const
{
  return mGateArray[pos];
}

END_NAMESPACE_YM_ISCAS89

#endif // ISCAS89NETWORK_H
