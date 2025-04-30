from http.server import BaseHTTPRequestHandler, HTTPServer
import requests

host_map = {
    'mel-akar.fr.42': 'http://127.0.0.1:443',
    'mafihlkifaya': 'http://127.0.0.1:443'
}

class ProxyHandler(BaseHTTPRequestHandler):
    def do_GET(self):
        hostname = self.headers['Host']
        target = host_map.get(hostname)

        if target:
            r = requests.get(f"{target}{self.path}")
            self.send_response(r.status_code)
            for key, value in r.headers.items():
                self.send_header(key, value)
            self.end_headers()
            self.wfile.write(r.content)
        else:
            self.send_response(502)
            self.end_headers()
            self.wfile.write(b'Bad Gateway: Unknown Host')

server = HTTPServer(('localhost', 3000), ProxyHandler)
print('Proxy running on http://localhost:3000')
server.serve_forever()
