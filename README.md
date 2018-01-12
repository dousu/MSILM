# MSILMとは
Meaning Selection Iterated Learning Model[1]

S. Kirby(2002)が提案したILM(Iterated Learning Model, 繰り返し学習モデル)[2]に意味の不確定性を加えたモデルです．KirbyのILMでは，親が発話した意図を子が直接読み取れてしまうモデルでした．現実の環境では，親と子が共通の意味を想起できることは少ないはずです．そこで，親と子に複数の意味を提示するモデルを考案しました．親は提示された意味から一つ選びその発話を生成する．子は親の発話が提示された意味のどれから生成されたか推測して学習するモデルです[1]．

また，子が親の選んだ意味を推測する手段として対称性バイアスを使う実装も含まれています．対称性バイアスによる推論は2種類あり，MSILMで実装しているのは厳格な対称性バイアス(symmetry)と曖昧性を含む対称性バイアス(ucsymmetry)です[3,4]．

<sup>1</sup>須藤 洸基, 的場 隆一, “意味の多重性を考慮した文法獲得モデルの構築”, 人工知能学会全国大会（第27回）, 富山, 6月, 2013.

<sup>2</sup>Kirby, S. (2002). Learning, Bottlenecks and the Evolution of Recursive Syntax. In: T. Briscoe. (ed.) Linguistic Evolution through Language Acquisition: Formal and Computational Models, pp.173-203. Cambridge: Cambridge University Press.

<sup>3</sup>Ryuichi Matoba, Hiroki Sudo, Makoto Nakamura, Satoshi Tojo, “Application of Loose Symmetry Bias to Multiple Meaning Environment”, The Seventh International Conference on Advanced Cognitive Technologies and Applications, Nice, France, March, 2015.

<sup>4</sup>Sudo, H., Matoba, R., Cooper, T., Tsukada, A., “Effect of symmetry bias on linguistic evolution”, In: Journal of Artificial Life and Robotics, Springer Japan, Vol. 21, No. 2, pp. 207-214, 2016.


## 構成要素
フォルダは，rubyの実行ユーティリティ"EXE"，本体ソースコード群"SOURCE", 実行結果用フォルダ"RESULT"で構成されています．
本プログラムを使用する場合は，"SOURCE"フォルダにて各実行環境でmakeを実行して実行ファイルを作り，EXEフォルダの実行ユーティリティをrubyで実行してください．


## 実行環境
下記実行環境をあらかじめ用意してください．

    ruby 2.4
  
    boost 1.59推奨
  
    c++14
  
    gnuplot


## make&実行
makeターゲットはmsとなっています．

        make ms

msilm.exeという実行ファイルができます．msilm.exeにオプションを付けて実行してください．オプションは-hで確認できます．

        msilm.exe -h
  

## 実行ユーティリティ
rubyによる実行ユーティリティを使うことで，複数スレッドでのシード値を変更しての実行，統計処理，グラフ描画が簡単にできます，初期設定では，世代数100，実行回数100，スレッド数5にて実行されます．実行回数ごとにシード値は変わります．第一引数に実行結果の入る（入っている）フォルダを指定してください．exe_count.rbでは--path, --dictionaryオプションは指定不要です．EXEフォルダを作業ディレクトリとして実行されることを想定した--dictionaryオプションが自動で付きますので，EXEフォルダにてexe_count.rbを実行してください．

例：
    ruby exe_count.rb ../RESULT --keep-random-rule --analyze --logging --generation 100 --accuracy-meaning --ucsymmetry
