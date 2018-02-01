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
