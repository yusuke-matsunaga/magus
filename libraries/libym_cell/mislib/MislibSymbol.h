#ifndef LIBYM_MISLIB_MISLIBSYMBOL_H
#define LIBYM_MISLIB_MISLIBSYMBOL_H

/// @file libym_mislib/MislibSymbol.h
/// @brief MislibNoninv, MislibInv, MislibUnknown のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "MislibNodeImpl.h"


BEGIN_NAMESPACE_YM_MISLIB

//////////////////////////////////////////////////////////////////////
/// @class MislibNoninv MislibSymbol.h "MislibSymbol.h"
/// @brief NONINV 極性を表すクラス
//////////////////////////////////////////////////////////////////////
class MislibNoninv :
  public MislibNodeImpl
{
  friend class MislibMgrImpl;

private:

  /// @brief コンストラクタ
  /// @param[in] loc 位置情報
  MislibNoninv(const FileRegion& loc);

  /// @brief デストラクタ
  virtual
  ~MislibNoninv();


public:

  /// @brief 種類を取り出す．
  virtual
  tType
  type() const;

  /// @brief 内容を出力する．
  /// デバッグ用
  virtual
  void
  dump(ostream& s) const;

};


//////////////////////////////////////////////////////////////////////
/// @class MislibInv MislibSymbol.h "MislibSymbol.h"
/// @brief INV 極性を表すクラス
//////////////////////////////////////////////////////////////////////
class MislibInv :
  public MislibNodeImpl
{
  friend class MislibMgrImpl;

private:

  /// @brief コンストラクタ
  /// @param[in] loc 位置情報
  MislibInv(const FileRegion& loc);

  /// @brief デストラクタ
  virtual
  ~MislibInv();


public:

  /// @brief 種類を取り出す．
  virtual
  tType
  type() const;

  /// @brief 内容を出力する．
  /// デバッグ用
  virtual
  void
  dump(ostream& s) const;

};


//////////////////////////////////////////////////////////////////////
/// @class MislibUnknown MislibSymbol.h "MislibSymbol.h"
/// @brief UNKNOWN 極性を表すクラス
//////////////////////////////////////////////////////////////////////
class MislibUnknown :
  public MislibNodeImpl
{
  friend class MislibMgrImpl;

private:

  /// @brief コンストラクタ
  /// @param[in] loc 位置情報
  MislibUnknown(const FileRegion& loc);

  /// @brief デストラクタ
  virtual
  ~MislibUnknown();


public:

  /// @brief 種類を取り出す．
  virtual
  tType
  type() const;

  /// @brief 内容を出力する．
  /// デバッグ用
  virtual
  void
  dump(ostream& s) const;

};

END_NAMESPACE_YM_MISLIB

#endif // LIBYM_MISLIB_MISLIBSYMBOL_H
