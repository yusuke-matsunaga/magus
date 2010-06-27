#ifndef LIBYM_MVN_MVCONCAT_H
#define LIBYM_MVN_MVCONCAT_H

/// @file libym_mvn/MvConcat.h
/// @brief MvConcat のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_mvn/MvNode.h"
#include "ym_mvn/MvPin.h"


BEGIN_NAMESPACE_YM_MVN

//////////////////////////////////////////////////////////////////////
/// @class MvConcat MvConcat.h "MvConcat.h"
/// @brief 連結演算を表すノード
//////////////////////////////////////////////////////////////////////
class MvConcat :
  public MvNode
{
  friend class MvMgr;
public:

  /// @brief ノードの種類を得る．
  virtual
  tType
  type() const;

  /// @brief 入力ピン数を得る．
  virtual
  ymuint
  input_num() const;

  /// @brief 出力ピン数を得る．
  virtual
  ymuint
  output_num() const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力ピンを得る．
  /// @param[in] pos 位置 ( 0 <= pos < input_num() )
  virtual
  MvInputPin*
  input(ymuint pos);

  /// @brief 出力ピンを得る．
  /// @param[in] pos 位置 ( 0 <= pos < output_num() )
  virtual
  MvOutputPin*
  output(ymuint pos);


private:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ / デストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ
  /// @param[in] module 親のモジュール
  /// @param[in] ni 入力数
  MvConcat(MvModule* module,
	   ymuint ni);

  /// @brief デストラクタ
  ~MvConcat();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 入力数
  ymuint mNi;
  
  // 入力ピン
  MvInputPin* mInput;
  
  // 出力ピン
  MvOutputPin mOutput;

};

END_NAMESPACE_YM_MVN

#endif // LIBYM_MVN_MVCONCAT_H
