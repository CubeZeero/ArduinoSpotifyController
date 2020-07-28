
import time
import requests
import serial
import json
import sys
import atexit
import webbrowser

comport_open = open('lib\ComPort.txt')
comport = comport_open.read()
comport_open.close

PP_sw = 0
R_sw = 0
S_sw = 0

sendSetting = ''
cw_count = 0
volume = 0
volume_str = ""
volume_str_r = ""
volume_co = ""
volume_tmp = 0
mute_sw = 0

authorization_base_url = "https://accounts.spotify.com/authorize"
token_url = "https://accounts.spotify.com/api/token"
redirect_url = 'http://localhost:12002'
client_id = r''
client_secret = ''

scope = [
    "user-read-playback-state",
    "user-modify-playback-state",
    "user-read-private"
]

headersCT = {'Content-Type': 'application/x-www-form-urlencoded'}

with open('lib\\access_token.txt', 'r', encoding='utf-8') as access_token_open:
    token = access_token_open.read()

with open('lib\\refresh_token.txt', 'r', encoding='utf-8') as refresh_token_open:
    refresh_token = refresh_token_open.read()

if token == "":
    print('Start Initialization.exe and complete the authentication.\n\rInitialization.exeを起動し、認証を完了させてください')
    time.sleep(5)
    sys.exit()

headers = {'Authorization': 'Bearer '+ token}

PStatus = requests.get('https://api.spotify.com/v1/me',headers=headers)
PStatusJS = PStatus.json()

def checktoken():
    global PStatusJS
    if 'error' in PStatusJS.keys():
        if str(PStatusJS["error"]["message"]) in 'The access token expired':
            PStatus = requests.post('https://accounts.spotify.com/api/token',headers=headersCT,data={'grant_type': 'refresh_token','refresh_token': refresh_token,'client_id': client_id,'client_secret': client_secret})
            PStatusJS = PStatus.json()
            with open('lib\\access_token.txt', 'w', encoding='utf-8') as access_token_open:
                access_token_open.write(str(PStatusJS["access_token"]))
                token = str(PStatusJS["access_token"])
                headers = {'Authorization': 'Bearer '+ token}
                PStatus = requests.get('https://api.spotify.com/v1/me',headers=headers)
                PStatusJS = PStatus.json()

def sync_set():

    global PStatusJS

    if str(PStatusJS["is_playing"]) in 'True':
        sendSetting = '0'
        PP_sw = 0
    else:
        sendSetting = '1'
        PP_sw = 1

    ArduinoS.write(int(sendSetting).to_bytes(1,'big'))
    time.sleep(0.2)

    if str(PStatusJS["shuffle_state"]) in 'True':
        sendSetting = '1'
        S_sw = 1
    else:
        sendSetting = '0'
        S_sw = 0

    ArduinoS.write(int(sendSetting).to_bytes(1,'big'))
    time.sleep(0.2)

    if str(PStatusJS["repeat_state"]) in 'track':
        sendSetting = '2'
        R_sw = 2
    elif str(PStatusJS["repeat_state"]) in 'context':
        sendSetting = '1'
        R_sw = 1
    elif str(PStatusJS["repeat_state"]) in 'off':
        sendSetting = '0'
        R_sw = 0

    ArduinoS.write(int(sendSetting).to_bytes(1,'big'))
    time.sleep(0.2)

    volume_set();
    ArduinoS.write(int(volume).to_bytes(1,'big'))
    time.sleep(0.8)

checktoken();

if str(PStatusJS["product"]) in 'free':
    print('Only available for premium accounts.')
    time.sleep(5)
    sys.exit()

try:
    ArduinoS = serial.Serial()
    ArduinoS.port = comport
    ArduinoS.baudrate = 9600
    #ArduinoS.setDTR(False)
    ArduinoS.open()
except:
    print('Could not connect to ' + comport + '.\n\r' + comport + 'に接続できませんでした')
    time.sleep(5)
    sys.exit()

try:
    PStatus = requests.get('https://api.spotify.com/v1/me/player',headers=headers)
    PStatusJS = PStatus.json()
except:
    print('You need to play the song in the Spotify player and activate it.\n\rSpotifyのプレイヤーで曲を再生しアクティブ状態にする必要があります')
    time.sleep(5)
    sys.exit()

def volume_set():
    global PStatusJS
    global volume
    volume = int(PStatusJS['device']['volume_percent'])
    if volume > 0 and volume < 100:
        if volume <= 9:
            if volume <= 5:
                volume = 0
            elif volume >= 5:
                volume = 5
        else:
            volume_str = str(volume)
            volume_co = volume_str[1]
            if int(volume_co) <= 4:
                volume_co = "0"
            elif int(volume_co) >= 5:
                volume_co = "5"
            volume_str_r = volume_str[0] + volume_co
            volume = int(volume_str_r)

volume_set();

if str(PStatusJS['device']['is_active']) in 'True':
    print('Confirm active.\n\rプレイヤーデバイスのアクティブを確認\n\r')
else:
    print('You need to play the song in the Spotify player and activate it.\n\rSpotifyのプレイヤーで曲を再生しアクティブ状態にする必要があります')
    time.sleep(5)
    sys.exit()

time.sleep(3)

print('Waiting for connection.\n\r接続待機中\n\r')

ArduinoRA = ArduinoS.read()
responce = int.from_bytes(ArduinoRA,'big')

print('Syncing settings.\n\r設定を同期しています\n\r')

sync_set();

print('Connected to ' + comport + '\n\r' + comport + 'に接続しました\n\r')

#clientIS.close()

while True:

    checktoken();

    ArduinoRA = ArduinoS.read_all()

    #responce = int.from_bytes(ArduinoRA,'big')
    time.sleep(0.05)

    if 'error' in PStatusJS.keys():
        checktoken()

    if str(ArduinoRA) in str("b\'pp\'"):
        if PP_sw == 0:
            requests.put('https://api.spotify.com/v1/me/player/pause',headers=headers)
            PP_sw = 1
        else:
            requests.put('https://api.spotify.com/v1/me/player/play',headers=headers)
            PP_sw = 0

    if str(ArduinoRA) in str("b\'pr\'"):
        requests.post('https://api.spotify.com/v1/me/player/previous',headers=headers)

    if str(ArduinoRA) in str("b\'n\'"):
        requests.post('https://api.spotify.com/v1/me/player/next',headers=headers)

    if str(ArduinoRA) in str("b\'rp\'"):
        if R_sw == 0:
            requests.put('https://api.spotify.com/v1/me/player/repeat',params={'state': 'context'},headers=headers)
            R_sw = 1
        elif R_sw == 1:
            requests.put('https://api.spotify.com/v1/me/player/repeat',params={'state': 'track'},headers=headers)
            R_sw = 2
        elif R_sw == 2:
            requests.put('https://api.spotify.com/v1/me/player/repeat',params={'state': 'off'},headers=headers)
            R_sw = 0

    if str(ArduinoRA) in str("b\'s\'"):
        if S_sw == 0:
            requests.put('https://api.spotify.com/v1/me/player/shuffle',params={'state': 'true'},headers=headers)
            S_sw = 1
        elif S_sw == 1:
            requests.put('https://api.spotify.com/v1/me/player/shuffle',params={'state': 'false'},headers=headers)
            S_sw = 0

    if str(ArduinoRA) in str("b\'vp\'"):
        if mute_sw == 1:
            mute_sw = 0
        volume += 5
        if volume < 0 or volume > 100:
            volume -= 5
        res =requests.put('https://api.spotify.com/v1/me/player/volume',params={'volume_percent': str(volume)},headers=headers)

    if str(ArduinoRA) in str("b\'vm\'"):
        volume -= 5
        if volume < 0 or volume > 100:
            volume += 5
        requests.put('https://api.spotify.com/v1/me/player/volume',params={'volume_percent': str(volume)},headers=headers)

    if str(ArduinoRA) in str("b\'mt\'"):
        if mute_sw == 0:
            volume_tmp = volume
            volume = 0
            requests.put('https://api.spotify.com/v1/me/player/volume',params={'volume_percent': "0"},headers=headers)
            mute_sw = 1
        else:
            volume = volume_tmp
            requests.put('https://api.spotify.com/v1/me/player/volume',params={'volume_percent': str(volume_tmp)},headers=headers)
            mute_sw = 0

    if str(ArduinoRA) in str("b\'sn\'"):
        print('Resyncing settings.\n\r設定を再同期しています\n\r')
        time.sleep(2)
        PStatus = requests.get('https://api.spotify.com/v1/me/player',headers=headers)
        PStatusJS = PStatus.json()
        volume_set();
        PStatus = requests.get('https://api.spotify.com/v1/me/player',headers=headers)
        PStatusJS = PStatus.json()
        sync_set();

    if str(ArduinoRA) in str("b\'0n\'"):
        PStatus = requests.get('https://api.spotify.com/v1/me/player',headers=headers)
        PStatusJS = PStatus.json()
        progress_s = str(PStatusJS["progress_ms"])
        progress_rs = int(progress_s[-3:])
        progress_int_s = int(progress_s.replace(str(progress_rs),""))
        progress_int_s += 5
        requests.put('https://api.spotify.com/v1/me/player/seek',params={'position_ms': str(progress_int_s)+"000"},headers=headers)

    if str(ArduinoRA) in str("b\'1n\'"):
        PStatus = requests.get('https://api.spotify.com/v1/me/player',headers=headers)
        PStatusJS = PStatus.json()
        progress_s = str(PStatusJS["progress_ms"])
        progress_rs = int(progress_s[-3:])
        progress_int_s = int(progress_s.replace(str(progress_rs),""))
        progress_int_s += 10
        requests.put('https://api.spotify.com/v1/me/player/seek',params={'position_ms': str(progress_int_s)+"000"},headers=headers)

    if str(ArduinoRA) in str("b\'0b\'"):
        PStatus = requests.get('https://api.spotify.com/v1/me/player',headers=headers)
        PStatusJS = PStatus.json()
        progress_s = str(PStatusJS["progress_ms"])
        progress_rs = int(progress_s[-3:])
        progress_int_s = int(progress_s.replace(str(progress_rs),""))
        if progress_int_s < 5:
            progress_int_s = 0
            requests.put('https://api.spotify.com/v1/me/player/seek',params={'position_ms': str(progress_int_s)+"000"},headers=headers)
        else:
            progress_int_s -= 5
            requests.put('https://api.spotify.com/v1/me/player/seek',params={'position_ms': str(progress_int_s)+"000"},headers=headers)

    if str(ArduinoRA) in str("b\'1b\'"):
        PStatus = requests.get('https://api.spotify.com/v1/me/player',headers=headers)
        PStatusJS = PStatus.json()
        progress_s = str(PStatusJS["progress_ms"])
        progress_rs = int(progress_s[-3:])
        progress_int_s = int(progress_s.replace(str(progress_rs),""))
        if progress_int_s < 10:
            progress_int_s = 0
            requests.put('https://api.spotify.com/v1/me/player/seek',params={'position_ms': str(progress_int_s)+"000"},headers=headers)
        else:
            progress_int_s -= 10
            requests.put('https://api.spotify.com/v1/me/player/seek',params={'position_ms': str(progress_int_s)+"000"},headers=headers)
