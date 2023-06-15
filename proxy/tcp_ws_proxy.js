const net = require('node:net')
const {WebSocketServer} = require('ws')

const connections = {
    tcp: null,
    ws: null,
}

const tcpServer = net.createServer({}, (socket) => {
    console.log('tcp connected')
    connections.tcp = socket

    socket.on('data', (data) => {
        console.log('tcp read', data.length, 'bytes')
        if (connections.ws) {
            connections.ws.send(data)
        }
    })

    socket.on('error', (err) => {
        if (err.code !== 'ECONNRESET') {
            console.log('tcp error', err)
            process.exit(1)
        }
    })

    socket.on('close', () => {
        console.log('tcp closed')
        connections.tcp = null
    })
})

tcpServer.listen('8080', () => {
    console.log('tcp on 8080')
})

const wss = new WebSocketServer({port: 8081}, () => {
    console.log('ws on 8081')
})

wss.on('connection', (ws) => {
    console.log('ws connected')
    connections.ws = ws

    ws.on('error', (err) => {
        console.error('ws err', err)
    })

    ws.on('message', (data) => {
        console.log('ws msg', data)
    })

    ws.on('close', () => {
        console.log('ws closed')
        connections.ws = null
    })
})
