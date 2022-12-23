const http = require('http');
const url = require('url');
const fs = require('fs');

const hostname = '127.0.0.1';
const port = 3000;

async function updateTmpTX(newValue) {
  // Read the JSON file
  const jsonString = await fs.promises.readFile(`${process.env.APPDATA}/ChromiaTemp/SSO.dat`, 'utf-8');

  // Parse the JSON string into an object
  const data = JSON.parse(jsonString);

  // Update the __tmpTX key
  data.__tmpTX = newValue;

  // Stringify the updated object and write it back to the file
  await fs.promises.writeFile(`${process.env.APPDATA}/ChromiaTemp/SSO.dat`, JSON.stringify(data));
}

const server = http.createServer((req, res) => {
  // Parse the URL and extract the query parameter
  const parsedUrl = url.parse(req.url, true);
  const rawTx = parsedUrl.query.rawTx;

  if (req.method === 'GET' && req.url.startsWith('/success')) {
    // If the request method is GET and the URL starts with "/success",
    // send a success response with the rawTx parameter
    res.statusCode = 200;
    res.setHeader('Content-Type', 'text/plain');
    if (rawTx == null || rawTx.length == 0)
    {
        res.end(`Error, invalid rawTx\n`);
        updateTmpTX(rawTx);
    }else{
        res.end(`Success!\n`);
        updateTmpTX(rawTx);
    }
   
  } else if (req.method === 'GET' && req.url.startsWith('/error')) {
    res.statusCode = 200;
    res.setHeader('Content-Type', 'text/plain');
    res.end(`Error!\n`);
  } else {
    // Otherwise, send a "Method Not Allowed" response
    res.statusCode = 405;
    res.setHeader('Content-Type', 'text/plain');
    res.end('Method Not Allowed\n');
  }
});

server.listen(port, hostname, () => {
  console.log(`Server running at http://${hostname}:${port}/`);
});