const print = require('./printer');

test('inserts a hard line break after a top-level expression', () => {
  expect(
    print([
      { type: 'leftPar', value: '(' },
      { type: 'leftPar', value: '(' },
      { type: 'rightPar', value: ')' },
      { type: 'rightPar', value: ')' },
      { type: 'leftPar', value: '(' },
      { type: 'rightPar', value: ')' },
    ]),
  ).toEqual('(())\n()\n');
});

test('inserts a space between tokens, but not inside of parens', () => {
  expect(
    print([
      { type: 'leftPar', value: '(' },
      { type: 'operator', value: '=' },
      { type: 'keyword', value: 'if' },
      { type: 'identifier', value: 'abc' },
      { type: 'rightPar', value: ')' },
    ]),
  ).toEqual('(= if abc)\n');
});

test('inserts an empty line if such a token occurs', () => {
  expect(
    print([
      { type: 'leftPar', value: '(' },
      { type: 'emptyLine', value: '' },
      { type: 'rightPar', value: ')' },
    ]),
  ).toEqual('(\n\n)\n');
});

test('inserts only one empty line even if it follows a hard line break', () => {
  expect(
    print([
      { type: 'leftPar', value: '(' },
      { type: 'rightPar', value: ')' },
      { type: 'emptyLine', value: '' },
      { type: 'leftPar', value: '(' },
      { type: 'rightPar', value: ')' },
    ]),
  ).toEqual('()\n\n()\n');
});

test('inserts a soft line break if the next token does not fit on the same line', () => {
  const identifier = Array(print.MAX_LINE_LENGTH - 2)
    .fill('x')
    .join('');
  expect(
    print([
      { type: 'leftPar', value: '(' },
      { type: 'identifier', value: identifier },
      { type: 'identifier', value: 'x' },
      { type: 'rightPar', value: ')' },
    ]),
  ).toEqual(`(${identifier}\nx)\n`);
});

test('does not insert a soft line break if the next token just barely fits on the same line', () => {
  const identifier = Array(print.MAX_LINE_LENGTH - 4)
    .fill('x')
    .join('');
  expect(
    print([
      { type: 'leftPar', value: '(' },
      { type: 'identifier', value: identifier },
      { type: 'identifier', value: 'x' },
      { type: 'rightPar', value: ')' },
    ]),
  ).toEqual(`(${identifier} x)\n`);
});

test('does not calculate line length based on a space that was replaced by a line break', () => {
  const identifier = Array(print.MAX_LINE_LENGTH - 1)
    .fill('x')
    .join('');
  expect(
    print([
      { type: 'leftPar', value: '(' },
      { type: 'identifier', value: 'x' },
      { type: 'identifier', value: identifier },
      { type: 'rightPar', value: ')' },
    ]),
  ).toEqual(`(x\n${identifier})\n`); // rightPar should not be on new line
});
