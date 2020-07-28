# ArduinoSpotifyController
This is a tool to control Spotify from Arduino TFT LCD.

ArduinoのTFTLCDからSpotifyを制御するためのツールです。

## 機能
* 再生 / 停止
* 次 / 前 の曲へスキップ
* リピートスイッチ
* シャッフルスイッチ
* ボリューム変更
* ミュート
* 5秒の前後シーク
* 10秒の前後シーク

## 確認済み動作環境
Windows 10 Home

## 使用するパーツ
Arduino UNO R3
https://is.gd/Z09Ys0

UNO専用 TFTLCD2.8インチシールド
https://is.gd/kgN2d

(作者が使用しているものと同じです)

**LCDについては2.8インチ推奨です**

LCDを使用する場合はレジスタを設定する必要があります
下記のページを参考にレジスタを特定してください
[http://nopnop2002.webcrow.jp/Arduino-Hard/ArduinoTFTShield.html](http://nopnop2002.webcrow.jp/Arduino-Hard/ArduinoTFTShield.html)

レジスタを特定したら以下のコードを特定したレジスタに書き換えてください
(Arduino_TFTLCDの373行目、setup内にあります)

デフォルト値は**9341**です
(上記のTFTLCDのレジスタです)

```
tft.begin(0x9341);
```

## Arduinoへの書き込み
ArduinoIDEでArduinoへ書き込みを行ってください

使用するライブラリは以下の通りです
* TouchScreen
* Adafruit_TFTLCD
* Adafruit_GFX
* Wire
* Adafruit BusIO

## コントローラー

![controller](https://github.com/CubeZeero/ArduinoSpotifyController/blob/master/img/controller.png)

* 1 - 再生 / 停止
* 2 - 前の曲にスキップ
* 3 - 次の曲へスキップ
* 4 - リピートスイッチ
* 5 - シャッフルスイッチ
* 6 - ボリュームアップ(5%)
* 7 - ボリュームダウン(5%)
* 8 - ミュートスイッチ
* 9 - 5秒次に移動
* 10 - 10秒次に移動
* 11 - 5秒前に移動
* 12 - 10秒前に移動
* 13 - 設定同期

他のデバイスとプレイヤーの状態が異なる場合、設定同期で同期できます

## Release ダウンロード
Windowsのみ.exeとして配布しています
[Release](https://github.com/CubeZeero/ArduinoSpotifyController/releases/)

## License
ArduinoSpotifyController is under [MIT license](https://en.wikipedia.org/wiki/MIT_License).
