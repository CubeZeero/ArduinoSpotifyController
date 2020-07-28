from flask import Flask
from flask import request
import requests
import sys
import os
import json
import webbrowser
import time

redirect_url = 'http://localhost:12002'
client_id = r''
client_secret = ''
webbrowser.open("https://accounts.spotify.com/authorize?client_id="+client_id+"&response_type=code&redirect_uri="+redirect_url+"&scope=user-read-playback-state user-modify-playback-state user-read-private")

app = Flask(__name__)

@app.route('/')
def authorization():
    global client_id
    global client_secret
    global redirect_url
    acode = str(request.args.get('code'))
    name = "<html><h1>Please close the window.<br>このウィンドウは閉じてください</h1></html>"
    PStatus = requests.post('https://accounts.spotify.com/api/token',data={'grant_type': 'authorization_code','code': acode,'redirect_uri': redirect_url,'client_id': client_id,'client_secret': client_secret})
    PStatusJS = PStatus.json()
    with open('lib\\access_token.txt', 'w', encoding='utf-8') as access_token_open:
        access_token_open.write(str(PStatusJS["access_token"]))
    with open('lib\\refresh_token.txt', 'w', encoding='utf-8') as refresh_token_open:
        refresh_token_open.write(str(PStatusJS["refresh_token"]))
    print('\n\rAuthentication is complete. Please close this window.\n\r認証が完了しました。このウィンドウは閉じてください。\n\r')
    return name

## おまじない
if __name__ == "__main__":
    app.run(debug=False, host='0.0.0.0', port=12002)
