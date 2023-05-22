#ifndef LIBYM_SEAL_IDXMAPPER_H
#define LIBYM_SEAL_IDXMAPPER_H

/// @file libym_seal/IdxMapper.h
/// @brief fsm_analysis の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "seal_nsdef.h"

BEGIN_NAMESPACE_YM_SEAL

//////////////////////////////////////////////////////////////////////
/// @class IdxMapper IdxMapper.h "IdxMapper.h"
/// @brief BDD用の変数インデックスを計算するクラス
//////////////////////////////////////////////////////////////////////
class IdxMapper
{
public:

  /// @brief コンストラクタ
  /// @param[in] input_num 外部入力数
  /// @param[in] output_num 外部出力数
  /// @param[in] ff_num FF数
  IdxMapper(ymuint input_num,
	    ymuint output_num,
	    ymuint ff_num);

  /// @brief デストラクタ
  ~IdxMapper();


public:

  /// @brief 外部入力の変数インデックスを得る．
  VarId
  input_idx(ymuint pos) const;

  /// @brief 外部出力の変数インデックスを得る．
  VarId
  output_idx(ymuint pos) const;

  /// @brief 正常回路の現状態の変数インデックスを得る．
  VarId
  cur_normal_idx(ymuint pos) const;

  /// @brief 正常回路の次状態の変数インデックスを得る．
  VarId
  next_normal_idx(ymuint pos) const;

  /// @brief 故障回路の現状態の変数インデックスを得る．
  VarId
  cur_error_idx(ymuint pos) const;

  /// @brief 故障回路の次状態の変数インデックスを得る．
  VarId
  next_error_idx(ymuint pos) const;

  /// @brief 現状態のエラービット
  VarId
  cur_error_bit() const;

  /// @brief 次状態のエラービット
  VarId
  next_error_bit() const;

  /// @brief 現状態の等価状態ビット
  VarId
  cur_ident_bit() const;

  /// @brief 次状態の等価状態ビット
  VarId
  next_ident_bit() const;

  /// @brief 状態からエラーを含んだ状態対を作る．
  State
  make_error_state(State normal_state,
		   ymuint error_pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 外部入力数
  std::uint32_t mInputNum;

  // 外部出力数
  std::uint32_t mOutputNum;

  // FF数
  std::uint32_t mFFNum;

};


// @brief コンストラクタ
// @param[in] input_num 外部入力数
// @param[in] output_num 外部出力数
// @param[in] ff_num FF数
inline
IdxMapper::IdxMapper(ymuint input_num,
		     ymuint output_num,
		     ymuint ff_num) :
  mInputNum(input_num),
  mOutputNum(output_num),
  mFFNum(ff_num)
{
}

// @brief デストラクタ
inline
IdxMapper::~IdxMapper()
{
}

// @brief 外部入力の変数インデックスを得る．
inline
VarId
IdxMapper::input_idx(ymuint pos) const
{
  return VarId(mOutputNum + mFFNum * 4 + 4 + pos);
}

// @brief 外部出力の変数インデックスを得る．
inline
VarId
IdxMapper::output_idx(ymuint pos) const
{
  return VarId(pos);
}

// @brief 正常回路の現状態の変数インデックスを得る．
inline
VarId
IdxMapper::cur_normal_idx(ymuint pos) const
{
  return VarId(mOutputNum + pos * 2 + 0);
}

// @brief 正常回路の次状態の変数インデックスを得る．
inline
VarId
IdxMapper::next_normal_idx(ymuint pos) const
{
  return VarId(mOutputNum + pos * 2 + 1);
}

// @brief 故障回路の現状態の変数インデックスを得る．
inline
VarId
IdxMapper::cur_error_idx(ymuint pos) const
{
  return VarId(mOutputNum + mFFNum * 2 + pos * 2 + 0);
}

// @brief 故障回路の次状態の変数インデックスを得る．
inline
VarId
IdxMapper::next_error_idx(ymuint pos) const
{
  return VarId(mOutputNum + mFFNum * 2 + pos * 2 + 1);
}

// @brief 現状態のエラービット
inline
VarId
IdxMapper::cur_error_bit() const
{
  return VarId(mOutputNum + mFFNum * 4);
}

// @brief 次状態のエラービット
inline
VarId
IdxMapper::next_error_bit() const
{
  return VarId(mOutputNum + mFFNum * 4 + 1);
}

// @brief 現状態の等価状態ビット
inline
VarId
IdxMapper::cur_ident_bit() const
{
  return VarId(mOutputNum + mFFNum * 4 + 2);
}

// @brief 次状態の等価状態ビット
inline
VarId
IdxMapper::next_ident_bit() const
{
  return VarId(mOutputNum + mFFNum * 4 + 3);
}

// @brief 状態からエラーを含んだ状態対を作る．
inline
State
IdxMapper::make_error_state(State normal_state,
			    ymuint error_pos) const
{
  State error_state = normal_state;
  ASSERT_COND( error_pos < error_state.size() );
  if ( error_state[error_pos] == '1' ) {
    error_state[error_pos] = '0';
  }
  else {
    error_state[error_pos] = '1';
  }
  return normal_state + error_state + "00";
}

END_NAMESPACE_YM_SEAL

#endif // LIBYM_SEAL_IDXMAPPER_H
