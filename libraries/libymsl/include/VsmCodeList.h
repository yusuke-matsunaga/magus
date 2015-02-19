#ifndef VSMCODELIST_H
#define VSMCODELIST_H

/// @file VsmCodeList.h
/// @brief VsmCodeList のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ymsl_int.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class VsmCodeList VsmCodeList.h "VsmCodeList.h"
/// @brief VSM用の命令コードのリスト
///
/// VsmCodeList のオブジェクトを作るにはまず
/// VsmCodeList::Builder オブジェクトに対して
/// write_opcode(), write_int(), write_float()
/// などの関数でコードの中身を作り，それを
/// VsmCodeList のコンストラクタの引数として渡す．
/// なので VsmCodeList は const の読み出し専用関数
/// しか持たない．
//////////////////////////////////////////////////////////////////////
class VsmCodeList
{
public:

  /// @brief 初期化用の builder クラス
  class Builder
  {
  public:

    /// @brief コンストラクタ
    Builder();

    /// @brief デストラクタ
    ~Builder();


  public:
    //////////////////////////////////////////////////////////////////////
    // 外部インターフェイス
    //////////////////////////////////////////////////////////////////////

    /// @brief 命令を追加する．
    /// @param[in] op 命令
    void
    write_opcode(Ymsl_CODE op);

    /// @brief INT を追加する
    /// @param[in] val 値
    void
    write_int(Ymsl_INT val);

    /// @brief FLOAT を追加する．
    /// @param[in] val 値
    void
    write_float(Ymsl_FLOAT val);

    /// @brief OBJPTR を追加する．
    /// @param[in] val 値
    void
    write_objptr(Ymsl_OBJPTR val);

    /// @brief サイズを得る．
    Ymsl_INT
    size() const;

    /// @brief 命令を読みだす．
    /// @param[in] addr アドレス
    /// @return 読みだした値を返す．
    Ymsl_CODE
    read_opcode(Ymsl_INT addr) const;


  private:
    //////////////////////////////////////////////////////////////////////
    // データメンバ
    //////////////////////////////////////////////////////////////////////

    // コード
    vector<Ymsl_CODE> mBody;

  };


public:

  /// @brief コンストラクタ
  /// @param[in] builder 初期化用オブジェクト
  VsmCodeList(const Builder& builder);

  /// @brief デストラクタ
  ~VsmCodeList();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief サイズを得る．
  Ymsl_INT
  size() const;

  /// @brief 命令を読みだす．
  /// @param[inout] addr アドレス
  /// @return 読みだした値を返す．
  ///
  /// addr は読み込んだ分進む
  Ymsl_CODE
  read_opcode(Ymsl_INT& addr) const;

  /// @brief INT を読みだす．
  /// @param[inout] addr アドレス
  /// @return 読みだした値を返す．
  ///
  /// addr は読み込んだ分進む
  Ymsl_INT
  read_int(Ymsl_INT& addr) const;

  /// @brief FLOAT を読みだす．
  /// @param[inout] addr アドレス
  /// @return 読みだした値を返す．
  ///
  /// addr は読み込んだ分進む
  Ymsl_FLOAT
  read_float(Ymsl_INT& addr) const;

  /// @brief OBJPTR を読みだす．
  /// @param[inout] addr アドレス
  /// @return 読みだした値を返す．
  ///
  /// addr は読み込んだ分進む
  Ymsl_OBJPTR
  read_objptr(Ymsl_INT& addr) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // サイズ
  ymuint mSize;

  // 実体
  Ymsl_CODE* mBody;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief サイズを得る．
inline
Ymsl_INT
VsmCodeList::size() const
{
  return mSize;
}

// @brief 命令を読みだす．
// @param[inout] addr アドレス
// @return 読みだした値を返す．
inline
Ymsl_CODE
VsmCodeList::read_opcode(Ymsl_INT& addr) const
{
  Ymsl_CODE op = mBody[addr];
  ++ addr;
  return op;
}

// @brief INT を読みだす．
// @param[inout] addr アドレス
// @return 読みだした値を返す．
inline
Ymsl_INT
VsmCodeList::read_int(Ymsl_INT& addr) const
{
  ASSERT_COND( sizeof(Ymsl_INT) == sizeof(Ymsl_CODE) );
  Ymsl_INT val = mBody[addr];
  ++ addr;
  return val;
}

// @brief FLOAT を読みだす．
// @param[inout] addr アドレス
// @return 読みだした値を返す．
inline
Ymsl_FLOAT
VsmCodeList::read_float(Ymsl_INT& addr) const
{
  Ymsl_CODE* c_ptr = mBody + addr;
  Ymsl_FLOAT* f_ptr = reinterpret_cast<Ymsl_FLOAT*>(c_ptr);
  Ymsl_FLOAT val = *f_ptr;
  addr += sizeof(Ymsl_FLOAT) / sizeof(Ymsl_CODE);
  return val;
}

// @brief OBJPTR を読みだす．
// @param[inout] addr アドレス
// @return 読みだした値を返す．
inline
Ymsl_OBJPTR
VsmCodeList::read_objptr(Ymsl_INT& addr) const
{
  Ymsl_CODE* c_ptr = mBody + addr;
  Ymsl_OBJPTR* o_ptr = reinterpret_cast<Ymsl_OBJPTR*>(c_ptr);
  Ymsl_OBJPTR val = *o_ptr;
  addr += sizeof(Ymsl_OBJPTR) / sizeof(Ymsl_CODE);
  return val;
}

END_NAMESPACE_YM_YMSL


#endif // VSMCODELIST_H
