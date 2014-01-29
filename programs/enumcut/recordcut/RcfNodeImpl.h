#ifndef RCFNODEIMPL_H
#define RCFNODEIMPL_H

/// @file RcfNodeImpl.h
/// @brief RcfNodeImpl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "RcfNode.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class RcfNodeImpl RcfNodeImpl.h "RcfNodeImpl.h"
/// @brief RcfNode の実装クラス
//////////////////////////////////////////////////////////////////////
class RcfNodeImpl :
  public RcfNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] id ID番号
  RcfNodeImpl(ymuint id);

  /// @brief デストラクタ
  virtual
  ~RcfNodeImpl();


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
  RcfNodeHandle
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
/// @class RcfInputNode RcfNodeImpl.h "RcfNodeImpl.h"
/// @brief 外部入力ノードを表すクラス
//////////////////////////////////////////////////////////////////////
class RcfInputNode :
  public RcfNodeImpl
{
public:

  /// @brief コンストラクタ
  /// @param[in] id ID番号
  /// @param[in] input_id 入力番号
  RcfInputNode(ymuint id,
	       ymuint input_id);

  /// @brief デストラクタ
  virtual
  ~RcfInputNode();


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

  /// @brief 関数を計算する．
  /// @param[in] func_array ノード番号をキーにして関数を格納した配列
  /// @param[in] conf_bits 設定変数番号をキーにして値を格納した配列
  virtual
  TvFunc
  calc_func(const vector<TvFunc>& func_array,
	    const vector<bool>& conf_bits) const;

  /// @brief 値を計算する．
  /// @param[in] val_array ノード番号をキーにして値を格納した配列
  /// @param[in] conf_bits 設定変数番号をキーにして値を格納した配列
  virtual
  bool
  simulate(const vector<bool>& val_array,
	   const vector<bool>& conf_bits) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 入力番号
  ymuint32 mInputId;

};


//////////////////////////////////////////////////////////////////////
/// @class RcfAndNode RcfNodeImpl.h "RcfNodeImpl.h"
/// @brief AND ノードを表すクラス
//////////////////////////////////////////////////////////////////////
class RcfAndNode :
  public RcfNodeImpl
{
public:

  /// @brief コンストラクタ
  /// @param[in] id ID番号
  /// @param[in] fanin0, fanin1 ファンインのハンドル
  RcfAndNode(ymuint id,
	     RcfNodeHandle fanin0,
	     RcfNodeHandle fanin1);

  /// @brief デストラクタ
  virtual
  ~RcfAndNode();


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
  RcfNodeHandle
  fanin(ymuint pos) const;

  /// @brief 関数を計算する．
  /// @param[in] func_array ノード番号をキーにして関数を格納した配列
  /// @param[in] conf_bits 設定変数番号をキーにして値を格納した配列
  virtual
  TvFunc
  calc_func(const vector<TvFunc>& func_array,
	    const vector<bool>& conf_bits) const;

  /// @brief 値を計算する．
  /// @param[in] val_array ノード番号をキーにして値を格納した配列
  /// @param[in] conf_bits 設定変数番号をキーにして値を格納した配列
  virtual
  bool
  simulate(const vector<bool>& val_array,
	   const vector<bool>& conf_bits) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファンインの配列
  RcfNodeHandle mFanin[2];

};


//////////////////////////////////////////////////////////////////////
/// @class RcfLutNode RcfNodeImpl.h "RcfNodeImpl.h"
/// @brief LUT ノードを表すクラス
//////////////////////////////////////////////////////////////////////
class RcfLutNode :
  public RcfNodeImpl
{
public:

  /// @brief コンストラクタ
  /// @param[in] id ID番号
  /// @param[in] conf_base configuration 変数の基底
  /// @param[in] fanin_list ファンインのリスト
  RcfLutNode(ymuint id,
	     ymuint conf_base,
	     const vector<RcfNodeHandle>& fanin_list);

  /// @brief デストラクタ
  virtual
  ~RcfLutNode();


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
  RcfNodeHandle
  fanin(ymuint pos) const;

  /// @brief LUT/MUX ノードの時の configuration 変数の最初の番号を得る．
  virtual
  ymuint
  conf_base() const;

  /// @brief LUT/MUX ノードの時の configuration 変数の数を得る．
  virtual
  ymuint
  conf_size() const;

  /// @brief 関数を計算する．
  /// @param[in] func_array ノード番号をキーにして関数を格納した配列
  /// @param[in] conf_bits 設定変数番号をキーにして値を格納した配列
  virtual
  TvFunc
  calc_func(const vector<TvFunc>& func_array,
	    const vector<bool>& conf_bits) const;

  /// @brief 値を計算する．
  /// @param[in] val_array ノード番号をキーにして値を格納した配列
  /// @param[in] conf_bits 設定変数番号をキーにして値を格納した配列
  virtual
  bool
  simulate(const vector<bool>& val_array,
	   const vector<bool>& conf_bits) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // configuration 変数の基底
  ymuint32 mConfBase;

  // ファンイン数
  ymuint32 mFaninNum;

  // ファンインの配列
  RcfNodeHandle mFanin[1];

};


//////////////////////////////////////////////////////////////////////
/// @class RcfMuxNode RcfNodeImpl.h "RcfNodeImpl.h"
/// @brief MUX ノードを表すクラス
//////////////////////////////////////////////////////////////////////
class RcfMuxNode :
  public RcfNodeImpl
{
public:

  /// @brief コンストラクタ
  /// @param[in] id ID番号
  /// @param[in] conf_base configuration 変数の基底
  /// @param[in] fanin_list ファンインのリスト
  RcfMuxNode(ymuint id,
	     ymuint conf_base,
	     const vector<RcfNodeHandle>& fanin_list);

  /// @brief デストラクタ
  virtual
  ~RcfMuxNode();


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
  RcfNodeHandle
  fanin(ymuint pos) const;

  /// @brief LUT/MUX ノードの時の configuration 変数の最初の番号を得る．
  virtual
  ymuint
  conf_base() const;

  /// @brief LUT/MUX ノードの時の configuration 変数の数を得る．
  virtual
  ymuint
  conf_size() const;

  /// @brief 関数を計算する．
  /// @param[in] func_array ノード番号をキーにして関数を格納した配列
  /// @param[in] conf_bits 設定変数番号をキーにして値を格納した配列
  virtual
  TvFunc
  calc_func(const vector<TvFunc>& func_array,
	    const vector<bool>& conf_bits) const;

  /// @brief 値を計算する．
  /// @param[in] val_array ノード番号をキーにして値を格納した配列
  /// @param[in] conf_bits 設定変数番号をキーにして値を格納した配列
  virtual
  bool
  simulate(const vector<bool>& val_array,
	   const vector<bool>& conf_bits) const;


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
  RcfNodeHandle mFanin[1];

};

END_NAMESPACE_YM

#endif // RCFNODEIMPL_H
