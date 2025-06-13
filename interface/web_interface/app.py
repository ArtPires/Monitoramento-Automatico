from flask import Flask, render_template, request, jsonify
from utils.tcp import send_tcp_command, get_tcp_status

app = Flask(__name__)

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/command', methods=['POST'])
def command():
    data = request.get_json()
    cmd = data.get('command')
    result = send_tcp_command(cmd)
    return jsonify({'response': result})

@app.route('/status', methods=['GET'])
def status():
    status_data = get_tcp_status()
    return jsonify({'status': status_data})

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000, debug=True)
