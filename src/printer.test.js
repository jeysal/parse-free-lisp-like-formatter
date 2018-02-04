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
  ).toEqual(`(${identifier}\n  x)\n`);
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
  const identifier = Array(print.MAX_LINE_LENGTH - 3)
    .fill('x')
    .join('');
  expect(
    print([
      { type: 'leftPar', value: '(' },
      { type: 'identifier', value: 'xx' },
      { type: 'identifier', value: identifier },
      { type: 'rightPar', value: ')' },
    ]),
  ).toEqual(`(xx\n  ${identifier})\n`); // rightPar should not be on new line
});

test('uses space beyond the line length limit for further tokens once the limit has been violated', () => {
  const tooLargeIdentifier = Array(print.MAX_LINE_LENGTH - 1)
    .fill('x')
    .join('');
  const halfLineIdentifier = Array(print.MAX_LINE_LENGTH / 2 - 1)
    .fill('x')
    .join('');
  expect(
    print([
      { type: 'leftPar', value: '(' },
      { type: 'identifier', value: 'x' },
      { type: 'identifier', value: tooLargeIdentifier },
      { type: 'identifier', value: halfLineIdentifier },
      { type: 'identifier', value: halfLineIdentifier },
      { type: 'rightPar', value: ')' },
    ]),
  ).toEqual(
    `(x\n  ${tooLargeIdentifier}\n  ${halfLineIdentifier} ${halfLineIdentifier}\n)\n`,
  );
});

test('does not apply the extended line length limit after a hard break', () => {
  const tooLargeIdentifier = Array(print.MAX_LINE_LENGTH - 1)
    .fill('x')
    .join('');
  const exactlyFittingIdentifier = Array(print.MAX_LINE_LENGTH - 2)
    .fill('x')
    .join('');
  expect(
    print([
      { type: 'leftPar', value: '(' },
      { type: 'identifier', value: 'x' },
      { type: 'identifier', value: tooLargeIdentifier },
      { type: 'rightPar', value: ')' },
      { type: 'leftPar', value: '(' },
      { type: 'identifier', value: 'x' },
      { type: 'identifier', value: exactlyFittingIdentifier },
      { type: 'rightPar', value: ')' },
    ]),
  ).toEqual(
    `(x\n  ${tooLargeIdentifier}\n)\n(x\n  ${exactlyFittingIdentifier}\n)\n`,
  );
});

test('indents multiple levels deep', () => {
  const identifier = Array(print.MAX_LINE_LENGTH - 3)
    .fill('x')
    .join('');
  expect(
    print([
      { type: 'leftPar', value: '(' },
      { type: 'identifier', value: identifier },
      { type: 'leftPar', value: '(' },
      { type: 'identifier', value: 'x' },
      { type: 'rightPar', value: ')' },
      { type: 'rightPar', value: ')' },
    ]),
  ).toEqual(`(${identifier} (\n    x))\n`);
});
