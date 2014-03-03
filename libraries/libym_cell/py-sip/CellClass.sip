
/// @file CellClass.sip
/// @brief CellClass の sip ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.

class CellClass /Abstract/
{
%TypeHeaderCode
#include "cell/CellClass.h"
using nsYm::CellClass;
%End

public:
  //////////////////////////////////////////////////////////////////////
  // 一般的な情報を取得する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ID番号を返す．
  /// @note CellLibrary::npn_class(id) で返されるオブジェクトの id() は id となる．
  unsigned int
  id() const;

  /// @brief 同位体変換の個数を得る．
  /// @note 恒等変換は含まない．
  unsigned int
  idmap_num() const;

  /// @brief 同位体変換を得る．
  /// @param[in] pos 位置番号 ( 0 <= pos < idmap_num() )
  const NpnMapM&
  idmap(unsigned int pos) const;


public:
  //////////////////////////////////////////////////////////////////////
  // このクラスに属しているセルグループの情報を取得する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief CellGroup の数を返す．
  unsigned int
  group_num() const;

  /// @brief CellGroup を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < func_num() )
  const CellGroup*
  cell_group(unsigned int pos) const;


private:

  ~CellClass();

};