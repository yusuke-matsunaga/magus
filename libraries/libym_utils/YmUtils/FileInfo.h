#ifndef YMUTILS_FILEINFO_H
#define YMUTILS_FILEINFO_H

/// @file YmUtils/FileInfo.h
/// @brief FinleInfo のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmTools.h"


BEGIN_NAMESPACE_YM

class FileLoc;

//////////////////////////////////////////////////////////////////////
/// @class FileInfo FileInfo.h "YmUtils/FileInfo.h"
/// @brief ファイルの情報を表すクラス
///
/// 具体的には
/// - ファイル名
/// - インクルード元のファイル位置
/// の情報を持つ．
/// 実際には 16 ビットの整数のみを持つというトリッキーな実装
/// @sa FileInfoMgr
//////////////////////////////////////////////////////////////////////
class FileInfo
{
public:

  /// @brief 空のコンストラクタ
  /// @note 無効な ID で初期化される．
  FileInfo();

  /// @brief ファイル名を指定したコンストラクタ
  /// @param[in] filename ファイル名
  FileInfo(const char* filename);

  /// @brief ファイル名を指定したコンストラクタ
  /// @param[in] filename ファイル名
  FileInfo(const string& filename);

  /// @brief ファイル名とインクルード元の親ファイルの情報
  /// @param[in] filename ファイル名
  /// @param[in] parent_loc インクルード元の親ファイルの情報
  FileInfo(const char* filename,
	   const FileLoc& parent_loc);

  /// @brief ファイル名とインクルード元の親ファイルの情報
  /// @param[in] filename ファイル名
  /// @param[in] parent_loc インクルード元の親ファイルの情報
  FileInfo(const string& filename,
	   const FileLoc& parent_loc);

  /// @brief デストラクタ
  ~FileInfo();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部に公開している関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 有効なデータを持っているときに true を返す．
  bool
  is_valid() const;

  /// @brief ID番号を得る．
  /// @note ほとんど等価比較演算子のための関数
  ymuint
  id() const;

  /// @brief ファイル名を返す．
  string
  filename() const;

  /// @brief インクルード元のファイル位置を返す．
  /// @note インクルードされていないファイルの場合には無効なデータが返される．
  FileLoc
  parent_loc() const;

  /// @brief インクルード元のファイル位置の情報をすべてリストに積む．
  /// @param[out] loc_list ファイルの位置情報のリスト
  /// @note トップレベルのファイルが先頭になる．
  void
  parent_loc_list(list<FileLoc>& loc_list) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ID番号
  ymuint16 mId;

};


//////////////////////////////////////////////////////////////////////
/// @name FileInfo に関係する演算子
/// @{

/// @relates FileInfo
/// @brief 等価比較演算子
/// @param[in] left, right オペランド
bool
operator==(FileInfo left,
	   FileInfo right);

/// @relates FileInfo
/// @brief 非等価比較演算子
/// @param[in] left, right オペランド
bool
operator!=(FileInfo left,
	   FileInfo right);

/// @relates FileInfo
/// @brief FileInfo 用のストリーム出力演算子
/// @param[in] s 出力ストリーム
/// @param[in] file_info ファイル情報
/// @return s をそのまま返す
ostream&
operator<<(ostream& s,
	   FileInfo file_info);

/// @}
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 有効なデータを持っているときに true を返す．
inline
bool
FileInfo::is_valid() const
{
  return mId != 0xFFFF;
}

// @brief ID番号を得る．
inline
ymuint
FileInfo::id() const
{
  return mId;
}

// @relates FileInfo
// @brief 等価比較演算子
// @param[in] left, right オペランド
inline
bool
operator==(FileInfo left,
	   FileInfo right)
{
  return left.id() == right.id();
}

// @relates FileInfo
// @brief 非等価比較演算子
// @param[in] left, right オペランド
inline
bool
operator!=(FileInfo left,
	   FileInfo right)
{
  return !operator==(left, right);
}

END_NAMESPACE_YM

#endif // YMUTILS_FILEINFO_H
