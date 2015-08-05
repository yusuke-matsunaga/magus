# magus

## [***注意***] git でリポジトリを取得する場合の注意事項

magus は内部で ym-common という別のリポジトリを submodule として使っています．
magus をただ git clone で持ってきた場合には ym-common が空のディレクトリになっていますので

```shell
$ git submodule init
$ git submodule update
```

で ym-common の中身を持ってくるようにしてください．

## はじめに

magus は論理回路の検証・合成アルゴリズムの研究・開発のためのプログラムです．
 内部で YmTools と YmTclpp のライブラリ群を使っています．
 ビルドに関しては BUILDING.md ファイルを参照してください．
