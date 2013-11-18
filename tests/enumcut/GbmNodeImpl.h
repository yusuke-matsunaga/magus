#ifndef GBMNODEIMPL_H
#define GBMNODEIMPL_H

/// @file GbmNodeImpl.h
/// @brief GbmNodeImpl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "GbmNode.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class GbmNodeImpl GbmNodeImpl.h "GbmNodeImpl.h"
/// @brief GbmNode の実装クラス
//////////////////////////////////////////////////////////////////////
class GbmNodeImpl :
  public GbmNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] id ID番号
  GbmNodeImpl(ymuint id);

  /// @brief デストラクタ
  virtual
  ~GbmNodeImpl();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ID番号を返す．
  virtual
  ymuint
  id() const;

  /// @brief 外部入力ノードの時 true を返す．
  virtual
  bool
  is_input() const;

  /// @brief ANDノードの時 true を返す．
  virtual
  bool
  is_and() const;

  /// @brief LUTノードの時 true を返す．
  virtual
  bool
  is_lut() const;

  /// @brief MUX ノードの時 true を返す．
  virtual
  bool
  is_mux() const;

  /// @brief 外部入力番号を返す．
  /// @note is_input() == true の時のみ意味を持つ．
  virtual
  ymuint
  input_id() const;

  /// @brief ファンイン数を返す．
  /// @note 外部入力ノードの場合は常に0
  /// @note AND ノードの場合は常に2
  virtual
  ymuint
  fanin_num() const;

  /// @brief ファンインのハンドルを返す．
  /// @param[in] pos ファンイン番号 ( 0 <= pos < fanin_num() )
  virtual
  GbmNodeHandle
  fanin(ymuint pos) const;

  /// @brief LUT/MUX ノードの時の configuration 変数の最初の番号を得る．
  virtual
  ymuint
  conf_base() const;

  /// @brief LUT/MUX ノードの時の configuration 変数の数を得る．
  virtual
  ymuint
  conf_size() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ID番号
  ymuint32 mId;

};


//////////////////////////////////////////////////////////////////////
/// @class GbmInputNode GbmNodeImpl.h "GbmNodeImpl.h"
/// @brief 外部入力ノードを表すクラス
//////////////////////////////////////////////////////////////////////
class GbmInputNode :
  public GbmNodeImpl
{
public:

  /// @brief コンストラクタ
  /// @param[in] id ID番号
  /// @param[in] input_id 入力番号
  GbmInputNode(ymuint id,
	       ymuint input_id);

  /// @brief デストラクタ
  virtual
  ~GbmInputNode();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 外部入力ノードの時 true を返す．
  virtual
  bool
  is_input() const;

  /// @brief 外部入力番号を返す．
  /// @note is_input() == true の時のみ意味を持つ．
  virtual
  ymuint
  input_id() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 入力番号
  ymuint32 mInputId;

};


//////////////////////////////////////////////////////////////////////
/// @class GbmAndNode GbmNodeImpl.h "GbmNodeImpl.h"
/// @brief AND ノードを表すクラス
//////////////////////////////////////////////////////////////////////
class GbmAndNode :
  public GbmNodeImpl
{
public:

  /// @brief コンストラクタ
  /// @param[in] id ID番号
  /// @param[in] fanin0, fanin1 ファンインのハンドル
  GbmAndNode(ymuint id,
	     GbmNodeHandle fanin0,
	     GbmNodeHandle fanin1);

  /// @brief デストラクタ
  virtual
  ~GbmAndNode();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ANDノードの時 true を返す．
  virtual
  bool
  is_and() const;

  /// @brief ファンイン数を返す．
  /// @note 外部入力ノードの場合は常に0
  /// @note AND ノードの場合は常に2
  virtual
  ymuint
  fanin_num() const;

  /// @brief ファンインのハンドルを返す．
  /// @param[in] pos ファンイン番号 ( 0 <= pos < fanin_num() )
  virtual
  GbmNodeHandle
  fanin(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファンインの配列
  GbmNodeHandle mFanin[2];

};


//////////////////////////////////////////////////////////////////////
/// @class GbmLutNode GbmNodeImpl.h "GbmNodeImpl.h"
/// @brief LUT ノードを表すクラス
//////////////////////////////////////////////////////////////////////
class GbmLutNode :
  public GbmNodeImpl
{
public:

  /// @brief コンストラクタ
  /// @param[in] id ID番号
  /// @param[in] conf_base configuration 変数の基底
  /// @param[in] fanin_list ファンインのリスト
  GbmLutNode(ymuint id,
	     ymuint conf_base,
	     const vector<GbmNodeHandle>& fanin_list);

  /// @brief デストラクタ
  virtual
  ~GbmLutNode();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief LUTノードの時 true を返す．
  virtual
  bool
  is_lut() const;

  /// @brief ファンイン数を返す．
  /// @note 外部入力ノードの場合は常に0
  /// @note AND ノードの場合は常に2
  virtual
  ymuint
  fanin_num() const;

  /// @brief ファンインのハンドルを返す．
  /// @param[in] pos ファンイン番号 ( 0 <= pos < fanin_num() )
  virtual
  GbmNodeHandle
  fanin(ymuint pos) const;

  /// @brief LUT/MUX ノードの時の configuration 変数の最初の番号を得る．
  virtual
  ymuint
  conf_base() const;

  /// @brief LUT/MUX ノードの時の configuration 変数の数を得る．
  virtual
  ymuint
  conf_size() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // configuration 変数の基底
  ymuint32 mConfBase;

  // ファンイン数
  ymuint32 mFaninNum;

  // ファンインの配列
  GbmNodeHandle mFanin[1];

};


//////////////////////////////////////////////////////////////////////
/// @class GbmMuxNode GbmNodeImpl.h "GbmNodeImpl.h"
/// @brief MUX ノードを表すクラス
//////////////////////////////////////////////////////////////////////
class GbmMuxNode :
  public GbmNodeImpl
{
public:

  /// @brief コンストラクタ
  /// @param[in] id ID番号
  /// @param[in] conf_base configuration 変数の基底
  /// @param[in] fanin_list ファンインのリスト
  GbmMuxNode(ymuint id,
	     ymuint conf_base,
	     const vector<GbmNodeHandle>& fanin_list);

  /// @brief デストラクタ
  virtual
  ~GbmMuxNode();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief MUX ノードの時 true を返す．
  virtual
  bool
  is_mux() const;

  /// @brief ファンイン数を返す．
  /// @note 外部入力ノードの場合は常に0
  /// @note AND ノードの場合は常に2
  virtual
  ymuint
  fanin_num() const;

  /// @brief ファンインのハンドルを返す．
  /// @param[in] pos ファンイン番号 ( 0 <= pos < fanin_num() )
  virtual
  GbmNodeHandle
  fanin(ymuint pos) const;

  /// @brief LUT/MUX ノードの時の configuration 変数の最初の番号を得る．
  virtual
  ymuint
  conf_base() const;

  /// @brief LUT/MUX ノードの時の configuration 変数の数を得る．
  virtual
  ymuint
  conf_size() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // configuration 変数の基底
  ymuint32 mConfBase;

  // configuration 変数の数．
  ymuint32 mConfSize;

  // ファンイン数
  ymuint32 mFaninNum;

  // ファンインの配列
  GbmNodeHandle mFanin[1];

};

END_NAMESPACE_YM

#endif // GBMNODEIMPL_H
