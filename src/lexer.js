const operators = ['=', '+', '-', '*', '/'];
const prefixes = ["'", '&'];
const booleans = ['true', 'false'];
const keywords = ['quote', 'lambda', 'defun', 'let', 'if', 'and', 'or'];

const whitespaceRegex = /\s/;
const alphaRegex = /[a-zA-Z]/;
const numRegex = /[0-9]/;
const alphaNumRegex = /[a-zA-Z0-9]/;

const tokenize = code => {
  let position = 0;
  const tokens = [];
  while (position < code.length) {
    let char = code[position];

    // whitespace
    if (whitespaceRegex.test(char)) {
      position++;
      continue;
    }

    // parentheses
    if (char === '(') {
      tokens.push({ type: 'leftPar', value: char });
      position++;
      continue;
    }
    if (char === ')') {
      tokens.push({ type: 'rightPar', value: char });
      position++;
      continue;
    }

    // operator
    if (operators.includes(char)) {
      tokens.push({ type: 'operator', value: char });
      position++;
      continue;
    }

    // prefix
    if (prefixes.includes(char)) {
      tokens.push({ type: 'prefix', value: char });
      position++;
      continue;
    }

    // number literal
    if (numRegex.test(char)) {
      let numberLiteral = '';

      do {
        if (numRegex.test(char)) numberLiteral += char;
        char = code[++position];
      } while (numRegex.test(char));

      tokens.push({ type: 'numLiteral', value: numberLiteral });
      continue;
    }

    // identifier, keyword, boolean literal
    if (alphaRegex.test(char)) {
      let name = '';

      do {
        if (alphaNumRegex.test(char)) name += char;
        char = code[++position];
      } while (alphaNumRegex.test(char));

      let type;
      if (booleans.includes(name)) type = 'boolLiteral';
      else if (keywords.includes(name)) type = 'keyword';
      else type = 'identifier';

      tokens.push({ type, value: name });
      continue;
    }

    throw new Error(`unexpected character ${char} at position ${position}`);
  }

  return tokens;
};

module.exports = tokenize;
