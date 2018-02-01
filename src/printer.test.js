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
