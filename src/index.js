const tokenize = require('./lexer');
const print = require('./printer');

const format = code => print(tokenize(code));

format.tokenize = tokenize;
format.print = print;

module.exports = format;
