from flask import Flask
app = Flask(__name__)

@app.route('/')
def hello_world():
        return 'Hello, World!'

@app.route('/1')
def sub_url1():
            return 'sub url /1!'

