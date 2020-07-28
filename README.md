# ArduinoSpotifyController
This is a tool to control Spotify from Arduino TFT LCD.
ArduinoのTFTLCDからSpotifyを制御するためのツールです。

## 機能
・再生 / 停止
・次 / 前 の曲へスキップ
・リピートスイッチ
・シャッフルスイッチ
・ボリューム変更
・ミュート
・5秒の前後シーク
・10秒の前後シーク

## 確認済み動作環境
Windows

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
'''
tft.begin(0x9341);
'''

## Arduinoへの書き込み
ArduinoIDEでArduinoへ書き込みを行ってください

使用するライブラリは以下の通りです
* TouchScreen
* Adafruit_TFTLCD
* Adafruit_GFX
* Wire
* Adafruit BusIO

