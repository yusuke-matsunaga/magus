#ifndef IGUGEN_H
#define IGUGEN_H

/// @file IguGen.h
/// @brief IguGen のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "igf_nsdef.h"
#include "RvMgr.h"
#include "FuncVect.h"


BEGIN_NAMESPACE_YM_IGF

//////////////////////////////////////////////////////////////////////
/// @class IguGen IguGen.h "IguGen.h"
/// @brief IGU を合成するクラス
//////////////////////////////////////////////////////////////////////
class IguGen
{
public:

  /// @brief コンストラクタ
  IguGen();

  /// @brief デストラクタ
  ~IguGen();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 登録ベクタを読み込む．
  /// @param[in] s 読み込み元のストリーム演算子
  /// @retval true 読み込みが成功した．
  /// @retval false 読み込みが失敗した．
  bool
  read_data(istream& s);

  /// @brief ベクタのサイズを得る．
  ///
  /// ベクタのサイズとはベクタのビット長
  ymuint
  vect_size() const;

  /// @brief ベクタのリストを得る．
  const vector<const RegVect*>&
  vect_list() const;

  /// @brief インデックスのサイズを得る．
  ///
  /// インデックスのサイズとはインデックスを2進符号化するのに
  /// 必要なビット数 = ceil(log (k + 1)): k はベクタ数
  ymuint
  index_size() const;

  /// @brief conflict free 分割法で構成する．
  /// @param[in] h_funcs 入力ハッシュ関数のリスト
  /// @param[out] map_list IGUごとのベクタ番号のリスト
  /// @retval true 構成できた．
  /// @retval false 構成が失敗した．
  bool
  cfp(const vector<InputFunc*>& h_funcs,
      vector<vector<ymuint> >& map_list) const;

  /// @brief naive parallel 法で構成する．
  /// @param[in] h_funcs 入力ハッシュ関数のリスト
  /// @param[out] map_list IGUごとのベクタ番号のリスト
  /// @retval true 構成できた．
  /// @retval false 構成が失敗した．
  bool
  naive(const vector<InputFunc*>& h_funcs,
	vector<vector<ymuint> >& map_list) const;

  /// @brief minimum perfect hash 法で構成する．
  /// @param[in] h_funcs 入力ハッシュ関数のリスト
  /// @param[out] map_list IGUごとの G 関数のリスト
  /// @retval true 構成できた．
  /// @retval false 構成が失敗した．
  bool
  mphf(const vector<InputFunc*>& h_funcs,
       vector<vector<ymuint> >& map_list) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief InputFunc から FuncVect に変換する．
  void
  convert(const vector<InputFunc*>& h_funcs,
	  vector<const FuncVect*>& func_list) const;

  /// @brief func_list を削除する．
  void
  free_func_list(const vector<const FuncVect*>& func_list) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 登録ベクタを管理するクラス
  RvMgr mRvMgr;

};

END_NAMESPACE_YM_IGF

#endif // IGUGEN_H
